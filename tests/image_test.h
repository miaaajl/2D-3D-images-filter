#ifndef IMAGE_TEST_H
#define IMAGE_TEST_H

#include "Image.h"

bool check_shape(Image& img, int height, int width, int channels);

bool test_write(Image &img);

bool test_get_data(Image &img);

bool test_set_data(Image &img);

bool test_shape(Image &img);

bool run_image_test();

#endif //IMAGE_TEST_H