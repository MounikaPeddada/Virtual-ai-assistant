#ifndef USER_H
#define USER_H

#include <string>

// Simple encapsulation example: private data, controlled access
// through getters/setters.
class User {
public:
    explicit User(const std::string& name = "Guest") : name(name) {}

    std::string getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }

private:
    std::string name;
};

#endif
