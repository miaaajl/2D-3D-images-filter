#ifndef BRIGHTNESSFILTER_H
#define BRIGHTNESSFILTER_H

#include "Filter.h"

/**
 * The BrightnessFilter class adjusts the brightness of an image.
 * This class inherits from the Filter class and operates on Image objects.
 * The brightness adjustment is defined by a brightness value, where the range of
 * possible values is from -255 (maximum decrease in brightness) to 255 (maximum increase in brightness).
 * A brightness value of 0 means no change to the image brightness.
 */
class BrightnessFilter : public Filter<Image> {
public:
    /**
     * Constructor for the BrightnessFilter.
     * Initializes a new BrightnessFilter object with a specified brightness adjustment value.
     * If no value is specified, the default brightness adjustment is 0, meaning no change.
     * @param brightness The brightness adjustment value. Range: -255 to 255.
     */
    explicit BrightnessFilter(int brightness = 0);  // Default brightness value is 0.

    /**
     * Destructor for the BrightnessFilter.
     */
    ~BrightnessFilter() override;

    /**
     * Applies the brightness filter to a given image.
     * This method adjusts the brightness of the image based on the brightnessValue property.
     * Positive values increase brightness, negative values decrease it.
     * @param img Reference to the Image object that the filter will be applied to.
     * @return
     */
    void apply(Image &img) override;

private:
    int brightnessValue;  // Optional brightness value in the range -255 to 255.
};

#endif //BRIGHTNESSFILTER_H