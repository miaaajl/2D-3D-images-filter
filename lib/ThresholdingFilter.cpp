#include "ThresholdingFilter.h"

#include "utils/transform.h"
#include "Image.h"


ThresholdingFilter::ThresholdingFilter() {
    this->setThreshold(128);
}


ThresholdingFilter::ThresholdingFilter(uint8_t threshold){
    this->setThreshold(threshold);
}


ThresholdingFilter::~ThresholdingFilter() = default;


void ThresholdingFilter::apply(Image &img) {
    auto [h, w, c] = img.shape();
    if (c != 3) {
        std::cerr << "Unsupported number of channels for HSL thresholding. Conversion skipped.\n";
        return;
    }

    auto data = img.get_data();

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            auto [r, g, b] = std::make_tuple(data[0](i, j), data[1](i, j), data[2](i, j));
            auto [hue, saturation, lightness] = convertToHSL(r, g, b);

            float thresholdScaled = threshold / 255.0f;
            float newLightness = (lightness > thresholdScaled) ? 1.0f : 0.0f;
            auto [newR, newG, newB] = convertFromHSL(hue, saturation, newLightness);

            data[0](i, j) = newR;
            data[1](i, j) = newG;
            data[2](i, j) = newB;
        }
    }

    img.set_data(data);
}


uint8_t ThresholdingFilter::getThreshold() const {
    return threshold;
}


void ThresholdingFilter::setThreshold(uint8_t threshold) {
    ThresholdingFilter::threshold = threshold;
}

