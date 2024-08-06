#include "GreyscaleFilter.h"
#include "Image.h"

GreyscaleFilter::GreyscaleFilter() = default;

GreyscaleFilter::~GreyscaleFilter() = default;

void GreyscaleFilter::apply(Image &img) {
    auto [h, w, c] = img.shape();

    if (c < 3) {
        std::cerr << "Image does not have 3 channels. Greyscale conversion skipped.\n";
        return;
    }

    std::vector<Matrix<uint8_t>> greyscale_data = std::vector<Matrix<uint8_t>>(
            1, Matrix<uint8_t>(h, w)
    );

    std::vector<Matrix<uint8_t>> data = img.get_data();

    int counter = 0;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            counter++;
            int r = data[0](i, j);
            int g = data[1](i, j);
            int b = data[2](i, j);
            int val = static_cast<uint8_t>(0.2126 * r + 0.7152 * g + 0.0722 * b);
            greyscale_data[0](i, j) = val;
        }
    }

    img.set_data(greyscale_data);
    img.set_shape(std::make_tuple(h, w, 1));
}

