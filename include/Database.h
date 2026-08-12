#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "Task.h"

// Wraps all SQL access. Nothing outside this class (Assistant, main.cpp,
// commands) ever writes a raw SQL query - they call methods like
// addTask() or listTasks(). That's the "abstraction" the project asks for:
// the rest of the program depends on this interface, not on SQLite itself.
//
// NOTE ON DB CHOICE: this uses SQLite (single file, no server needed -
// easiest to demo/grade). If your course specifically wants MySQL, the
// public interface below stays identical - only the .cpp implementation
// changes to use MySQL Connector/C++ instead of the sqlite3 C API.
class Database {
public:
    explicit Database(const std::string& dbFilePath);
    ~Database();

    // Creates tables if they don't already exist. Call once at startup.
    void initSchema();

    // ---- Tasks ----
    void addTask(const std::string& description);
    bool markTaskDone(int taskId);
    bool deleteTask(int taskId);
    std::vector<Task> listTasks() const;

    // ---- Conversation history ----
    void logConversation(const std::string& userMessage, const std::string& assistantReply);
    std::vector<std::string> recentHistory(int limit) const;

    // ---- User preferences ----
    void setPreference(const std::string& key, const std::string& value);
    std::string getPreference(const std::string& key, const std::string& fallback = "") const;

private:
    sqlite3* db;
    void execOrThrow(const std::string& sql);
};

#endif
