#include "EdgeFilter.h"
#include "GreyscaleFilter.h"


EdgeFilter::EdgeFilter(EdgeType type) : edgeType(type) {}

EdgeFilter::~EdgeFilter() = default;

void EdgeFilter::preprocessImage(Image &img, bool apply_blur) {
    GreyscaleFilter greyFilter;
    greyFilter.apply(img);

    // Optionally, apply a box filter for blurring
    if (apply_blur) {
        Conv2DFilter gaussianFilter(Conv2DFilter::box, 3);
        gaussianFilter.apply(img);
    }
}

void EdgeFilter::apply(Image &img, bool apply_blur) {
    preprocessImage(img, apply_blur);

    // Select and build the kernels for x and y directions based on the specified edge detection type.
    Matrix<float> kernelX, kernelY;
    switch (edgeType) {
        case Sobel:
            kernelX = buildSobel(true);
            kernelY = buildSobel(false);
            break;
        case Prewitt:
            kernelX = buildPrewitt(true);
            kernelY = buildPrewitt(false);
            break;
        case Scharr:
            kernelX = buildScharr(true);
            kernelY = buildScharr(false);
            break;
        case Roberts:
            kernelX = buildRobertsCross(true);
            kernelY = buildRobertsCross(false);
            break;
        default:
            throw std::invalid_argument("Unknown edge detection type.");
    }

    Matrix<uint8_t> imgData = img.get_data()[0];
    std::vector<Matrix<uint8_t>> padded_data = pad_zero(img);
    auto [h, w] = imgData.shape();
    Matrix<uint8_t> convolvedX(h, w), convolvedY(h, w);

    // Apply the convolution with the x and y kernels.
    set_kernel(kernelX);
    convolve_static(padded_data[0], convolvedX);
    set_kernel(kernelY);
    convolve_static(padded_data[0], convolvedY);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            float gradX = static_cast<float>(convolvedX(i, j));
            float gradY = static_cast<float>(convolvedY(i, j));
            float magnitude = std::sqrt(gradX * gradX + gradY * gradY);
            imgData(i, j) = static_cast<uint8_t>(std::min(magnitude, 255.0f));
        }
    }

    std::vector<Matrix<uint8_t>> newData{imgData};
    img.set_data(newData);
}


Matrix<float> EdgeFilter::buildSobel(bool is_in_X) {
    if (is_in_X) {
        return Matrix<float>({ {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} });
    } else {
        return Matrix<float>({ {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} });
    }
}

Matrix<float> EdgeFilter::buildPrewitt(bool is_in_X) {
    if (is_in_X) {
        return Matrix<float>({ {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} });
    } else {
        return Matrix<float>({ {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} });
    }
}

Matrix<float> EdgeFilter::buildScharr(bool is_in_X) {
    if (is_in_X) {
        return Matrix<float>({ {-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3} });
    } else {
        return Matrix<float>({ {-3, -10, -3}, {0, 0, 0}, {3, 10, 3} });
    }
}

Matrix<float> EdgeFilter::buildRobertsCross(bool is_in_X) {
    if (is_in_X) {
        return Matrix<float>({ {1, 0}, {0, -1} });
    } else {
        return Matrix<float>({ {0, 1}, {-1, 0} });
    }
}
