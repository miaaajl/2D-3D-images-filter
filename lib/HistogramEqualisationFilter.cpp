#include "HistogramEqualisationFilter.h"
#include "Image.h"
#include "utils/transform.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdint>

HistogramEqualisationFilter::HistogramEqualisationFilter() = default;

HistogramEqualisationFilter::~HistogramEqualisationFilter() = default;

void HistogramEqualisationFilter::setColorSpace(const std::string& space) {
        color_space = space;
    }
// Compute image histogram

template<typename T>
std::vector<int> HistogramEqualisationFilter::compute_histogram(const Matrix<T>& channel) {
    std::vector<int> histogram(256, 0);
    auto& mutable_channel = const_cast<Matrix<T>&>(channel);
    auto [rows, cols] = mutable_channel.shape();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if constexpr (std::is_floating_point<T>::value) {
                int index = static_cast<int>(channel(i, j) * 255.0f);
                index = std::max(0, std::min(index, 255));
                histogram[index]++;
            } else {
                histogram[channel(i, j)]++;
            }
        }
    }
    return histogram;
}



std::vector<int> HistogramEqualisationFilter::compute_cumulative_histogram(const std::vector<int>& histogram) {
    std::vector<int> cumulative_histogram(256);
    cumulative_histogram[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cumulative_histogram[i] = cumulative_histogram[i - 1] + histogram[i];
    }
    return cumulative_histogram;
}


template<typename T>
void HistogramEqualisationFilter::apply_histogram_equalization(Matrix<T>& channel, const std::vector<int>& cumulative_histogram) {
    auto& mutable_channel = const_cast<Matrix<T>&>(channel);
    auto [rows, cols] = mutable_channel.shape();
    float norm_factor;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if constexpr (std::is_floating_point<T>::value) {
                float total_pixels = rows * cols;
                float min_cdf = cumulative_histogram[0] / total_pixels;
                norm_factor = 1.0f / (1.0f - min_cdf);

                int pixel_value = static_cast<int>(channel(i, j) * 255.0f);
                float equalized_value = (cumulative_histogram[pixel_value] / total_pixels - min_cdf) * norm_factor;
                channel(i, j) = equalized_value;
            } else {
                norm_factor = 255.0f / (rows * cols - cumulative_histogram[0]);
                int equalized_value = static_cast<int>((cumulative_histogram[channel(i, j)] - cumulative_histogram[0]) * norm_factor);
                channel(i, j) = static_cast<uint8_t>(equalized_value);
            }
        }
    }
}

void HistogramEqualisationFilter::apply(Image &img) {
    auto [h, w, c] = img.shape();

    if (c == 1) {
        // Process grayscale image
        std::vector<Matrix<uint8_t>> data = img.get_data();
        Matrix<uint8_t>& channel = data[0];
    
        auto histogram = compute_histogram(channel);
        auto cumulative_histogram = compute_cumulative_histogram(histogram);
    
        apply_histogram_equalization(channel, cumulative_histogram);
    
        img.set_data(data);
    } else if (c == 3) {
        auto rgb_data = img.get_data();
        std::vector<Matrix<float>> hsv_hsl_data(3, Matrix<float>(h, w));
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                uint8_t r = rgb_data[0](i, j), g = rgb_data[1](i, j), b = rgb_data[2](i, j);

                if (color_space == "HSV") {
                    auto [h, s, v] = convertToHSV(r, g, b);
                    hsv_hsl_data[0](i, j) = h;
                    hsv_hsl_data[1](i, j) = s;
                    hsv_hsl_data[2](i, j) = v;
                } else { // HSL
                    auto [h, s, l] = convertToHSL(r, g, b);
                    hsv_hsl_data[0](i, j) = h;
                    hsv_hsl_data[1](i, j) = s;
                    hsv_hsl_data[2](i, j) = l;
                }
            }
        }

        auto histogram = compute_histogram(hsv_hsl_data[2]);
        auto cumulative_histogram = compute_cumulative_histogram(histogram);
        apply_histogram_equalization(hsv_hsl_data[2], cumulative_histogram);

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (color_space == "HSV") {
                    auto [r, g, b] = convertFromHSV(hsv_hsl_data[0](i, j), hsv_hsl_data[1](i, j), hsv_hsl_data[2](i, j));
                    rgb_data[0](i, j) = r;
                    rgb_data[1](i, j) = g;
                    rgb_data[2](i, j) = b;
                } else { // HSL
                    auto [r, g, b] = convertFromHSL(hsv_hsl_data[0](i, j), hsv_hsl_data[1](i, j), hsv_hsl_data[2](i, j));
                    rgb_data[0](i, j) = r;
                    rgb_data[1](i, j) = g;
                    rgb_data[2](i, j) = b;
                }
            }
        }

        // Update the image data
        img.set_data(rgb_data);
    }
}
