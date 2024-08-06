cmake_minimum_required(VERSION 3.26)
project(advanced_programming_group_binary_search)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(
        advanced_programming_group_binary_search
        main.cpp
        lib/Image.cpp
        lib/Filter.cpp
        lib/GreyscaleFilter.cpp
	  	lib/HistogramEqualisationFilter.cpp
	  	lib/BrightnessFilter.cpp
        lib/Conv2DFilter.cpp
		lib/ThresholdingFilter.cpp
        lib/NoiseFilter.cpp
        lib/Matrix.cpp
        lib/Volume.cpp
		lib/EdgeFilter.cpp
      	lib/Conv3DFilter.cpp
        lib/Projection.cpp
        lib/Slice.cpp
        lib/utils/sorting.cpp
        lib/utils/auxiliary.cpp
      	lib/utils/transform.cpp
      	lib/UserInterface.cpp
)


add_executable(
        tests
        lib/Image.cpp
        lib/Filter.cpp
        lib/GreyscaleFilter.cpp
      	lib/HistogramEqualisationFilter.cpp
      	lib/BrightnessFilter.cpp
        lib/Conv2DFilter.cpp
	  	lib/ThresholdingFilter.cpp
        lib/Matrix.cpp
		lib/EdgeFilter.cpp
        lib/NoiseFilter.cpp
		lib/Volume.cpp
        lib/Slice.cpp
		lib/Projection.cpp
		lib/Conv3DFilter.cpp
        lib/utils/sorting.cpp
        lib/utils/auxiliary.cpp
      	lib/utils/transform.cpp
        tests/run_tests.cpp
        tests/image_test.cpp
        tests/greyscale_test.cpp
      	tests/histogram_equalisation_test.cpp
      	tests/brightness_test.cpp
        tests/conv2d_test.cpp
        tests/matrix_test.cpp
        tests/utils_test.cpp
        tests/noise_test.cpp
      	tests/thresholding_test.cpp
        tests/conv3d_test.cpp
		tests/projection_test.cpp
        tests/edge_test.cpp
        tests/slice_test.cpp
)

enable_testing()

add_test(NAME matrix_test COMMAND tests "matrix")
add_test(NAME image_test COMMAND tests "image")
add_test(NAME greyscale_test COMMAND tests "greyscale")
add_test(NAME convolution_test COMMAND tests "convolution")
add_test(NAME histogram_equalisation_test COMMAND tests "histogram_equalisation")
add_test(NAME brightness_test COMMAND tests "brightness")
add_test(NAME Thresholding_test COMMAND tests "thresholding")
add_test(NAME utils_test COMMAND tests "utils")
add_test(NAME projection_test COMMAND tests "projection")
add_test(NAME conv3d_test COMMAND tests "conv3d")
add_test(NAME edge_test COMMAND tests "edges")
add_test(NAME slice_test COMMAND tests "slice")
