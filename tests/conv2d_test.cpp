#include "conv2d_test.h"
#include "Conv2DFilter.h"


template<typename T>
bool matrices_are_equal(Matrix<T> &a, Matrix<T> &b, T tolerance = 1e-5) {
    auto [ha, wa] = a.shape();
    auto [hb, wb] = b.shape();
    if (ha != hb || wa != wb) return false;
    // Compare each element within the specified tolerance
    for (int i = 0; i < ha; ++i) {
        for (int j = 0; j < wa; ++j) {
            if (std::abs(a(i, j) - b(i, j)) > tolerance) return false;
        }
    }
    return true;
}

bool test_box_filter() {
    auto conv_filter = Conv2DFilter(Conv2DFilter::box, 3);
    Matrix<float> expected_kernel = conv_filter.box_filter(); // Assuming you can access this directly or modify as needed

    Matrix<float> manual_kernel(3, 3, 1.f/9.f); // Assuming a constructor that fills all elements with a value
    return matrices_are_equal(expected_kernel, manual_kernel, 1e-6f);
}

bool test_gaussian_filter() {
    Conv2DFilter conv_filter(Conv2DFilter::gaussian, 5, 1);  // kernel_3d size of 5 implies sigma = 1
    Matrix<float> generated_kernel = conv_filter.gaussian_filter();
    // Manually define the expected kernel for a Gaussian filter with sigma calculated based on kernel size
    Matrix<float> expected_kernel(5, 5);
    expected_kernel(0, 0) = 0.00296902; expected_kernel(0, 1) = 0.0133062;
    expected_kernel(0, 2) = 0.0219382; expected_kernel(0, 3) = 0.0133062; expected_kernel(0, 4) = 0.00296902;
    expected_kernel(1, 0) = 0.0133062; expected_kernel(1, 1) = 0.0596343;
    expected_kernel(1, 2) = 0.0983204; expected_kernel(1, 3) = 0.0596343; expected_kernel(1, 4) = 0.0133062;
    expected_kernel(2, 0) = 0.0219382; expected_kernel(2, 1) = 0.0983204;
    expected_kernel(2, 2) = 0.162103; expected_kernel(2, 3) = 0.0983204; expected_kernel(2, 4) = 0.0219382;
    expected_kernel(3, 0) = 0.0133062; expected_kernel(3, 1) = 0.0596343;
    expected_kernel(3, 2) = 0.0983204; expected_kernel(3, 3) = 0.0596343; expected_kernel(3, 4) = 0.0133062;
    expected_kernel(4, 0) = 0.00296902; expected_kernel(4, 1) = 0.0133062;
    expected_kernel(4, 2) = 0.0219382; expected_kernel(4, 3) = 0.0133062; expected_kernel(4, 4) = 0.00296902;

    return matrices_are_equal(generated_kernel, expected_kernel, 1e-5f);
}


bool test_pad_zero() {
    Image img("../tests/images/input.png", 3);
    int padding_size = 1;
    Conv2DFilter filter(Conv2DFilter::box, 3);

    auto original_shape = img.shape();
    auto padded_data = filter.pad_zero(img);
    auto padded_shape = padded_data[0].shape();
    // Calculate the expected dimensions after padding
    int expected_height = std::get<0>(original_shape) + 2 * padding_size;
    int expected_width = std::get<1>(original_shape) + 2 * padding_size;

    // Verify that the padded image dimensions match the expected dimensions
    bool dimensions_correct = std::get<0>(padded_shape) == expected_height && std::get<1>(padded_shape) == expected_width;

    return dimensions_correct;
}


bool test_convolve_static() {
    Matrix<uint8_t> padded_channel({
        {0, 0, 0, 0, 0}, {0, 10, 20, 30, 0}, {0, 40, 50, 60, 0}, {0, 70, 80, 90, 0}, {0, 0, 0, 0, 0}
    });  // simulate padded data here
    Matrix<uint8_t> target_channel({{10, 20, 30}, {40, 50, 60}, {70, 80, 90}});
    Conv2DFilter filter = Conv2DFilter(Conv2DFilter::box, 3);
    target_channel = filter.convolve_static(padded_channel, target_channel);

    uint8_t expected_value = 50;

    // compare central pixel value of the convolved output with the expected value
    return (int)(target_channel(1, 1)) == (int)(expected_value);
}

bool test_convolve_dynamic() {
    Matrix<uint8_t> input_channel({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});  // Create an input matrix filled with zeros
    Matrix<uint8_t> padded_channel({
        {0, 0, 0, 0, 0}, {0, 1, 2, 3, 0}, {0, 4, 5, 6, 0}, {0, 7, 8, 9, 0}, {0, 0, 0, 0, 0}
    });  // Create an input matrix filled with zeros

    auto med = compute_median(input_channel.flatten());

    Conv2DFilter filter = Conv2DFilter(Conv2DFilter::median, 3); // Using a 3x3 kernel_3d
    Matrix<uint8_t> output_channel = filter.convolve_dynamic(padded_channel, input_channel);

    return (output_channel(1, 1) == med);
}


bool run_convolution_test() {
    std::cerr << "Starting unit tests for Conv2DFilter class...\n";
    // Execute each test case and report any failures
    if (!test_box_filter()) {
        std::cerr << "Box filter test failed.\n";
        return false;
    }
    if (!test_pad_zero()) {
        std::cerr << "Padding test failed.\n";
        return false;
    }
    if (!test_convolve_static()) {
        std::cerr << "Static convolve test failed.\n";
        return false;

    }
    if (!test_gaussian_filter()) {
        std::cerr << "Gaussian filter test failed.\n";
        return false;
    }
    if (!test_convolve_dynamic()) {
        std::cerr << "Dynamic convolve test failed.\n";
        return false;
    }
    return true;
}
