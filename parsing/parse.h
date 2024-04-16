#pragma once

#include <string>
#include <vector>

namespace parser {
    struct Token {
        std::string name;
        std::vector<std::string> params;
        void ClearToken();
        bool EmptyToken() const;
    };

    std::vector<Token> ParseTokens(int argc, char** argv);
};  // namespace parser
