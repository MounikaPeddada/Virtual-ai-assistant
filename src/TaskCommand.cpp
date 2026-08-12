#include "TaskCommand.h"
#include <algorithm>
#include <cctype>
#include <sstream>

static std::string toLowerLocal(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

bool TaskCommand::matches(const std::string& input) const {
    std::string lower = toLowerLocal(input);
    return lower.rfind("task ", 0) == 0 || lower == "tasks" ||
           lower.rfind("done ", 0) == 0 || lower.rfind("delete task", 0) == 0;
}

std::string TaskCommand::execute(const std::string& input) {
    std::string lower = toLowerLocal(input);

    if (lower.rfind("delete task", 0) == 0) {
        std::istringstream iss(lower);
        std::string w1, w2;
        int id = -1;
        iss >> w1 >> w2 >> id;
        return database.deleteTask(id) ? "Deleted task #" + std::to_string(id) + "."
                                        : "No task with that ID.";
    }

    if (lower.rfind("done ", 0) == 0) {
        std::istringstream iss(lower);
        std::string w1;
        int id = -1;
        iss >> w1 >> id;
        return database.markTaskDone(id) ? "Marked task #" + std::to_string(id) + " as done."
                                          : "No task with that ID.";
    }

    if (lower == "tasks") {
        auto tasks = database.listTasks();
        if (tasks.empty()) return "You have no tasks.";
        std::string out = "Your tasks:\n";
        for (const auto& t : tasks) {
            std::ostringstream line;
            line << t;               // uses Task's friend operator<<
            out += "  " + line.str() + "\n";
        }
        return out;
    }

    // "task <text>"
    std::string text = input.substr(5);
    database.addTask(text);
    return "Added task: \"" + text + "\"";
}
