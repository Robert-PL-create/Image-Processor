#ifndef CPP_HSE_READER_H
#define CPP_HSE_READER_H

#include <string>

#include "../image/image.h"

namespace reading_and_writing {
    class Reader {
    private:
        std::string path_;
        size_t BytesToNum(unsigned char* bytes);

    public:
        Reader(const std::string& path);  // NOLINT
        Image Read();
    };
    size_t GetPaddedSize(size_t width);
};  // namespace reading_and_writing

#endif  // CPP_HSE_READER_H
