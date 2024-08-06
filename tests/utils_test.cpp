#include "utils_test.h"

bool test_median() {
    Matrix<uint8_t> input_channel({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});  // Create an input matrix filled with zeros
    return compute_median(input_channel.flatten()) == 5;
    return true;
}

bool test_convertToHSV(){
    // red RGB(255, 0, 0) should be converted to HSV(0, 1, 1)
    auto [h, s, v] = convertToHSV(255, 0, 0);
    bool test1 = (h == 0 && s == 1.0f && v == 1.0f);

    auto [h2, s2, v2] = convertToHSV(0, 255, 0);
    bool test2 = (h2 == 120 && s2 == 1.0f && v2 == 1.0f);

    return test1 && test2;
}

bool test_convertFromHSV(){
    auto [r, g, b] = convertFromHSV(0, 1.0f, 1.0f);
    bool test1 = (r == 255 && g == 0 && b == 0);

    auto [r2, g2, b2] = convertFromHSV(120, 1.0f, 1.0f);
    bool test2 = (r2 == 0 && g2 == 255 && b2 == 0);

    return test1 && test2;
}

bool test_convertToHSL(){
    auto [h, s, l] = convertToHSL(255, 0, 0);
    bool test1 = (h == 0 && s == 1.0f && l == 0.5f);

    auto [h2, s2, l2] = convertToHSL(0, 255, 0);
    h2 *= 360.0f;
    bool test2 = (h2 == 120 && s2 == 1.0f && l2 == 0.5f);

    return test1 && test2;
}
bool test_convertFromHSL(){
    auto [r, g, b] = convertFromHSL(0, 1.0f, 0.5f);
    bool test1 = (r == 255 && g == 0 && b == 0);

    auto [r2, g2, b2] = convertFromHSL(1.0f/3.0f, 1.0f, 0.5f);
    bool test2 = (r2 == 0 && g2 == 255 && b2 == 0);

    return test1 && test2;
}

int run_utils_test() {
    if (!test_median()) {
        std::cerr << "Median test failed.\n";
        return false;
    }else if(!test_convertToHSV()){
        std::cerr << "convertToHSV test failed.\n";
        return false;
    }else if(!test_convertFromHSV()){
        std::cerr << "convertFromHSV test failed.\n";
        return false;
    }else if(!test_convertToHSL()){
        std::cerr << "convertToHSL test failed.\n";
        return false;
    }else if(!test_convertToHSV()){
        std::cerr << "convertFromHSL test failed.\n";
        return false;
    }
    return true;
};

