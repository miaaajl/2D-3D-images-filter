#ifndef BRIGHTNESS_TEST_H
#define BRIGHTNESS_TEST_H

#include "Image.h"
#include "BrightnessFilter.h"

bool test_brightness_filter(Image &img, int brightnessValue, int expectedValue);

bool run_brightness_test();

#endif //BRIGHTNESS_TEST_H
