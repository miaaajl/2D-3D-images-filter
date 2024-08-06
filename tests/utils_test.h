#ifndef UTILS_H
#define UTILS_H

#include "Matrix.h"
#include "utils/auxiliary.h"
#include "utils/transform.h"
#include <cstdint>
#include <iostream>

bool test_median();

bool test_convertToHSV();
bool test_convertFromHSV();
bool test_convertToHSL();
bool test_convertFromHSL();

int run_utils_test();

#endif //UTILS_H
