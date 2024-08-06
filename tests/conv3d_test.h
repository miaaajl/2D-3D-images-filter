#ifndef CONV3D_TEST_H
#define CONV3D_TEST_H

#include "Conv3DFilter.h"
#include "Volume.h"
#include <vector>
#include <cassert>
#include <iostream>

Volume create_test_set();
bool testMedianFilter();
bool testBoxFilter();
bool testGaussianFilter();
bool run_conv3d_test();

#endif //CONV3D_TEST_H