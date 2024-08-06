#include <iostream>
#include "image_test.h"

bool check_shape(Image& img, int height, int width, int channels) {
    auto [h, w, c] = img.shape();
    return w == width && h == height && c == channels;
}

bool test_shape(Image &img) {
    return check_shape(img, 600, 512, 3);
}


bool test_write(Image &img) {
    int result = img.write("../tests/images/output.png", 3);
    return (result > 0);
}

bool test_get_data(Image &img) {
    std::vector<Matrix<uint8_t>> data = img.get_data();
    return (!data.empty());
}

bool test_set_data(Image &img) {
    int test_value = 42;
    std::vector<Matrix<uint8_t>> new_data = std::vector<Matrix<uint8_t>>(
    3, Matrix<uint8_t>(25, 50)
    );

    for (auto& matrix : new_data) {matrix(10, 10) = test_value;}
    img.set_data(new_data);

    std::vector<Matrix<uint8_t>> fetched_data = img.get_data();

    for (auto &mat : fetched_data) {
        if (mat(10, 10) != test_value)
            return false;
    }
    return true;
}

bool run_image_test() {
    std::cerr << "Starting unit tests for Image class...\n";
    Image img = Image("../tests/images/input.png", 3);
    if (!test_shape(img)) {
        std::cerr << "Image shape test failed.\n";
        return false;
    }
    else if (!test_write(img)) {
        std::cerr << "Padding test failed.\n";
        return false;
    }
    else if (!test_get_data(img)) {
        std::cerr << "Fetch data test failed.\n";
        return false;
    }
    else if (!test_set_data(img)) {
        std::cerr << "Set data test failed.\n";
        return false;
    }
    return true;

}