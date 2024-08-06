#ifndef CONV2DFILTER_H
#define CONV2DFILTER_H

#include "Filter.h"
#include <string>
#include <vector>
#include <unordered_map>

/**
 * The Conv2DFilter class implements 2D convolution operations on images.
 * It supports different types of filters like median, box, and Gaussian for image processing tasks.
 * This class inherits from the Filter class and operates on Image objects.
 */
class Conv2DFilter : public Filter<Image> {
public:
    /**
     * An enumeration to define the type of filter applied during convolution.
     */
    enum FilterType {
        median,
        box,
        gaussian
    };

    /**
     * Default constructor. Initializes the convolution filter with default settings.
     */
    Conv2DFilter();

    /**
     * Parameterized constructor for specifying the filter type and kernel size.
     * @param filter_type The type of filter to apply.
     * @param kernel_size The size of the kernel matrix. Must be an odd number.
     */
    Conv2DFilter(FilterType filter_type, int kernel_size, float sigma = 2.);

    /**
     * Destructor for the Conv2DFilter.
     */
    ~Conv2DFilter() override;

    /**
     * Applies the specified 2D convolution filter to an image.
     * @param img The image to apply the filter to.
     * @return
     */
    void apply(Image &img) override;

    /**
     * Generates a box filter kernel.
     * @return The box filter kernel as a Matrix<float>.
     */
    Matrix<float> box_filter() const;

    /**
     * Generates a Gaussian filter kernel.
     * @return The Gaussian filter kernel as a Matrix<float>.
     */
    
    Matrix<float> gaussian_filter() const;


    /**
     * Adds zero padding to the image to prepare for convolution.
     * @param img The image to pad.
     * @return A vector of padded image channels.
     */
    std::vector<Matrix<uint8_t>> pad_zero(Image &img) const;


    /**
     * Performs convolution using a dynamic kernel, such as computing the median of neighbors.
     * @param padded_channel A single padded channel of the image.
     * @param target_channel The channel of the target image to apply the convolution results.
     * @return The convolved image channel.
     */
    Matrix<uint8_t> convolve_static(Matrix<uint8_t> &padded_channel, Matrix<uint8_t> &target_channel);

    /**
    * Convolution for a padded_channel with dynamic kernel_3d, i.e. compute_median of neighbours.
    */
    Matrix<uint8_t> convolve_dynamic(Matrix<uint8_t> &padded_channel, Matrix<uint8_t> &target_channel) const;

    /**
     * Builds the kernel matrix based on the specified filter type.
     * @param type The type of filter to build the kernel for.
     * @return
     */
    void build_kernel(FilterType type);

    /**
     * Sets the kernel matrix for convolution.
     * @param kernel The kernel matrix to be set.
     * @return
     */
    void set_kernel(Matrix<float> kernel);

protected:
    Matrix<float> kernel{}; // The kernel matrix for convolution
    // The size of the kernel, defaults to 3; The padding added to the image, calculated from kernel size
    unsigned int kernel_size{3}, padding{1};
    FilterType filter_type{box};
    float sigma{2};
};


#endif //CONV2DFILTER_H
