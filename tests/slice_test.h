#ifndef SLICE_TEST_H
#define SLICE_TEST_H

#include "Slice.h"
#include "Volume.h"
#include <vector>
#include <cassert>
#include <iostream>

Volume createMockVolume();
bool test_slice_x_direction();
bool test_slice_y_direction();
bool test_slice_z_direction();
bool run_slice_test();

#endif //SLICE_TEST_H