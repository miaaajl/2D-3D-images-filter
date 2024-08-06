#include "noise_test.h"


bool test_noise_filter_empty_image() {
    Image img;
    NoiseFilter filter;
    try {
        filter.apply(img);
    } catch (...) {
        std::cerr << "Noise filter failed on an empty image.\n";
        return false;
    }
    std::cout << "Noise filter test on an empty image passed.\n";
    return true;
}

bool test_noise_filter(Image &img) {

    auto original_shape = img.shape();
    auto original_data = img.get_data();

    NoiseFilter filter;
    filter.apply(img);

    auto modified_data = img.get_data();
    bool noise_applied = false;

    for (int k = 0; k < std::get<2>(original_shape); ++k) {
        for (int i = 0; i < std::get<0>(original_shape); ++i) {
            for (int j = 0; j < std::get<1>(original_shape); ++j) {
                if (original_data[k](i, j) != modified_data[k](i, j)) {
                    noise_applied = true;
                    break;
                }
            }
            if (noise_applied) break;
        }
        if (noise_applied) break;
    }

    return noise_applied;

}

bool test_noise_percentage(Image &img, uint8_t expectedPercentage) {

    auto [h, w, c] = img.shape();
    auto original_shape = img.shape();

    auto original_data = img.get_data();
    uint32_t pixel_total = h * w;
    uint32_t pixel_changed = 0;

    NoiseFilter filter(expectedPercentage);
    filter.apply(img);

    auto modified_data = img.get_data();
    bool noise_applied = false;

    for (int k = 0; k < std::get<2>(original_shape); ++k) {
        for (int i = 0; i < std::get<0>(original_shape); ++i) {
            for (int j = 0; j < std::get<1>(original_shape); ++j) {
                if (original_data[k](i, j) != modified_data[k](i, j)) {
                    pixel_changed++;
                }
            }
        }
    }

    double noisePercentage = (double)expectedPercentage / 100;

    double actualPercentage = (double)pixel_changed / pixel_total * 100.0;
    // Assuming a tolerance for the noise percentage verification
    const double tolerance = 1.0;
    bool isWithinTolerance = actualPercentage >= (noisePercentage - tolerance) &&
                             actualPercentage <= (noisePercentage + tolerance);

    return isWithinTolerance;

}

bool run_noise_test() {
    std::cerr << "Starting unit test for NoiseFilter class...\n";
    Image img = Image("../tests/images/input.png", 3);
    std::string imagePath = "../tests/images/input.png";
    Image originalImage(imagePath, 3); // Load original image
    Image noisyImage = originalImage; // Make a copy of the original image to apply noise

    if (!test_noise_filter_empty_image()) {
        std::cerr << "Test on an empty image failed.\n";
    } else {
        std::cout << "Test on an empty image passed.\n";
    }

    if (!test_noise_filter(img)) {
        std::cerr << "Noise filter test failed.\n";
        return false;
    }
    if (!test_noise_percentage(img, 20)) {
        std::cerr << "Noise filter test failed.\n";
        return false;
    }
    std::cerr << "Noise filter test passed.\n";
    return true;
}
