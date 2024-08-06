#include "Projection.h"

Projection::Projection(Method method, int min_depth, int max_depth)
: method(method), min_depth(min_depth), max_depth(max_depth) {
    get_callable();
};

Projection::~Projection() = default;

void Projection::get_callable() {
    switch(method) {
        case maximum:
            execution = [this](Volume &vol) -> Image {
                return this->maximum_projection(vol);
            };
            break;
        case minimum:
            execution = [this](Volume &vol) -> Image {
                return this->minimum_projection(vol);
            };
            break;
        case mean:
            execution = [this](Volume &vol) -> Image {
                return this->mean_projection(vol);
            };
            break;
        case median:
            execution = [this](Volume &vol) -> Image {
                return this->median_projection(vol);
            };
            break;
        default:
            throw std::invalid_argument("Unknown method.");
    }
}

Image Projection::maximum_projection(Volume &vol) {

    auto [h, w, depth] = vol.shape();
    int min_d = std::min(min_depth - 1, depth);
    int max_d = std::max(max_depth - 1, 0); // Adjust for 1-based indexing

    auto &vol_data = vol.data;
    if (vol_data.empty()) return {};

    Image img{};
    img.set_shape(std::make_tuple(h, w, 1));

    std::vector<Matrix<uint8_t>> projection_data(1, Matrix<uint8_t>(h, w));
    uint8_t max_intensity;

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            max_intensity = 0;
            for (int z = min_d; z < max_d; ++z)
                max_intensity = std::max(max_intensity, vol_data[z](j, i));
            projection_data[0](j, i) = max_intensity;
        }
    }
    img.set_data(projection_data);
    return img;
}

Image Projection::minimum_projection(Volume &vol) {

    auto [h, w, depth] = vol.shape();
    int min_d = std::min(min_depth - 1, depth);
    int max_d = std::max(max_depth - 1, 0);

    auto &vol_data = vol.data;
    if (vol_data.empty()) return {};

    Image img{};
    img.set_shape(std::make_tuple(h, w, 1));

    std::vector<Matrix<uint8_t>> projection_data(1, Matrix<uint8_t>(h, w));
    uint8_t min_intensity;

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            min_intensity = 255;
            for (int z = min_d; z < max_d; ++z)
                min_intensity = std::min(min_intensity, vol_data[z](j, i));
            projection_data[0](j, i) = min_intensity;
        }
    }
    img.set_data(projection_data);
    return img;
}

Image Projection::mean_projection(Volume &vol) {

    auto [h, w, depth] = vol.shape();
    int min_d = std::min(min_depth - 1, depth);
    int max_d = std::max(max_depth - 1, 0);

    auto &vol_data = vol.data;
    if (vol_data.empty()) return {};

    Image img{};
    img.set_shape(std::make_tuple(h, w, 1));

    std::vector<Matrix<uint8_t>> projection_data(1, Matrix<uint8_t>(h, w));

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            float sum = 0.;
            for (int z = min_d; z < max_d; ++z) {
                sum += vol_data[z](y, x);
            }
            float avg = sum / max_d;
            projection_data[0](y, x) = static_cast<uint8_t>(avg);
        }
    }
    img.set_data(projection_data);
    return img;
}

Image Projection::median_projection(Volume &vol) {
    auto [h, w, depth] = vol.shape();
    int min_d = std::min(min_depth - 1, depth);
    int max_d = std::max(max_depth - 1, 0);

    std::vector<Matrix<uint8_t>> projection_data(1, Matrix<uint8_t>(h, w));
    std::vector<uint8_t> pixel_vals;

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            pixel_vals.clear();
            for (int z = min_d; z < max_d; ++z)
                pixel_vals.push_back(vol.data[z](y, x));
            projection_data[0](y, x) = compute_median(pixel_vals);
        }
    }

    Image img;
    img.set_data(projection_data);
    img.set_shape(std::make_tuple(h, w, 1));
    return img;

}

Image Projection::execute(Volume& vol) {
    auto [h, w, d] = vol.shape();
    if (max_depth == -1) max_depth = d + 1;
    if (min_depth == -1) min_depth = 1;
    return execution(vol);
}


