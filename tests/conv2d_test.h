#ifndef CONV2D_TEST_H
#define CONV2D_TEST_H

#include "Conv2DFilter.h"
#include "utils/auxiliary.h"


template<typename T>
bool matrices_are_equal(Matrix<T>& a, Matrix<T>& b);

bool test_box_filter();

bool test_pad_zero();

bool test_convolve_static();

bool test_convolve_dynamic();

bool run_convolution_test();

#endif //CONV2D_TEST_H
