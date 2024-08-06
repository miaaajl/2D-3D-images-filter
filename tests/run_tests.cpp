#include "image_test.h"
#include "greyscale_test.h"
#include "conv2d_test.h"
#include "conv3d_test.h"
#include "matrix_test.h"
#include "thresholding_test.h"
#include "noise_test.h"
#include "histogram_equalisation_test.h"
#include "utils_test.h"
#include "brightness_test.h"
#include "projection_test.h"
#include "edge_test.h"
#include "slice_test.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "No test specified" << std::endl;
        return 1;
    }
    std::string type(argv[1]);

    if (type == "matrix") {
        if (!run_matrix_tests()) {return 1;}
    }
    else if (type == "image") {
        if (!run_image_test()) {return 1;}
    }
    else if (type == "greyscale") {
        if (!run_greyscale_test()) {return 1;}
    }
    else if (type == "convolution") {
        if (!run_convolution_test()) {return 1;}
    }
    else if (type == "thresholding") {
        if (!run_thresholding_test()) {return 1;}
    }
    else if (type == "noise") {
        if (!run_noise_test()) {return 1;}
    }
    else if (type == "histogram_equalisation") {
        if (!run_histogram_equalisation_test()) { return 1; }
    }
    else if (type == "brightness") {
        if (!run_brightness_test()) { return 1; }
    }
    else if (type == "utils") {
        if (!run_utils_test()) {return 1;}
    }
    else if (type == "projection") {
        if (!run_projection_test()) {return 1;}
    }
    else if (type == "conv3d") {
        if (!run_conv3d_test()) {return 1;}
    }
    else if (type == "edges") {
        if (!run_edge_test()) {return 1;}
    }
    else if (type == "slice") {
        if (!run_slice_test()) {return 1;}
    }
    else {
        std::cerr << "Unknown test specified" << std::endl;
        return 1;
    }
    return 0;
}