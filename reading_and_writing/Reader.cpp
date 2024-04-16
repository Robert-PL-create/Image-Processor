#include <cerrno>
#include <fstream>
#include <cstring>

#include "Reader.h"
#include "Utils.h"

reading_and_writing::Reader::Reader(const std::string& filename) {
    path_ = filename;
}
Image reading_and_writing::Reader::Read() {
    unsigned char file_header[image::utils::FILE_HEADER_SIZE];
    unsigned char info_header[image::utils::INFO_HEADER_SIZE];
    std::ifstream img_file;
    img_file.open(path_, std::ios::in | std::ios::binary);
    if (!img_file.is_open()) {
        throw std::invalid_argument("Error opening file: " + path_ + " " + std::strerror(errno));
    }
    if (errno == EACCES) {
        throw std::invalid_argument("No permission to read file: " + path_);
    }
    img_file.read(reinterpret_cast<char*>(file_header), image::utils::FILE_HEADER_SIZE);
    if (file_header[0] != image::utils::FORMAT_BYTES[0] || file_header[1] != image::utils::FORMAT_BYTES[1]) {
        throw std::invalid_argument("Invalid file format" + path_);
    }
    try {
        img_file.read(reinterpret_cast<char*>(info_header), image::utils::INFO_HEADER_SIZE);
        size_t width = BytesToNum(info_header + image::utils::HEADER_WIDTH_OFFSET);
        size_t height = BytesToNum(info_header + image::utils::HEADER_HEIGHT_OFFSET);
        Image img(width, height);

        std::vector<std::vector<Color>> img_data(height);
        std::vector<Color> row(width);

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                unsigned char pix[image::utils::PIXEL_DATA_SIZE];
                img_file.read(reinterpret_cast<char*>(pix), image::utils::PIXEL_DATA_SIZE);
                row[j].SetVals(pix[0], pix[1], pix[2]);
            }
            img_data[i] = row;
            img_file.ignore(GetPaddedSize(width));  // NOLINT
        }
        img = Image(img_data);
        img_file.close();
        return img;
    } catch (std::exception& e) {
        throw "Error reading file: " + static_cast<std::string>(e.what());
    }
};

size_t reading_and_writing::Reader::BytesToNum(unsigned char* bytes) {
    size_t num = *bytes;
    for (size_t i = 0; i < image::utils::SHIFT_BYTES.size(); ++i) {
        num += *(bytes + i + 1) << image::utils::SHIFT_BYTES[i];
    }
    return num;
}

size_t reading_and_writing::GetPaddedSize(size_t width) {
    return (image::utils::PADDING_SIZE - (width * image::utils::PIXEL_DATA_SIZE) % image::utils::PADDING_SIZE) %
           image::utils::PADDING_SIZE;
}
