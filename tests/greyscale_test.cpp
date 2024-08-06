#include "greyscale_test.h"


bool test_greyscale_filter(Image &img) {
    GreyscaleFilter filter;
    filter.apply(img);
    auto [h, w, c] = img.shape();
    return (c == 1);
}


bool run_greyscale_test() {
    std::cerr << "Starting unit test for GreyscaleFilter class...\n";
    Image img = Image("../tests/images/input.png", 3);
    if (!test_greyscale_filter(img)) {
        std::cerr << "Greyscale filter test failed.\n";
        return false;
    }
    std::cerr << "Greyscale filter test passed.\n";
    return true;
}

