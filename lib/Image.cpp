#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"
#include "utils/stb_image.h"
#include "utils/stb_image_write.h"
#include <utility>
#include <unordered_map>

Image::Image() = default;

Image::Image(const std::string &path, const int desired_channels) {
    unsigned char* buffer = stbi_load(path.c_str(), &w, &h, &c, desired_channels);
    if (buffer == nullptr) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << "\n";
    }
    else {
        // Update channel count based on desired_channels and construct the image tensor.
        c = desired_channels;
        construct_tensor(buffer);
        stbi_image_free(buffer);
    }
}


Image::~Image() = default;


void Image::construct_tensor(const unsigned char* buffer) {
    data = std::vector<Matrix<uint8_t>>(c, Matrix<uint8_t>(h, w));
    int buffer_idx;
    // Populate the data matrices with pixel values from the buffer.
    for (int k = 0; k < c; ++k) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                buffer_idx = (i * w + j) * c + k;
                data[k](i, j) = buffer[buffer_idx];
            }
        }
    }
}

unsigned char* Image::retrieve_buffer() {
    auto* buffer = new unsigned char[h * w * c];
    int idx;
    // Populate the buffer with pixel values from the data matrices.
    for (int k = 0; k < c; ++k) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                idx = (i * w + j) * c + k;
                buffer[idx] = data[k](i, j);
            }
        }
    }
    return buffer;
}


int Image::write(const std::string &path, const int out_channels) {
    unsigned char* buffer = retrieve_buffer();
    int result =  stbi_write_png(path.c_str(), w, h, out_channels, buffer, 0);
    delete[] buffer;
    return result;
}


std::vector<Matrix<uint8_t>> Image::get_data() {
    return data;
}


void Image::set_data(std::vector<Matrix<uint8_t>> &new_data) {
    data.clear();
    data = std::move(new_data);

}


std::tuple<int, int, int> Image::shape() {
    return std::make_tuple(h, w, c);
}


void Image::set_channels(int channels) {c = channels;}


void Image::set_shape(std::tuple<int, int, int> shape) {
    auto &[height, width, channels] = shape;
    h = height, w = width, c = channels;
}


std::string Image::summary() const {
    return "width: " + std::to_string(w) + ", height: " + std::to_string(h) + ", channels: " + std::to_string(c);
}

