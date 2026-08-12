#include "GrammarCorrector.h"
#include <sstream>
#include <cctype>

GrammarCorrector::GrammarCorrector() {
    // Common typo / informal -> correct mappings.
    // Extend this map as you find more cases worth handling.
    replacements = {
        {"i", "I"},
        {"im", "I'm"},
        {"dont", "don't"},
        {"doesnt", "doesn't"},
        {"cant", "can't"},
        {"wont", "won't"},
        {"teh", "the"},
        {"recieve", "receive"},
        {"seperate", "separate"},
        {"alot", "a lot"},
        {"u", "you"},
        {"ur", "your"}
    };
}

std::string GrammarCorrector::process(const std::string& text) {
    std::string result = fixSpacing(text);
    result = applyWordReplacements(result);
    result = capitalizeSentences(result);
    result = ensureEndPunctuation(result);
    return result;
}

// Collapse multiple spaces into one, trim leading/trailing spaces.
std::string GrammarCorrector::fixSpacing(const std::string& text) const {
    std::string out;
    bool lastWasSpace = false;
    for (char c : text) {
        if (c == ' ') {
            if (!lastWasSpace) out += c;
            lastWasSpace = true;
        } else {
            out += c;
            lastWasSpace = false;
        }
    }
    // trim
    size_t start = out.find_first_not_of(' ');
    size_t end = out.find_last_not_of(' ');
    if (start == std::string::npos) return "";
    return out.substr(start, end - start + 1);
}

// Replace whole words found in the replacements map (case-insensitive match,
// but only on lowercase tokens to keep this simple).
std::string GrammarCorrector::applyWordReplacements(const std::string& text) const {
    std::istringstream iss(text);
    std::string word;
    std::string result;
    bool first = true;

    while (iss >> word) {
        // strip trailing punctuation for lookup, remember it, re-attach after
        std::string trailingPunct;
        while (!word.empty() && std::ispunct(static_cast<unsigned char>(word.back()))) {
            trailingPunct = word.back() + trailingPunct;
            word.pop_back();
        }

        std::string lower = word;
        for (auto& ch : lower) ch = std::tolower(static_cast<unsigned char>(ch));

        auto it = replacements.find(lower);
        std::string finalWord = (it != replacements.end()) ? it->second : word;

        if (!first) result += " ";
        result += finalWord + trailingPunct;
        first = false;
    }
    return result;
}

// Capitalize the first letter of the text and the first letter after
// '.', '!' or '?'.
std::string GrammarCorrector::capitalizeSentences(std::string text) const {
    bool capitalizeNext = true;
    for (size_t i = 0; i < text.size(); ++i) {
        if (capitalizeNext && std::isalpha(static_cast<unsigned char>(text[i]))) {
            text[i] = std::toupper(static_cast<unsigned char>(text[i]));
            capitalizeNext = false;
        } else if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            capitalizeNext = true;
        } else if (!std::isspace(static_cast<unsigned char>(text[i])) && text[i] != '.' &&
                   text[i] != '!' && text[i] != '?') {
            // any other non-space char after punctuation cancels capitalization trigger
            // only if it's not part of the punctuation run itself
        }
    }
    return text;
}

// Add a period at the end if the text doesn't already end with . ! or ?
std::string GrammarCorrector::ensureEndPunctuation(std::string text) const {
    if (text.empty()) return text;
    char last = text.back();
    if (last != '.' && last != '!' && last != '?') {
        text += ".";
    }
    return text;
}
