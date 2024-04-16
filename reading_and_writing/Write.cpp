#include "Writer.h"
#include "Utils.h"
#include "Reader.h"

namespace reading_and_writing {
    Writer::Writer(const std::string& path) : path_(std::move(path)) {
    }
    template <typename T>
    void Writer::NumToBytes(T number, unsigned char* bytes) {
        *bytes = number;
        for (size_t i = 0; i < image::utils::SHIFT_BYTES.size(); ++i) {
            *(bytes + i + 1) = (number >> image::utils::SHIFT_BYTES[i]);
        }
    };
    void Writer::WriteHeader(unsigned char* file_header, size_t file_size) {
        file_header[image::utils::FORMAT_FILE_FIRST_POSITION] = image::utils::FORMAT_BYTES[0];
        file_header[image::utils::FORMAT_FILE_SECOND_POSITION] = image::utils::FORMAT_BYTES[1];
        NumToBytes(file_size, file_header + image::utils::FILE_SIZE_POSITION);
        file_header[image::utils::PIX_POSITION] = image::utils::FILE_HEADER_SIZE + image::utils::INFO_HEADER_SIZE;
    };

    void Writer::WriteInfoHeader(unsigned char* info_header, size_t width, size_t height) {
        info_header[image::utils::INFO_HEADER_SIZE_POSITION] = image::utils::INFO_HEADER_SIZE;
        NumToBytes(width, info_header + image::utils::HEADER_WIDTH_OFFSET);
        NumToBytes(height, info_header + image::utils::HEADER_HEIGHT_OFFSET);
        info_header[image::utils::CNT_OF_PLANE_POSITION] = image::utils::CNT_OF_PLANE;
        info_header[image::utils::CNT_OF_BITS_POSITION] = image::utils::CNT_OF_BITS;
    };
    void Writer::Write(const Image& img) {
        std::ofstream out_img;
        out_img.open(path_, std::ios::out | std::ios::binary);
        if (!out_img.is_open()) {
            throw std::invalid_argument("Error opening file: " + path_);
        }
        if (errno == EACCES) {
            throw std::invalid_argument("No permission to write file: " + path_);
        }
        const size_t file_size = image::utils::FILE_HEADER_SIZE + image::utils::INFO_HEADER_SIZE +
                                 img.GetWidth() * img.GetHeight() * image::utils::PIXEL_DATA_SIZE +
                                 GetPaddedSize(img.GetWidth()) * GetPaddedSize(img.GetHeight());
        unsigned char file_header[image::utils::FILE_HEADER_SIZE] = {};
        std::fill(file_header, file_header + image::utils::FILE_HEADER_SIZE, 0);
        WriteHeader(file_header, file_size);

        unsigned char info_header[image::utils::INFO_HEADER_SIZE] = {};
        std::fill(info_header, info_header + image::utils::INFO_HEADER_SIZE, 0);
        WriteInfoHeader(info_header, img.GetWidth(), img.GetHeight());

        out_img.write(reinterpret_cast<char*>(file_header), image::utils::FILE_HEADER_SIZE);
        out_img.write(reinterpret_cast<char*>(info_header), image::utils::INFO_HEADER_SIZE);

        unsigned char empty_pix[image::utils::PIXEL_DATA_SIZE] = {0, 0, 0};
        for (size_t y = 0; y < img.GetHeight(); ++y) {
            for (size_t x = 0; x < img.GetWidth(); ++x) {
                unsigned char blue = static_cast<unsigned char>(img.GetColor(x, y).blue);
                unsigned char green = static_cast<unsigned char>(img.GetColor(x, y).green);
                unsigned char red = static_cast<unsigned char>(img.GetColor(x, y).red);
                unsigned char pix[] = {blue, green, red};
                out_img.write(reinterpret_cast<char*>(pix), image::utils::PIXEL_DATA_SIZE);
            }
            out_img.write(reinterpret_cast<char*>(empty_pix), GetPaddedSize(img.GetWidth()));  // NOLINT
        }
        out_img.close();
    };
}  // namespace reading_and_writing
