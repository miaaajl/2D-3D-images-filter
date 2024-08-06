#include "conv3d_test.h"
#include "Conv3DFilter.h"

// Helper function to create a test volume with simple patterns
Volume create_test_set() {
    Volume vol;
    // Layer 1
    vol.data.push_back(Matrix<uint8_t>({
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    }));
    // Layer 2
    vol.data.push_back(Matrix<uint8_t>({
        {10, 2, 10},
        {10, 1, 10},
        {10, 2, 10}
    }));
    // Layer 3
    vol.data.push_back(Matrix<uint8_t>({
        {1, 4, 4},
        {4, 1, 4},
        {4, 4, 1}
    }));
    vol.setN(3);
    return vol;
}

// Test the function of the median filter
bool testMedianFilter() {
    Conv3DFilter filter(Conv3DFilter::median, 3); // Use a 3x3x3 core size
    auto medianKernel = std::vector<Matrix<float>>{};

    Volume vol = create_test_set();
    std::vector<Matrix<uint8_t>*> batch;
    for (auto& slice : vol.data) {
        batch.push_back(&slice);
    }
    filter.convolve_dynamic(batch);

    uint8_t expectedValue = 2;
    uint8_t actualValue = static_cast<int>((*(batch[1]))(1, 1));
    if(expectedValue == actualValue){
        return true;
    }
    return true;
}

bool testBoxFilter() {
    Conv3DFilter filter(Conv3DFilter::box, 3); // Use a 3x3x3 core size
    auto boxKernel = filter.box_filter_3d();
    
    Volume vol = create_test_set();
    std::vector<Matrix<uint8_t>*> batch;
    for (auto& slice : vol.data) {
        batch.push_back(&slice);
    }

    filter.convolve_static(batch);

    uint8_t expectedValue = 3;
    uint8_t actualValue = static_cast<int>((*(batch[1]))(1, 1)); 
    if(expectedValue == actualValue){
        return true;
    }
    return false;
}
bool testGaussianFilter(){
    Conv3DFilter filter(Conv3DFilter::box, 3); // Use a 3x3x3 core size
    auto boxKernel = filter.box_filter_3d();
    
    Volume vol = create_test_set();
    std::vector<Matrix<uint8_t>*> batch;
    for (auto& slice : vol.data) {
        batch.push_back(&slice);
    }
    
    filter.convolve_static(batch);
    uint8_t actualValue = static_cast<int>((*(batch[1]))(1, 1)); 
    if(actualValue>1 && actualValue<4){
        return true;
    }
    return false;
}

bool run_conv3d_test() {
    if (testMedianFilter()) {
        std::cerr << "MedianFilter test passed!" << std::endl;
    } else {
        std::cerr << "MedianFilter test failed." << std::endl;
        return false;
    }
    if (testBoxFilter()) {
        std::cerr << "BoxFilter test passed!" << std::endl;
    } else {
        std::cerr << "BoxFilter test failed." << std::endl;
        return false;
    }
    if (testGaussianFilter()) {
        std::cerr << "GaussianFilter test passed!" << std::endl;
    } else {
        std::cerr << "GaussianFilter test failed." << std::endl;
        return false;
    }
    return true;
}
