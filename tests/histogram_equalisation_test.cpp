#include "histogram_equalisation_test.h"
#include <iostream>
#include <tuple> 
#include <string>


// Test the histogram equalization filter
bool test_histogram_equalisation_filter(Image &img, std::string type) {

    HistogramEqualisationFilter filter;

    auto shape = img.shape();
    int channels = std::get<2>(shape); // Get the number of channels
    // Save the processed image
    std::string processedImagePath;
    if(channels == 3){
        if(type == "HSV"){
            processedImagePath = "../tests/images/vh_anatomy_HSV.png";
        }else{
            filter.setColorSpace("HSL");
            processedImagePath = "../tests/images/vh_anatomy_HSL.png";
        }

    }else{
        processedImagePath = "../tests/images/vh_grey.png";
    }
    // Apply the histogram equalization filter
    filter.apply(img);
    img.write(processedImagePath, channels);
    std::cerr << "Processed image saved successfully\n";
    return true;
}

bool run_histogram_equalisation_test() {
    std::cerr << "Starting unit test for HistogramEqualisationFilter class...\n";
    Image img_rgb("../tests/images/input.png", 3); // Make sure this path is correct
    Image img_grey("../tests/images/input_grey.png", 1); // Make sure this path is correct
    bool rgb_h = test_histogram_equalisation_filter(img_rgb,"HSV");
    bool rgb_v = test_histogram_equalisation_filter(img_rgb,"HSL");
    bool grey = test_histogram_equalisation_filter(img_grey,"GREY");
    if (!rgb_h && !rgb_v && !grey) {
        std::cerr << "Histogram equalisation filter test failed.\n";
        return false;
    }
    std::cerr << "Histogram equalisation filter test passed.\n";
    return true;
}
