#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// Abstract base class for every action the assistant can perform.
// This is the heart of the polymorphism in the project: Assistant
// holds a list of Command* (via unique_ptr) and calls execute()
// on whichever one matches, without knowing the concrete type.
class Command {
public:
    virtual ~Command() = default;

    // Does this command know how to handle this input?
    // e.g. TimeCommand returns true if input contains "time".
    virtual bool matches(const std::string& input) const = 0;

    // Perform the action and return the assistant's reply text.
    virtual std::string execute(const std::string& input) = 0;

    // Shown in a "help" listing.
    virtual std::string description() const = 0;
};

#endif
