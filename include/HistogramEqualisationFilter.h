#ifndef HISTOGRAMEQUALISATIONFILTER_H
#define HISTOGRAMEQUALISATIONFILTER_H

#include "Filter.h"
#include "Image.h"
#include "utils/transform.h"
#include <vector>
#include <cstdint>

/**
 * The HistogramEqualisationFilter class applies histogram equalization to images
 * to improve the contrast in an image. The filter can be applied to both grayscale
 * and RGB images. For RGB images, the equalization is performed on the intensity
 * channel in the HSV color space.
 */
class HistogramEqualisationFilter : public Filter<Image> {
public:
    /**
     * Constructor for the HistogramEqualisationFilter.
     * Initializes a new HistogramEqualisationFilter object.
     */
    HistogramEqualisationFilter();

    /**
     * Destructor for the HistogramEqualisationFilter.
     */
    ~HistogramEqualisationFilter() override;

    /**
     * Applies histogram equalization to an image to improve its contrast.
     * For grayscale images, it stretches the intensity histogram. For RGB images,
     * it equalises the intensity values across all channels.
     *
     * @param img The image to which the histogram equalization filter will be applied.
     * @return
     */
    void apply(Image &img) override;

    void setColorSpace(const std::string& space);
private:
    std::string color_space = "HSV"; // The default is HSV
    // Compute histogram, overloaded for uint8_t and float types

    template<typename T>
    std::vector<int> compute_histogram(const Matrix<T>& channel);

    /**
     * Computes the cumulative histogram from a histogram.
     *
     * @param histogram The histogram from which to compute the cumulative histogram.
     * @return A vector representing the cumulative histogram.
     */
    std::vector<int> compute_cumulative_histogram(const std::vector<int>& histogram);

    /**
     * Applies histogram equalization to a single channel using the cumulative histogram.
     *
     * @param channel The channel to which the histogram equalization will be applied.
     * @param cumulative_histogram The cumulative histogram used for equalization.
     */
    template<typename T>
    void apply_histogram_equalization(Matrix<T>& channel, const std::vector<int>& cumulative_histogram);

};


#endif //HISTOGRAMEQUALISATION_H
