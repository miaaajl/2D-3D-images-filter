#ifndef NOISEFILTER_H
#define NOISEFILTER_H
#include "Filter.h"
#include <random>
#include <unordered_set>

/**
 * The NoiseFilter class applies a noise effect to images. It inherits from the Filter class
 * and operates on Image objects, allowing for pointwise greyscale noise addition.
 * The intensity of the noise effect can be adjusted through the noisePercentage property.
 */
class NoiseFilter : public Filter<Image> {
private:
    /**< The percentage of noise to apply to the image, ranging from 0 to 100. */
    uint8_t noisePercentage;
public:
    /**
     * Retrieves the current noise percentage applied by the filter.
     * @return The noise percentage as an unsigned 8-bit integer.
     */
    uint8_t getNoisePercentage() const;

    /**
     * Sets the noise percentage to be applied by the filter.
     * @param noisePercentage The new noise percentage as an unsigned 8-bit integer.
     * @return
     */
    void setNoisePercentage(uint8_t noisePercentage);

    /**
     * Default constructor. Initializes a new NoiseFilter3 object with a default noise percentage.
     */
    NoiseFilter();

    /**
     * Parameterized constructor. Initializes a new NoiseFilter object with a specified noise percentage.
     * @param _noisePercentage The noise percentage as an unsigned 8-bit integer.
     */
    NoiseFilter(uint8_t _noisePercentage);

    /**
     * Destructor. Cleans up any resources used by the NoiseFilter object.
     */
    ~NoiseFilter() override;

    /**
    * Applies the noise filter to a given image in place. The operation is performed pointwise
    * on the greyscale representation of the image.
    * @param img The image to apply the noise filter to, modified in place.
    * @return
    */
    void apply(Image &img) override;

};

#endif //NOISEFILTER_H
