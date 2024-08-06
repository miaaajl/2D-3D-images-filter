#ifndef AUXILIARY_H
#define AUXILIARY_H

#include "Matrix.h"
#include <filesystem>
#include <functional>
#include <cmath>


uint8_t compute_median(std::vector<uint8_t> kernel);

uint8_t compute_median_3d(Matrix<uint8_t> kernel);

int file_count(const std::filesystem::directory_iterator& path);

int extract_number(const std::string& filePath);

long long timeit(const std::function<void()> &func);

void image_size_performance(int n_iters);

void volume_size_performance(int n_iters);

void kernel_size_performance(int n_iters);


#endif //AUXILIARY_H
