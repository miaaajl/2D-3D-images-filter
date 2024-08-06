#ifndef EDGE_H
#define EDGE_H

#include "EdgeFilter.h"
#include "Image.h"
#include <iostream>
#include <cmath>

Image createTestImage();
bool verifyEdgeDetection(Image& image);
bool run_edge_test();

#endif //EDGE_H