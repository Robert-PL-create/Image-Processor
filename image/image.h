#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#include <vector>
#include <cstddef>
#include <stdexcept>

#include "color.h"

class Image {
public:
    Image() = default;

    Image(size_t width, size_t height);

    Image(const std::vector<std::vector<Color> >& pixels);  // NOLINT

    Image(const Image& other);

    ~Image() = default;

    size_t GetHeight() const;

    size_t GetWidth() const;

    const std::vector<std::vector<Color> >& GetData();

    const Color& GetColor(size_t x, size_t y) const;

    Color& GetColor(size_t x, size_t y);

    void SetColor(size_t x, size_t y, const Color& color);

private:
    void CheckHeightAndWidth(size_t width, size_t height) const;
    std::vector<std::vector<Color> > pixels_;
    size_t width_ = 0;
    size_t height_ = 0;
};

#endif  // CPP_HSE_IMAGE_H
