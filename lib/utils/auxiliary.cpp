#include "utils/auxiliary.h"
#include "utils/sorting.h"
#include "utils/stb_image.h"
#include "utils/stb_image_write.h"

#include "Conv2DFilter.h"
#include "Image.h"
#include "HistogramEqualisationFilter.h"
#include "EdgeFilter.h"
#include "Conv3DFilter.h"

#include <fstream>
#include <regex>

uint8_t compute_median(std::vector<uint8_t> v) {

    v = counting_sort(v);

    uint8_t median_value;
    size_t size = v.size();

    if (size % 2 == 0)
        median_value = (v[size / 2 - 1] + v[size / 2]) / 2;
    else
        median_value = v[size / 2];
    return median_value;

}

uint8_t compute_median_3d(Matrix<uint8_t> kernel) {
    std::vector<uint8_t> v = kernel.flatten();

    v = counting_sort(v);

    uint8_t median_value;
    size_t size = v.size();

    if (size % 2 == 0)
        median_value = (v[size / 2 - 1] + v[size / 2]) / 2;
    else
        median_value = v[size / 2];
    return median_value;

}


int file_count(const std::filesystem::directory_iterator &path) {
    int count = 0;
    for (auto& entry : path) {
        if (entry.is_regular_file())
            ++count;
    }
    return count;
}


int extract_number(const std::string& filePath) {

    // regex captures numbers right before ".png"
    static const std::regex num_extractor(R"_(\d+(?=\.png))_");
    std::smatch match;

    if (std::regex_search(filePath, match, num_extractor) && !match.empty()) {
        return std::stoi(match.str(0));
    }
    return 0;
}


long long timeit(const std::function<void()> &func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
}


void image_size_performance(int n_iters) {

    int w = 3 * (pow(2, 4));
    int h = 3 * (pow(2, 4));

    std::ofstream file("../data/image.csv");
    file << "Size,MedianBlur,HistogramEQ,RobertsEdge\n";

    for (int i = 0; i < n_iters; ++i) {
        std::cerr << "iter = " << i << ", img size = " << w * h << "\n";
        Matrix<uint8_t> mat(w, h, 0);
        std::vector<Matrix<uint8_t>> data = {mat};

        Image img = Image("", 0);
        img.set_data(data);
        img.set_shape({h, w, 1});

        auto median_duration = timeit([&] {
            Conv2DFilter(Conv2DFilter::median, 3).apply(img);
        });

        auto he_duration = timeit([&] {
            HistogramEqualisationFilter().apply(img);
        });

        auto edge_duration = timeit([&] {
            EdgeFilter(EdgeFilter::Roberts).apply(img, true);
        });

        file << std::to_string(w * h) << "," << median_duration << "," << he_duration << "," << edge_duration << "\n";
        w *= 2;
        h *= 2;
    }
}

void volume_size_performance(int n_iters) {
    int vol_size = 2;

    std::ofstream file("../data/volume.csv");
    file << "Size,LoadData,Gaussian,GaussianSeperated,Median\n";

    for (int i = 0; i < n_iters; ++i) {
        std::cerr << "iter = " << i << ", vol size = " << vol_size << "\n";

        auto dataload = timeit([&] {
            Volume vol("../Scans/fracture", "../Output/Scans/test_frac/exp", 0, vol_size);
        });

        Volume vol("../Scans/fracture", "../Output/Scans/test_frac/exp", 0, vol_size);

        auto gaussian = timeit([&] {
            Conv3DFilter(Conv3DFilter::gaussian, 3, false).apply(vol);
        });

        auto gaussian_sep = timeit([&] {
            Conv3DFilter(Conv3DFilter::gaussian_separated, 3, false).apply(vol);
        });

        auto median = timeit([&] {
            Conv3DFilter(Conv3DFilter::median, 3, false).apply(vol);
        });

        file << std::to_string(vol_size) << "," << dataload << "," << gaussian << "," << gaussian_sep << "," << median << "\n";

        vol_size *= 2;
    }
}

void kernel_size_performance(int n_iters) {
    int kernel_size = 3;

    std::ofstream file("../data/kernel.csv");
    file << "Size,Gaussian,GaussianSeperated,Median\n";


    for (int i = 0; i < n_iters; ++i) {
        std::cerr << "iter = " << i << ", kernel size = " << kernel_size << "\n";

        Volume vol("../Scans/fracture", "../Output/Scans/test_frac/exp", 0, 13);

        auto gaussian_duration = timeit([&] {
            Conv3DFilter(Conv3DFilter::gaussian, kernel_size, false).apply(vol);
        });

        auto gaussian_sep_duration = timeit([&] {
            Conv3DFilter(Conv3DFilter::gaussian_separated, kernel_size, false).apply(vol);
        });

        auto median_duration = timeit([&] {
            Conv3DFilter(Conv3DFilter::median, kernel_size, false).apply(vol);
        });

        file << kernel_size << "," << gaussian_duration << "," << gaussian_sep_duration << "," << median_duration << "\n";
        kernel_size += 2;
    }
}
