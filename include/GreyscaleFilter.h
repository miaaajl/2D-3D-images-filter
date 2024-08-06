#ifndef GREYSCALEFILTER_H
#define GREYSCALEFILTER_H

#include "Filter.h"

/**
 * The GreyscaleFilter class inherits from Filter<Image> and implements an operation
 * to convert a color image into greyscale. The conversion is applied directly to the provided Image object,
 * modifying it in place. The filter assumes the image is in RGB format and converts it to greyscale by averaging
 * the RGB values or using a more sophisticated method for preserving perceived luminance.
 */
class GreyscaleFilter : public Filter<Image> {
public:
    /**
     * Default constructor for the GreyscaleFilter.
     * Initializes a new GreyscaleFilter object ready to be applied to Image objects.
     */
    GreyscaleFilter();

    /**
     * Destructor for the GreyscaleFilter.
     * Overrides the virtual destructor from the Filter base class. Ensures that any
     * resources allocated by GreyscaleFilter are properly released.
     */
    ~GreyscaleFilter() override;

    /**
     * Applies the greyscale filter to an Image object.
     * This method converts the color image into greyscale by modifying each pixel of the image in place.
     * The image must be in RGB format. The method adjusts the RGB values of each pixel to reflect
     * the greyscale equivalent, effectively removing color while preserving luminance.
     *
     * @param img A reference to an Image object to which the greyscale filter will be applied. The image
     * is modified in place.
     * @return
     */
    void apply(Image &img) override;

};


#endif //GREYSCALEFILTER_H
