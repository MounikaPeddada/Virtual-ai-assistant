#ifndef NOTE_COMMAND_H
#define NOTE_COMMAND_H

#include "Command.h"
#include "NoteManager.h"

// Handles "note ...", "notes", "delete note N".
// Holds a reference to NoteManager rather than owning one, since the
// Assistant owns the actual NoteManager instance.
class NoteCommand : public Command {
public:
    explicit NoteCommand(NoteManager& manager) : manager(manager) {}

    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override {
        return "Notes: 'note <text>', 'notes' (view), 'delete note <n>'";
    }

private:
    NoteManager& manager;
};

#endif
