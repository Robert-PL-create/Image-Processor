#ifndef CPP_HSE_WRITER_H
#define CPP_HSE_WRITER_H

#include <cstddef>
#include <utility>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../image/image.h"

namespace reading_and_writing {
    class Writer : public Image {
    private:
        std::string path_;
        template <typename T>
        void NumToBytes(T number, unsigned char* bytes);

        void WriteHeader(unsigned char* file_header, size_t file_size);
        void WriteInfoHeader(unsigned char* info_header, size_t width, size_t height);

    public:
        Writer(const std::string& path);  // NOLINT
        void Write(const Image& img);
    };
}  // namespace reading_and_writing
#endif  // CPP_HSE_WRITER_H
