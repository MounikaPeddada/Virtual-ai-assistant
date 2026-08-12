#include "NoteCommand.h"
#include <algorithm>
#include <cctype>
#include <sstream>

static std::string toLowerLocal(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

bool NoteCommand::matches(const std::string& input) const {
    std::string lower = toLowerLocal(input);
    return lower.rfind("note ", 0) == 0 || lower == "notes" ||
           lower.rfind("delete note", 0) == 0;
}

std::string NoteCommand::execute(const std::string& input) {
    std::string lower = toLowerLocal(input);

    if (lower.rfind("delete note", 0) == 0) {
        std::istringstream iss(lower);
        std::string word1, word2;
        int idx = -1;
        iss >> word1 >> word2 >> idx;
        if (manager.deleteNote(idx)) {
            return "Deleted note #" + std::to_string(idx) + ".";
        }
        return "Couldn't find a note with that number. Try 'notes' to see the list.";
    }

    if (lower == "notes") {
        auto all = manager.viewNotes();
        if (all.empty()) return "You don't have any notes yet.";
        std::string out = "Your notes:\n";
        for (size_t i = 0; i < all.size(); ++i) {
            out += "  " + std::to_string(i + 1) + ". " + all[i] + "\n";
        }
        return out;
    }

    // "note <text>"
    std::string text = input.substr(5); // strip leading "note "
    std::string saved = manager.addNote(text);
    return "Saved note: \"" + saved + "\"";
}
