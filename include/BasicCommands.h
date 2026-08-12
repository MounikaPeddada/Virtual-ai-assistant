#ifndef BASIC_COMMANDS_H
#define BASIC_COMMANDS_H

#include "Command.h"
#include <unordered_map>
#include <vector>

// Each class below is a separate, focused Command. Assistant doesn't
// know or care which one runs - it just calls matches() on each until
// one says yes, then calls execute() on that one. That's polymorphism
// doing the actual dispatch work.

class GreetingCommand : public Command {
public:
    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override { return "Say hi / bye"; }
};

class TimeCommand : public Command {
public:
    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override { return "Tell the current time and date"; }
};

class CalculatorCommand : public Command {
public:
    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override { return "Do basic math: 'add 5 and 3', 'multiply 4 and 6'"; }
};

class KnowledgeCommand : public Command {
public:
    KnowledgeCommand();
    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override { return "Answer simple canned questions"; }

private:
    std::unordered_map<std::string, std::string> knowledgeBase;
};

class HelpCommand : public Command {
public:
    // populated by Assistant after all commands are registered
    std::vector<std::string>* allDescriptions = nullptr;
    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override { return "List everything I can do"; }
};

class QuitCommand : public Command {
public:
    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override { return "Exit the assistant"; }
    bool wasTriggered = false;
};

#endif
