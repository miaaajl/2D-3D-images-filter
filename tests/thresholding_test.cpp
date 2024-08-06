#include "thresholding_test.h"

bool test_thresholding_filter(Image &img1) {
    // Step 1: Create a test image
    int width = 100; // Example width
    int height = 100; // Example height
    std::vector<Matrix<uint8_t>> imageData(3, Matrix<uint8_t>(height, width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Set left half of the image below the threshold, right half above
            uint8_t value = j < width / 2 ? 100 : 200; // Example threshold: 128
            for (int c = 0; c < 3; ++c) { // Assuming RGB channels
                imageData[c](i, j) = value;
            }
        }
    }

    Image img;
    img.set_data(imageData);
    img.set_shape(std::make_tuple(height, width, 3)); // Set as an RGB image

    // Step 2: Apply the ThresholdingFilter
    ThresholdingFilter filter(128); // Set the threshold to 128
    filter.apply(img);

    // Step 3: Verify the resulting image
    bool success = true;
    auto data = img.get_data();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uint8_t expectedValue = j < width / 2 ? 0 : 255;
            for (int c = 0; c < 3; ++c) { // Check each channel
                if (data[c](i, j) != expectedValue) {
                    std::cerr << "Thresholding incorrect at (" << i << ", " << j << ", " << c << ")\n";
                    success = false;
                }
            }
        }
    }

    return success;
}

bool run_thresholding_test() {
    std::cerr << "Starting unit test for ThresholdingFilter class...\n";
    Image img = Image("../tests/images/input.png", 3);
    if (!test_thresholding_filter(img)) {
        std::cerr << "Thresholding filter test failed.\n";
        return false;
    }
    std::cerr << "Thresholding filter test passed.\n";
    return true;
}
