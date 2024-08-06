#include "Conv3DFilter.h"
#include <cmath>
#include <utility>

Conv3DFilter::Conv3DFilter() = default;

Conv3DFilter::Conv3DFilter(FilterType filterType, int kernel_size, bool write) : filter_type(filterType), kernel_size(kernel_size), write(write) {
    padding = std::floor(kernel_size/2);
    build_kernel(filter_type);
    build_callable();
}

Conv3DFilter::~Conv3DFilter() = default;


std::vector<Matrix<float>> Conv3DFilter::gaussian_filter_3d() {
    float sigma = 2.;
    float sum = 0.0;
    int center = std::floor((kernel_size - 1) / 2);

    kernel_3d.resize(kernel_size);
    for (int z = 0; z < kernel_size; ++z) {
        kernel_3d[z] = Matrix<float>(kernel_size, kernel_size);
        for (int x = 0; x < kernel_size; ++x) {
            for (int y = 0; y < kernel_size; ++y) {
                auto distance = static_cast<float>(std::pow(x - center, 2) + std::pow(y - center, 2) + std::pow(z - center, 2));
                kernel_3d[z](x, y) = std::exp(-distance / (2 * sigma * sigma));
                sum += kernel_3d[z](x, y);
            }
        }
    }

    for (int z = 0; z < kernel_size; ++z) {
        for (int x = 0; x < kernel_size; ++x) {
            for (int y = 0; y < kernel_size; ++y) {
                kernel_3d[z](x, y) /= sum;
            }
        }
    }
    return kernel_3d;
}

std::vector<Matrix<float>> Conv3DFilter::box_filter_3d() const {
    std::vector<Matrix<float>> filter(kernel_size);

    auto value = static_cast<float>(1.0f / pow(kernel_size, 3));


    Matrix<float> layer(kernel_size, kernel_size);
    for (int i = 0; i < kernel_size; ++i) {
        for (int j = 0; j < kernel_size; ++j)
            layer(i, j) = value;
    }
    // Copy this layer across the third dimension
    for (int k = 0; k < kernel_size; ++k) filter[k] = layer;
    return filter;
}

std::vector<float> Conv3DFilter::gaussian_filter_1d() {
    float sigma = 2.0;
    float sum = 0.0;
    int center = std::floor((kernel_size - 1) / 2);
    kernel_1d.reserve(kernel_size);

    for (int i = 0; i < kernel_size; ++i) {
        auto distance = static_cast<float>(std::pow(i - center, 2));
        kernel_1d.emplace_back(std::exp(-distance / (2 * sigma * sigma)));
        sum += kernel_1d[i];
    }

    for (int i = 0; i < kernel_size; ++i)
        kernel_1d[i] /= sum;

    return kernel_1d;
}

std::vector<float> Conv3DFilter::box_filter_1d() {
    kernel_1d.reserve(kernel_size);
    for (int i = 0; i < kernel_size; ++i) {kernel_1d.emplace_back(1.f/static_cast<float>(kernel_size));}
    return kernel_1d;
}

void Conv3DFilter::build_kernel(FilterType type) {
    switch(type) {
        case median:
            kernel_3d = std::vector<Matrix<float>>{};
            break;
        case gaussian:
            kernel_3d = gaussian_filter_3d();
            break;
        case box:
            kernel_3d = box_filter_3d();
            break;
        case box_separated:
            kernel_1d = box_filter_1d();
            break;
        case gaussian_separated:
            kernel_1d = gaussian_filter_1d();
            break;
        default:
            throw std::invalid_argument("Unknown filter.");
    }
}


void Conv3DFilter::build_callable() {
    if (filter_type == median) {
        convolve = [this](std::vector<Matrix<uint8_t>*> batch) -> void {
            return this->convolve_dynamic(std::move(batch));
        };
    }
    else {
        convolve = [this](const std::vector<Matrix<uint8_t> *>& batch) -> void {
            return this->convolve_static(batch);
        };
    }
}

