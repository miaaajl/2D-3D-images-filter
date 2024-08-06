#ifndef HISTOGRAM_EQUALISATION_TEST_H
#define HISTOGRAM_EQUALISATION_TEST_H

#include "Image.h"
#include "HistogramEqualisationFilter.h"
#include <iostream>
#include <string>

bool test_histogram_equalisation_filter(Image &img, std::string type);

bool run_histogram_equalisation_test();

#endif // HISTOGRAM_EQUALISATION_TEST_H
