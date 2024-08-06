#ifndef TEST_H
#define TEST_H

#include "Image.h"
#include "NoiseFilter.h"
#include <iostream>

bool test_noise_filter(Image &img);

bool run_noise_test();

bool test_noise_percentage(Image &img, uint8_t expectedPercentage);

bool test_noise_filter_empty_image();


#endif //TEST_H
