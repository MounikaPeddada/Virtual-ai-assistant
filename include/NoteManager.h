#ifndef NOTE_MANAGER_H
#define NOTE_MANAGER_H

#include "TextProcessor.h"
#include <string>
#include <vector>
#include <memory>

// Manages notes as plain lines in a text file. Deliberately NOT using
// SQL here - notes are simple free text with no need for structured
// querying, so a flat file is the leaner choice. Tasks/history/prefs
// (which DO benefit from structured queries) live in the Database class.
class NoteManager {
public:
    // Takes a TextProcessor by reference to the interface (not a concrete
    // GrammarCorrector) so the corrector can be swapped later without
    // changing this class at all.
    NoteManager(const std::string& filePath, TextProcessor& processor);

    // Corrects the text via the processor, then appends it to the file.
    // Returns the corrected text that was actually saved.
    std::string addNote(const std::string& rawText);

    std::vector<std::string> viewNotes() const;

    // 1-based index as shown by viewNotes().
    bool deleteNote(int index);

private:
    std::string filePath;
    TextProcessor& processor;

    std::vector<std::string> readAllLines() const;
    void writeAllLines(const std::vector<std::string>& lines) const;
};

#endif
