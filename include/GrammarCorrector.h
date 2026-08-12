#ifndef GRAMMAR_CORRECTOR_H
#define GRAMMAR_CORRECTOR_H

#include "TextProcessor.h"
#include <unordered_map>

// Rule-based grammar/spelling cleanup. No AI, no API - just string
// processing. Good enough to catch common mistakes and demonstrate
// the TextProcessor interface in action.
class GrammarCorrector : public TextProcessor {
public:
    GrammarCorrector();

    std::string process(const std::string& text) override;
    std::string name() const override { return "GrammarCorrector (rule-based)"; }

private:
    std::unordered_map<std::string, std::string> replacements;

    std::string fixSpacing(const std::string& text) const;
    std::string capitalizeSentences(std::string text) const;
    std::string applyWordReplacements(const std::string& text) const;
    std::string ensureEndPunctuation(std::string text) const;
};

#endif
