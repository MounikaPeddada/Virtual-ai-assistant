#include "Assistant.h"
#include "Database.h"
#include "NoteManager.h"
#include "GrammarCorrector.h"
#include <iostream>

int main() {
    std::cout << "=== C++ Virtual Assistant (OOP demo project) ===\n";
    std::cout << "What's your name? ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) name = "Guest";

    User user(name);

    // Database (SQL) for tasks / history / preferences.
    Database db("assistant.db");
    db.initSchema();

    // GrammarCorrector implements the TextProcessor interface.
    // NoteManager only knows about TextProcessor, not GrammarCorrector -
    // swap in a different processor later with zero changes here.
    GrammarCorrector corrector;
    NoteManager notes("notes.txt", corrector);

    Assistant assistant(user, db, notes);

    std::cout << "\nHi " << user.getName()
              << "! Type 'help' to see what I can do, or 'quit' to exit.\n\n";

    std::string input;
    while (true) {
        std::cout << user.getName() << "> ";
        if (!std::getline(std::cin, input)) break;
        if (input.empty()) continue;

        std::string reply = assistant.handleInput(input);
        std::cout << "Assistant: " << reply << "\n";

        if (assistant.shouldQuit()) break;
    }

    return 0;
}
