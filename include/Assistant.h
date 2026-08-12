#ifndef ASSISTANT_H
#define ASSISTANT_H

#include "Command.h"
#include "BasicCommands.h"
#include "User.h"
#include "Database.h"
#include "NoteManager.h"
#include <vector>
#include <memory>
#include <string>

// The orchestrator. Holds a User, a Database, a NoteManager, and a list
// of Command objects (notes and tasks included - see NoteCommand.h /
// TaskCommand.h). On each input it finds the first matching command and
// calls execute() on it via the base-class pointer: that single call
// site is the polymorphic dispatch at the center of the whole design -
// Assistant never needs to know which concrete Command it's talking to.
class Assistant {
public:
    Assistant(User user, Database& db, NoteManager& notes);

    // Returns the assistant's reply. Also logs the exchange to the DB.
    std::string handleInput(const std::string& input);

    bool shouldQuit() const;

private:
    User user;
    Database& db;
    NoteManager& notes;
    std::vector<std::unique_ptr<Command>> commands;
    std::vector<std::string> commandDescriptions;

    QuitCommand* quitCommandPtr = nullptr; // non-owning, checked by shouldQuit()

    void registerCommands();
};

#endif
