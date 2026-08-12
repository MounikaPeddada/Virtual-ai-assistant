#ifndef TASK_H
#define TASK_H

#include <string>
#include <ostream>

// Represents one task. Deliberately a real class (not a plain struct)
// so it has something worth encapsulating and a reason to demonstrate
// a friend function below.
class Task {
public:
    Task(int id, const std::string& description, bool done)
        : id(id), description(description), done(done) {}

    int getId() const { return id; }
    const std::string& getDescription() const { return description; }
    bool isDone() const { return done; }
    void markDone() { done = true; }

    // operator<< can't be a member function (the left-hand operand has to
    // be the ostream, not a Task), but it still needs access to the
    // private fields below - that's exactly the case `friend` exists for.
    friend std::ostream& operator<<(std::ostream& os, const Task& t);

private:
    int id;
    std::string description;
    bool done;
};

inline std::ostream& operator<<(std::ostream& os, const Task& t) {
    os << "[" << (t.done ? "x" : " ") << "] #" << t.id << " " << t.description;
    return os;
}

#endif
