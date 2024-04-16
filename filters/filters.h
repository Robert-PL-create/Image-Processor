#ifndef CPP_HSE_FILTER_H
#define CPP_HSE_FILTER_H

#include <memory>
#include <algorithm>
#include <vector>

#include "../image/image.h"
#include "../parsing/parse.h"

namespace filters {
    class Filter {
    public:
        Filter() = default;
        virtual Image Apply(const Image& img) const = 0;
        virtual ~Filter() = default;

    protected:
        template <typename T>
        std::vector<T> GetPixColor(const std::vector<std::vector<T> >& matrix, const Image& image, int x, int y) const;
    };

    class SharpeningFilter : public Filter {
    public:
        Image Apply(const Image& img) const override;
    };

    class Crop : public Filter {
    public:
        Crop(size_t width, size_t height);
        Image Apply(const Image& img) const override;

    private:
        size_t width_;
        size_t height_;
    };

    class EdgeFilter : public Filter {
    public:
        explicit EdgeFilter(double max_color);
        Image Apply(const Image& img) const override;

    private:
        double max_color_;
    };

    class GrayScaleFilter : public Filter {
    public:
        Image Apply(const Image& img) const override;  // NOLINT
    };

    class NegativeFilter : public Filter {
    public:
        Image Apply(const Image& img) const override;  // NOLINT
    };

    std::unique_ptr<Filter> CreateFilter(const parser::Token& token);
}  // namespace filters
#endif  // CPP_HSE_FILTER_H
