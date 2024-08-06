#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <string>
#include <cstdint>


/**
 * Counting sort algorithm implemented due to speed and constrained values (0-255), reducing memory impact
 */
std::vector<uint8_t> counting_sort(std::vector<uint8_t> input);

int partition(std::vector<std::string>& files, int low, int high);

void insertion_sort(std::vector<std::string> &files);

void quick_sort(std::vector<std::string>& files, int low, int high);


#endif //SORTING_H
