#include "Assistant.h"
#include "NoteCommand.h"
#include "TaskCommand.h"

Assistant::Assistant(User user, Database& db, NoteManager& notes)
    : user(std::move(user)), db(db), notes(notes) {
    registerCommands();
}

void Assistant::registerCommands() {
    // Order matters: first match wins, so put more specific matchers
    // (notes/tasks) before general ones where overlap could occur.
    commands.push_back(std::make_unique<NoteCommand>(notes));
    commands.push_back(std::make_unique<TaskCommand>(db));
    commands.push_back(std::make_unique<GreetingCommand>());
    commands.push_back(std::make_unique<TimeCommand>());
    commands.push_back(std::make_unique<CalculatorCommand>());
    commands.push_back(std::make_unique<KnowledgeCommand>());

    auto quit = std::make_unique<QuitCommand>();
    quitCommandPtr = quit.get(); // keep a raw observer pointer before moving ownership
    commands.push_back(std::move(quit));

    // HelpCommand needs to see every other command's description, so it's
    // built last and given a pointer into commandDescriptions.
    for (const auto& cmd : commands) {
        commandDescriptions.push_back(cmd->description());
    }
    auto help = std::make_unique<HelpCommand>();
    help->allDescriptions = &commandDescriptions;
    commandDescriptions.push_back(help->description());
    commands.push_back(std::move(help));
}

std::string Assistant::handleInput(const std::string& input) {
    std::string reply;

    for (auto& cmd : commands) {
        if (cmd->matches(input)) {
            reply = cmd->execute(input);   // <-- polymorphic call: base-class
                                            //     pointer, derived-class behavior
            break;
        }
    }

    if (reply.empty()) {
        reply = "I didn't understand that. Type 'help' to see what I can do.";
    }

    db.logConversation(input, reply);
    return reply;
}

bool Assistant::shouldQuit() const {
    return quitCommandPtr && quitCommandPtr->wasTriggered;
}