std::vector<Matrix<uint8_t>> Conv3DFilter::pad_zero(
    std::vector<Matrix<uint8_t>*> batch, std::tuple<unsigned int, unsigned int, unsigned int> padding_dim
) {
    if(batch.empty()) return {};

    auto [pad_h, pad_w, pad_d] = padding_dim;
    auto [height, width] = batch[0]->shape();
    unsigned int depth = batch.size();

    std::vector<Matrix<uint8_t>> padded_volume(depth + (2 * pad_d));


    for (auto &layer : padded_volume)
        layer = Matrix<uint8_t>(height + (2 * pad_h), width + (2 * pad_w), 0);


    for (int d = 0; d < depth; ++d) {
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w)
                padded_volume[d + pad_d](h + pad_h, w + pad_w) = (*batch[d])(h, w);
        }
    }
    return padded_volume;
}


void Conv3DFilter::convolve_static_3d(std::vector<Matrix<uint8_t>*> batch) {

    auto padded_batch = pad_zero(batch, {padding, padding, padding});
    auto [h, w] = padded_batch[0].shape();
    unsigned int z = padded_batch.size();
    float pixel_val;

    for (int k = 0; k < z - (padding * 2); ++k) {
        for (int i = 0; i < h - (padding * 2); ++i) {
            for (int j = 0; j < w - (padding * 2); ++j) {
                pixel_val = 0.;
                for (int kk = 0; kk < kernel_size; ++kk) {
                    for (int ki = 0; ki < kernel_size; ++ki) {
                        for (int kj = 0; kj < kernel_size; ++kj) {
                            pixel_val += kernel_3d[kk](ki, kj) * (float)padded_batch[k + kk](i + ki, j + kj);
                        }
                    }
                }
                (*batch[k])(i, j) = static_cast<uint8_t>(pixel_val);
            }
        }
    }

}

void Conv3DFilter::convolve_static_1d(std::vector<Matrix<uint8_t>*> batch, std::tuple<int, int, int> dims) {
    auto [ix, jx, kx] = dims;
    auto padded_batch = pad_zero(batch, {padding * ix, padding * jx, padding * kx});
    auto [h, w] = padded_batch[0].shape();
    unsigned int z = padded_batch.size();
    h -= (padding * 2 * ix), w -= (padding * 2 * jx), z -= (padding * 2 * kx);

    float pixel_val;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int k = 0; k < z; ++k) {
                pixel_val = 0.;
                for (int kk = 0; kk < kernel_size; ++kk) {
                    pixel_val += kernel_1d[kk] * (float)padded_batch[k + (kx*kk)](i+(ix*kk), j+(jx*kk));
                }
                (*batch[k])(i, j) = static_cast<uint8_t>(pixel_val);
            }
        }
    }
}

void Conv3DFilter::convolve_static(const std::vector<Matrix<uint8_t>*>& batch) {

    if (kernel_3d.empty()) {
        convolve_static_1d(batch, {1, 0, 0});
        convolve_static_1d(batch, {0, 1, 0});
        convolve_static_1d(batch, {0, 0, 1});
    }
    else {
        convolve_static_3d(batch);
    }

}

void Conv3DFilter::convolve_dynamic(std::vector<Matrix<uint8_t>*> batch) {
    auto padded_batch = pad_zero(batch, {padding, padding, padding});
    auto [h, w] = padded_batch[0].shape();
    unsigned int z = padded_batch.size();

    std::vector<uint8_t> neighbourhood(kernel_size * kernel_size * kernel_size);
    int idx;
    uint8_t median;

    for (int k = 0; k < z - kernel_size; ++k) {
        for (int j = 0; j < w - kernel_size + 1; ++j) {
            for (int i = 0; i < h - kernel_size; ++i) {
                idx = 0;

                for (int kk = 0; kk < kernel_size; ++kk) {
                    for (int ki = 0; ki < kernel_size; ++ki) {
                        for (int kj = 0; kj < kernel_size; ++kj) {
                            neighbourhood[idx++] = padded_batch[k + kk](i + ki, j + kj);                        }
                    }
                }
                median = compute_median(neighbourhood);
                (*batch[k])(i, j) = median;
            }
        }
    }
}

void Conv3DFilter::apply(Volume &vol) {

    std::cerr << "Applying 3D convolutions to volume...\n";

    auto [h, w, n] = vol.shape();
    int batch_size = vol.batch_size;
    if (vol.load_full_volume) {batch_size = n;}

    for (int i = 0; i < n; i += batch_size) {
        int batch_end = std::min(i + batch_size, n);
        std::cerr << "Processing batch: " << i << " <-> " << batch_end << "\n";
        auto b = vol.get_batch(i, batch_end);
        convolve(b);
        if (write) vol.write_batch(i, batch_end);
        vol.shift();
    }

}



