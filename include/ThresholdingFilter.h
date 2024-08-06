#ifndef THRESHOLDINGFILTER_H
#define THRESHOLDINGFILTER_H

#include "Filter.h"
#include "utils/transform.h"

/**
 * The ThresholdingFilter class applies a threshold filter to images. It inherits from the Filter class
 * and operates on Image objects. The filter converts the image to a binary image based on a threshold value.
 * Pixels above the threshold are set to one color, and pixels below are set to another.
 */
class ThresholdingFilter : protected Filter<Image> {
private:
    /**< Threshold value for the filter, determining the cut-off point between the two binary states. */
    uint8_t threshold;
public:
    /**
     * Retrieves the current threshold value used by the filter.
     * @return The threshold value as an unsigned 8-bit integer.
     */
    uint8_t getThreshold() const;

    /**
     * Sets the threshold value to be used by the filter.
     * @param threshold The new threshold value as an unsigned 8-bit integer.
     * @return
     */
    void setThreshold(uint8_t threshold);

    /**
     * Default constructor. Initializes a new ThresholdingFilter object with a default threshold value.
     */
    ThresholdingFilter();

    /**
     * Parameterized constructor. Initializes a new ThresholdingFilter object with a specified threshold value.
     * @param threshold The threshold value as an unsigned 8-bit integer.
     */
    ThresholdingFilter(uint8_t threshold);

    /**
     * Destructor. Cleans up any resources used by the ThresholdingFilter object.
     */
    ~ThresholdingFilter() override;

    /**
     * Applies the threshold filter to a given image in place. Pixels above the threshold are set to white,
     * and pixels below the threshold are set to black.
     * @param img The image to apply the threshold filter to, modified in place.
     * @return
     */
    void apply(Image &img) override;
  
};
#endif //THRESHOLDINGFILTER_H
