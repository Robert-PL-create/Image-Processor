#include "filters.h"
#include <cmath>

namespace filters {

    template <typename T>
    std::vector<T> filters::Filter::GetPixColor(const std::vector<std::vector<T>>& matrix, const Image& image, int x,
                                                int y) const {
        T blue = 0;
        T green = 0;
        T red = 0;
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                red += image
                               .GetColor(std::clamp(x - 1 + i, static_cast<size_t>(0), image.GetWidth() - 1),
                                         std::clamp(y - 1 + j, static_cast<size_t>(0), image.GetHeight() - 1))
                               .red *
                       matrix[i][j];
                green += image
                                 .GetColor(std::clamp(x - 1 + i, static_cast<size_t>(0), image.GetWidth() - 1),
                                           std::clamp(y - 1 + j, static_cast<size_t>(0), image.GetHeight() - 1))
                                 .green *
                         matrix[i][j];
                blue += image
                                .GetColor(std::clamp(x - 1 + i, static_cast<size_t>(0), image.GetWidth() - 1),
                                          std::clamp(y - 1 + j, static_cast<size_t>(0), image.GetHeight() - 1))
                                .blue *
                        matrix[i][j];
            }
        }
        return {blue, green, red};
    }

    EdgeFilter::EdgeFilter(double max_color) {
        max_color_ = max_color;
    }
    Image EdgeFilter::Apply(const Image& img) const {
        std::vector<std::vector<Color>> new_data;
        std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};  // NOLINT
        for (size_t i = 0; i < img.GetHeight(); ++i) {
            std::vector<Color> row(img.GetWidth());
            for (size_t j = 0; j < img.GetWidth(); ++j) {
                std::vector<int> colors = GetPixColor(matrix, img, j, i);  // NOLINT
                if (colors[2] >= max_color_) {
                    row[j].blue = 255;   // NOLINT
                    row[j].green = 255;  // NOLINT
                    row[j].red = 255;    // NOLINT
                } else {
                    row[j].blue = 0;   // NOLINT
                    row[j].green = 0;  // NOLINT
                    row[j].red = 0;
                }
            }
            new_data.push_back(row);
        }
        return {new_data};
    }
    uint8_t GetCorrectColors(int color, const uint8_t max_color = 255) {
        if (color <= 0) {
            return 0;
        } else if (color > max_color) {
            return max_color;
        }
        return color;
    }
    Image GrayScaleFilter::Apply(const Image& img) const {
        std::vector<std::vector<Color>> new_data;
        for (size_t i = 0; i < img.GetHeight(); ++i) {
            std::vector<Color> row(img.GetWidth());
            for (size_t j = 0; j < img.GetWidth(); ++j) {
                row[j].blue = static_cast<uint8_t>(img.GetColor(j, i).blue * 0.114        // NOLINT
                                                   + img.GetColor(j, i).green * 0.587 +   // NOLINT
                                                   img.GetColor(j, i).red * 0.299);       // NOLINT
                row[j].green = static_cast<uint8_t>(img.GetColor(j, i).blue * 0.114       // NOLINT
                                                    + img.GetColor(j, i).green * 0.587 +  // NOLINT
                                                    img.GetColor(j, i).red * 0.299);      // NOLINT
                row[j].red = static_cast<uint8_t>(img.GetColor(j, i).blue * 0.114         // NOLINT
                                                  + img.GetColor(j, i).green * 0.587 +    // NOLINT
                                                  img.GetColor(j, i).red * 0.299);        // NOLINT
            }
            new_data.push_back(row);
        }
        return {new_data};
    }
    Image NegativeFilter::Apply(const Image& img) const {
        std::vector<std::vector<Color>> new_data;
        for (size_t i = 0; i < img.GetHeight(); ++i) {
            std::vector<Color> row(img.GetWidth());
            for (size_t j = 0; j < img.GetWidth(); ++j) {
                row[j].blue = static_cast<uint8_t>(255 - img.GetColor(j, i).blue);    // NOLINT
                row[j].green = static_cast<uint8_t>(255 - img.GetColor(j, i).green);  // NOLINT
                row[j].red = static_cast<uint8_t>(255 - img.GetColor(j, i).red);      // NOLINT
            }
            new_data.push_back(row);
        }
        return {new_data};
    }
    Image filters::SharpeningFilter::Apply(const Image& img) const {
        std::vector<std::vector<Color>> new_data;
        std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};  // NOLINT
        for (size_t i = 0; i < img.GetHeight(); ++i) {
            std::vector<Color> row(img.GetWidth());
            for (size_t j = 0; j < img.GetWidth(); ++j) {
                std::vector<int> colors = GetPixColor(matrix, img, j, i);  // NOLINT
                row[j].blue = GetCorrectColors(colors[0]);
                row[j].green = GetCorrectColors(colors[1]);
                row[j].red = GetCorrectColors(colors[2]);
            }
            new_data.push_back(row);
        }
        return {new_data};
    }
    Crop::Crop(size_t width, size_t height) {
        width_ = width;
        height_ = height;
    }
    Image Crop::Apply(const Image& image) const {
        Image new_img(std::min(width_, image.GetWidth()), std::min(height_, image.GetHeight()));
        for (size_t i = image.GetHeight() - std::min(height_, image.GetHeight()); i < image.GetHeight(); ++i) {
            for (size_t j = 0; j < std::min(width_, image.GetWidth()); ++j) {
                new_img.SetColor(j, i - (image.GetHeight() - std::min(height_, image.GetHeight())), image.GetColor(j, i));
            }
        }
        return new_img;
    }
}  // namespace filters
std::unique_ptr<filters::Filter> filters::CreateFilter(const parser::Token& token) {
    const ::std::string& name = token.name;
    if (name == "-sharp") {
        if (!token.params.empty()) {
            throw std::invalid_argument("Too many parameters for sharp filter");
        }
        auto ptr = std::make_unique<SharpeningFilter>();
        return ptr;
    } else if (name == "-gs") {
        if (!token.params.empty()) {
            throw std::invalid_argument("Too many parameters for gs filter");
        }
        auto ptr = std::make_unique<GrayScaleFilter>();
        return ptr;
    } else if (name == "-neg") {
        if (!token.params.empty()) {
            throw std::invalid_argument("Too many parameters for neg filter");
        }
        auto ptr = std::make_unique<NegativeFilter>();
        return ptr;
    } else if (name == "-crop") {
        if (token.params.size() != 2) {
            throw std::invalid_argument("Too many or too few parameters for crop filter");
        }
        try {
            size_t width = std::stoul(token.params[0]);
            size_t height = std::stoul(token.params[1]);
            auto ptr = std::make_unique<Crop>(width, height);
            return ptr;
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Invalid argument");
        }

    } else if (name == "-edge") {
        if (token.params.size() != 1) {
            throw std::invalid_argument("Too many parameters for neg filter");
        }
        double max_color = std::stod(token.params[0]);
        auto ptr = std::make_unique<EdgeFilter>(255 * max_color);  // NOLINT
        return ptr;
    } else {
        throw std::invalid_argument("Too many or too few parameters for edge filter");
    }
}
