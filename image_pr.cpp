#include <iostream>
#include "parsing/parse.h"
#include "image/image.h"
#include "reading_and_writing/Reader.h"
#include "reading_and_writing/Writer.h"
#include "filters/filters.h"

Image GetImage(const std::string& path) {
    reading_and_writing::Reader reader(path);
    Image image = reader.Read();
    return image;
}

std::vector<parser::Token> ParseTokens(int argc, char** argv) {
    std::vector<parser::Token> tokens = parser::ParseTokens(argc, argv);
    if (tokens.size() < 2) {
        throw std::invalid_argument("Not enough arguments");
    }
    return tokens;
}

Image ApplyFilter(const std::vector<parser::Token>& tokens, Image& image) {
    for (size_t i = 2; i < tokens.size(); ++i) {
        image = filters::CreateFilter(tokens[i])->Apply(image);
    }
    return image;
}

void PrintImage(const std::string& path, const Image& image) {
    reading_and_writing::Writer writer(path);
    writer.Write(image);
}

int main(int argc, char** argv) {
    std::cout << argc;
    try {
        std::vector<parser::Token> tokens = ParseTokens(argc, argv);
        Image image = GetImage(tokens[0].name);
        image = ApplyFilter(tokens, image);
        PrintImage(tokens[1].name, image);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
