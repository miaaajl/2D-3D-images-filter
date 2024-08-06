#include "Conv2DFilter.h"
#include "utils/auxiliary.h"
#include <stdexcept>
#include <cmath>


Conv2DFilter::Conv2DFilter() = default;

Conv2DFilter::Conv2DFilter(
    FilterType filter_type, int kernel_size, float sigma)
    : filter_type(filter_type), kernel_size(kernel_size), sigma(sigma) {
    padding = std::floor(kernel_size/2);
    build_kernel(filter_type);
}

Conv2DFilter::~Conv2DFilter() = default;

void Conv2DFilter::build_kernel(FilterType type) {
    switch(type) {
        case median:
            kernel = Matrix<float>(kernel_size, kernel_size);
            break;
        case box:
            kernel = box_filter();
            break;
        case gaussian:
            kernel = gaussian_filter();
            break;
        default:
            throw std::invalid_argument("Unknown filter.");
    }
}


Matrix<float> Conv2DFilter::box_filter() const {
    return {kernel_size, kernel_size, 1.f / (float)(kernel_size * kernel_size)};
}


Matrix<float> Conv2DFilter::gaussian_filter() const {
    Matrix<float> specific_kernel(kernel_size, kernel_size);
    float sum = 0.0;

    int center_val = std::floor((kernel_size - 1) / 2);

    for (int i = 0; i < kernel_size; ++i) {
        for (int j = 0; j < kernel_size; ++j) {
            int x = i - center_val;
            int y = j - center_val;
            specific_kernel(i, j) = (float)(1 / (2*M_PI * sigma*sigma)) * std::exp(-(float)(x*x + y*y) / (2 * sigma * sigma));
            sum += specific_kernel(i, j);
        }
    }

    // normalise the kernel_3d now
    for (int i = 0; i < kernel_size; ++i) {
        for (int j = 0; j < kernel_size; ++j) {
            specific_kernel(i, j) /= sum;
        }
    }

    return specific_kernel;
}

std::vector<Matrix<uint8_t>> Conv2DFilter::pad_zero(Image &img) const {

    auto [h, w, c] = img.shape();
    std::vector<Matrix<uint8_t>> data = img.get_data();
    std::vector<Matrix<uint8_t>> padded_data = std::vector<Matrix<uint8_t>>(
    c, Matrix<uint8_t>(h + (2 * padding), w + (2 * padding))
    );

    for (int k = 0; k < c; ++k) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                padded_data[k](i + padding, j + padding) = data[k](i, j);
            }
        }
    }
    return padded_data;
}



Matrix<uint8_t> Conv2DFilter::convolve_static(Matrix<uint8_t> &padded_channel, Matrix<uint8_t> &target_channel) {
    float pixel_val;
    auto [h, w] = padded_channel.shape();

    for (int i = 0; i < h - (padding * 2); ++i) {
        for (int j = 0; j < w - (padding * 2); ++j) {
            pixel_val = 0.;
            for (int ki = 0; ki < kernel_size; ++ki) {
                for (int kj = 0; kj < kernel_size; ++kj) {
                    pixel_val += kernel(ki, kj) * (float)padded_channel(i + ki, j + kj);
                }
            }
            target_channel(i, j) = static_cast<uint8_t>(pixel_val);
        }
    }

    return target_channel;
}


Matrix<uint8_t> Conv2DFilter::convolve_dynamic(Matrix<uint8_t> &padded_channel, Matrix<uint8_t> &target_channel) const {

    auto [h, w] = padded_channel.shape();
    std::vector<uint8_t> neighbourhood(kernel_size * kernel_size);
    int idx;
    uint8_t median;

    for (int i = 0; i < h - (padding * 2); ++i) {
        for (int j = 0; j < w - (padding * 2); ++j) {
            idx = 0;
            for (int ki = 0; ki < kernel_size; ++ki) {
                for (int kj = 0; kj < kernel_size; ++kj)
                    neighbourhood[idx++] = padded_channel(i + ki, j + kj);
            }
            median = compute_median(neighbourhood);
            target_channel(i, j) = median;
        }
    }
    return target_channel;
}

void Conv2DFilter::set_kernel(Matrix<float> new_kernel) {
    kernel = new_kernel;
    kernel_size = std::get<0>(new_kernel.shape());
}


void Conv2DFilter::apply(Image &img) {

    // create lambda based on kernel_type to see if use convolve_static or convolve_dynamic
    auto convolve = [this](Matrix<uint8_t> p, Matrix<uint8_t> d) -> Matrix<uint8_t> {
        return this->filter_type == median ? this->convolve_dynamic(p, d) : this->convolve_static(p, d);
    };

    auto [h, w, c] = img.shape();

    std::vector<Matrix<uint8_t>> padded_data = pad_zero(img);
    std::vector<Matrix<uint8_t>> data = img.get_data();

    for (int k = 0; k < c; ++k)
        data[k] = convolve(padded_data[k], data[k]);

    img.set_data(data);
}
