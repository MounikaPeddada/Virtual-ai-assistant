#include "NoteManager.h"
#include <fstream>

NoteManager::NoteManager(const std::string& filePath, TextProcessor& processor)
    : filePath(filePath), processor(processor) {}

std::string NoteManager::addNote(const std::string& rawText) {
    std::string corrected = processor.process(rawText);

    std::ofstream out(filePath, std::ios::app);
    out << corrected << "\n";
    out.close();

    return corrected;
}

std::vector<std::string> NoteManager::readAllLines() const {
    std::vector<std::string> lines;
    std::ifstream in(filePath);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    return lines;
}

void NoteManager::writeAllLines(const std::vector<std::string>& lines) const {
    std::ofstream out(filePath, std::ios::trunc);
    for (const auto& line : lines) {
        out << line << "\n";
    }
}

std::vector<std::string> NoteManager::viewNotes() const {
    return readAllLines();
}

bool NoteManager::deleteNote(int index) {
    std::vector<std::string> lines = readAllLines();
    if (index < 1 || index > static_cast<int>(lines.size())) {
        return false;
    }
    lines.erase(lines.begin() + (index - 1));
    writeAllLines(lines);
    return true;
}
