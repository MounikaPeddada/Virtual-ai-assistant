#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>

// Interface for anything that transforms user text.
// Today the only implementation is GrammarCorrector (rule-based).
// Later you could add an LLMTextProcessor that calls a real API -
// nothing else in the codebase would need to change, because
// NoteManager only ever talks to this interface, never to a
// concrete class. This is the Strategy pattern / open-closed principle.
class TextProcessor {
public:
    virtual ~TextProcessor() = default;
    virtual std::string process(const std::string& text) = 0;
    virtual std::string name() const = 0;
};

#endif
