#include "parse.h"
#include <iostream>

namespace parser {
    void Token::ClearToken() {
        name.clear();
        params.clear();
    }

    bool Token::EmptyToken() const {
        return name.empty() && params.empty();
    }

    std::vector<Token> ParseTokens(int argc, char** argv) {
        std::vector<Token> tokens;
        Token token;
        for (size_t i = 1; i < static_cast<size_t>(argc); ++i) {
            std::string str(argv[i]);
            if (i <= 2) {
                token.name = str;
                tokens.push_back(token);
                token.ClearToken();
            } else {
                if (str.front() == '-') {
                    if (!token.EmptyToken()) {
                        tokens.push_back(token);
                        token.ClearToken();
                    }
                    token.name = str;
                } else {
                    token.params.push_back(str);
                }
            }
        }
        if (!token.EmptyToken()) {
            tokens.push_back(token);
        }
        std::cout << tokens.size() << std::endl;
        return tokens;
    }
};  // namespace parser
