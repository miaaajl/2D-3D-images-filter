#include "utils/sorting.h"
#include "utils/auxiliary.h"
#include <vector>
#include <string>

std::vector<uint8_t> counting_sort(std::vector<uint8_t> input) {
    std::size_t end = input.size();
    if (end == 0) return input;
    int range = 256;
    auto frequency_array = new int[range]();

    for (std::size_t i = 0; i < end; i++)
        frequency_array[input[i]]++;

    int array_idx = 0;

    for (int i = 0; i < range; i++) {
        while (frequency_array[i] > 0) {
            input[array_idx++] = static_cast<uint8_t>(i);
            frequency_array[i]--;
        }
    }
    delete[] frequency_array;
    return input;
}



void insertion_sort(std::vector<std::string> &files) {
    for (size_t i = 1; i < files.size(); ++i) {
        std::string key = files[i];
        int keyNumber = extract_number(key);
        int j = i - 1;
        while (j >= 0 && extract_number(files[j]) > keyNumber) {
            files[j + 1] = files[j];
            j = j - 1;
        }
        files[j + 1] = key;
    }
}

int partition(std::vector<std::string>& files, int low, int high) {
    std::string pivot = files[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; ++j) {
        if (extract_number(files[j]) < extract_number(pivot)) {
            i++;
            std::swap(files[i], files[j]);
        }
    }
    std::swap(files[i + 1], files[high]);
    return (i + 1);
}

void quick_sort(std::vector<std::string>& files, int low, int high) {
    if (low < high) {
        int pi = partition(files, low, high);
        quick_sort(files, low, pi - 1);
        quick_sort(files, pi + 1, high);
    }
}


