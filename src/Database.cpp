#include "Database.h"
#include <stdexcept>
#include <iostream>

Database::Database(const std::string& dbFilePath) : db(nullptr) {
    if (sqlite3_open(dbFilePath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    }
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

void Database::execOrThrow(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string msg = errMsg ? errMsg : "unknown error";
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + msg);
    }
}

void Database::initSchema() {
    execOrThrow(
        "CREATE TABLE IF NOT EXISTS tasks ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  description TEXT NOT NULL,"
        "  done INTEGER NOT NULL DEFAULT 0"
        ");"
    );

    execOrThrow(
        "CREATE TABLE IF NOT EXISTS conversation_history ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  user_message TEXT NOT NULL,"
        "  assistant_reply TEXT NOT NULL,"
        "  timestamp TEXT DEFAULT CURRENT_TIMESTAMP"
        ");"
    );

    execOrThrow(
        "CREATE TABLE IF NOT EXISTS user_prefs ("
        "  key TEXT PRIMARY KEY,"
        "  value TEXT NOT NULL"
        ");"
    );
}

// ---- Tasks ----

void Database::addTask(const std::string& description) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO tasks (description, done) VALUES (?, 0);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool Database::markTaskDone(int taskId) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE tasks SET done = 1 WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, taskId);
    sqlite3_step(stmt);
    bool changed = sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return changed;
}

bool Database::deleteTask(int taskId) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, taskId);
    sqlite3_step(stmt);
    bool changed = sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return changed;
}

std::vector<Task> Database::listTasks() const {
    std::vector<Task> tasks;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, description, done FROM tasks ORDER BY id;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        bool done = sqlite3_column_int(stmt, 2) != 0;
        tasks.emplace_back(id, description, done);
    }
    sqlite3_finalize(stmt);
    return tasks;
}

// ---- Conversation history ----

void Database::logConversation(const std::string& userMessage, const std::string& assistantReply) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO conversation_history (user_message, assistant_reply) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, userMessage.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, assistantReply.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<std::string> Database::recentHistory(int limit) const {
    std::vector<std::string> lines;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT user_message, assistant_reply, timestamp FROM conversation_history "
                       "ORDER BY id DESC LIMIT ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, limit);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string userMsg = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string reply = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string ts = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        lines.push_back("[" + ts + "] You: " + userMsg + " | Assistant: " + reply);
    }
    sqlite3_finalize(stmt);
    return lines;
}

// ---- User preferences ----

void Database::setPreference(const std::string& key, const std::string& value) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO user_prefs (key, value) VALUES (?, ?) "
                       "ON CONFLICT(key) DO UPDATE SET value = excluded.value;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::string Database::getPreference(const std::string& key, const std::string& fallback) const {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT value FROM user_prefs WHERE key = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);

    std::string result = fallback;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return result;
}
