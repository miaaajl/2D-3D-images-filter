#include "NoiseFilter.h"
#include "Image.h"
#include <chrono>


NoiseFilter::NoiseFilter(){
    this->setNoisePercentage(30);
}


NoiseFilter::NoiseFilter(uint8_t _noisePercentage) : noisePercentage(_noisePercentage){
}


NoiseFilter::~NoiseFilter() = default;


void NoiseFilter::apply(Image &img) {
    auto [h, w, c] = img.shape();
    auto data = img.get_data();

    unsigned int totalPixels = h * w;
    unsigned int noisedPixels = static_cast<unsigned int>(totalPixels * (this->getNoisePercentage() / 100.0f));

    // Initialize random number generators
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distributionHeight(0, h - 1);
    std::uniform_int_distribution<int> distributionWidth(0, w - 1);
    std::uniform_int_distribution<int> distributionColor(0, 1); // For deciding between black or white
    std::uniform_int_distribution<int> distributionPixel(0, totalPixels - 1);

    // Create a set to track which pixels have been noised
    std::unordered_set<int> noisedPixelIndices;

    while (noisedPixelIndices.size() < noisedPixels) {
        int pixelIndex = distributionPixel(generator);
        if (noisedPixelIndices.insert(pixelIndex).second) {
            int x = pixelIndex % w;
            int y = pixelIndex / w;
            uint8_t color = distributionColor(generator) * 255;

            for (int channel = 0; channel < c; ++channel) {
                data[channel](y, x) = color;
            }
        }
    }

    img.set_data(data);
}


uint8_t NoiseFilter::getNoisePercentage() const {
    return noisePercentage;
}


void NoiseFilter::setNoisePercentage(uint8_t noisePercentage) {
    NoiseFilter::noisePercentage = noisePercentage;
}

