#ifndef TASK_COMMAND_H
#define TASK_COMMAND_H

#include "Command.h"
#include "Database.h"

// Handles "task ...", "tasks", "done <n>", "delete task <n>".
// Talks only through the Database interface - never writes raw SQL itself.
class TaskCommand : public Command {
public:
    explicit TaskCommand(Database& database) : database(database) {}

    bool matches(const std::string& input) const override;
    std::string execute(const std::string& input) override;
    std::string description() const override {
        return "Tasks: 'task <text>', 'tasks' (view), 'done <n>', 'delete task <n>'";
    }

private:
    Database& database;
};

#endif
