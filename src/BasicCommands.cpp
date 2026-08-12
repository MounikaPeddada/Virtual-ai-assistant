#include "BasicCommands.h"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cctype>

// small helper: lowercase + does the string contain a keyword
static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}
static bool contains(const std::string& haystack, const std::string& needle) {
    return toLower(haystack).find(needle) != std::string::npos;
}

// ---------------- GreetingCommand ----------------

bool GreetingCommand::matches(const std::string& input) const {
    return contains(input, "hi") || contains(input, "hello") ||
           contains(input, "hey") || contains(input, "bye") ||
           contains(input, "how are you");
}

std::string GreetingCommand::execute(const std::string& input) {
    if (contains(input, "bye")) return "Goodbye! Talk soon.";
    if (contains(input, "how are you")) return "I'm doing well, thanks for asking!";
    return "Hello! How can I help you today?";
}

// ---------------- TimeCommand ----------------

bool TimeCommand::matches(const std::string& input) const {
    return contains(input, "time") || contains(input, "date");
}

std::string TimeCommand::execute(const std::string& input) {
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%A, %d %B %Y - %H:%M:%S", std::localtime(&now));
    return std::string("It's currently ") + buf;
}

// ---------------- CalculatorCommand ----------------

bool CalculatorCommand::matches(const std::string& input) const {
    return contains(input, "add ") || contains(input, "subtract ") ||
           contains(input, "multiply ") || contains(input, "divide ");
}

std::string CalculatorCommand::execute(const std::string& input) {
    std::istringstream iss(toLower(input));
    std::string op, aWord, andWord, bWord;
    double a = 0, b = 0;

    iss >> op >> a >> andWord >> b;

    if (iss.fail()) {
        return "Sorry, try phrasing it like 'add 5 and 3'.";
    }

    double result = 0;
    if (op == "add") result = a + b;
    else if (op == "subtract") result = a - b;
    else if (op == "multiply") result = a * b;
    else if (op == "divide") {
        if (b == 0) return "Can't divide by zero!";
        result = a / b;
    } else {
        return "I only know add, subtract, multiply, divide.";
    }

    std::ostringstream oss;
    oss << "Result: " << result;
    return oss.str();
}

// ---------------- KnowledgeCommand ----------------

KnowledgeCommand::KnowledgeCommand() {
    knowledgeBase = {
        {"your name", "I'm your C++ virtual assistant, built for an OOP class project."},
        {"who made you", "I was built as a student project demonstrating OOP in C++."},
        {"what can you do", "I can chat, take notes, manage tasks, do basic math, and tell the time."}
    };
}

bool KnowledgeCommand::matches(const std::string& input) const {
    std::string lower = toLower(input);
    for (const auto& pair : knowledgeBase) {
        if (lower.find(pair.first) != std::string::npos) return true;
    }
    return false;
}

std::string KnowledgeCommand::execute(const std::string& input) {
    std::string lower = toLower(input);
    for (const auto& pair : knowledgeBase) {
        if (lower.find(pair.first) != std::string::npos) return pair.second;
    }
    return "I'm not sure how to answer that yet.";
}

// ---------------- HelpCommand ----------------

bool HelpCommand::matches(const std::string& input) const {
    return contains(input, "help") || contains(input, "commands");
}

std::string HelpCommand::execute(const std::string& input) {
    if (!allDescriptions) return "No commands registered yet.";
    std::string out = "Here's what I can do:\n";
    for (const auto& d : *allDescriptions) {
        out += "  - " + d + "\n";
    }
    return out;
}

// ---------------- QuitCommand ----------------

bool QuitCommand::matches(const std::string& input) const {
    return contains(input, "quit") || contains(input, "exit");
}

std::string QuitCommand::execute(const std::string& input) {
    wasTriggered = true;
    return "Shutting down. Goodbye!";
}
