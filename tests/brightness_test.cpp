#include "brightness_test.h"
#include <cassert>


bool test_brightness_filter(Image &img, int brightnessValue, int expectedValue) {
    BrightnessFilter filter(brightnessValue);
    filter.apply(img);

    auto data = img.get_data();
    for (auto &channel : data) {
        auto [height, width] = channel.shape();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (channel(i, j) != expectedValue) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool run_brightness_test() {
    std::cerr << "Starting unit test for BrightnessFilter class...\n";

    Image img("../tests/images/white.png", 3);

    if (!test_brightness_filter(img, 10, 255)) {  // Assuming clamping at 255
        std::cerr << "Increase brightness test failed.\n";
        return false;
    }

    if (!test_brightness_filter(img, -10, 245)) {
        std::cerr << "Decrease brightness test failed.\n";
        return false;
    }

    std::cerr << "All brightness filter tests passed.\n";
    return true;
}