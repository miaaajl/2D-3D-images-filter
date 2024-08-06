#include "BrightnessFilter.h"
#include "Image.h"

BrightnessFilter::BrightnessFilter(int brightness) : brightnessValue(brightness) {}

BrightnessFilter::~BrightnessFilter() = default;

inline uint8_t clamp_value(int value, int low, int high) {
    if (value < low) return static_cast<uint8_t>(low);
    else if (value > high) return static_cast<uint8_t>(high);
    else return static_cast<uint8_t>(value);
}

void BrightnessFilter::apply(Image &img) {
    auto data = img.get_data();

    for (auto &channel : data) {
        auto [height, width] = channel.shape();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int val = static_cast<int>(channel(i, j)) + brightnessValue;
                channel(i, j) = clamp_value(val, 0, 255);
            }
        }
    }

    img.set_data(data);
}