#ifndef EDGES_H
#define EDGES_H

#include "Image.h"
#include "Conv2DFilter.h"


/**
 * The Edges class applies various edge detection filters to images.
 * It inherits from the Conv2DFilter class and allows for the application of Sobel, Prewitt,
 * Scharr, and Roberts edge detection techniques.
 */
class EdgeFilter : public Conv2DFilter {
public:
    /**
     * Enumeration to define the type of edge detection filter to be applied.
     */
    enum EdgeType {
        Sobel,
        Prewitt,
        Scharr,
        Roberts
    };

    /**
     * Constructor that initializes an Edges object with a specified edge detection type.
     * @param edgeType The type of edge detection filter to apply.
     */
    EdgeFilter(EdgeType edgeType);

    /**
     * Destructor for the Edges class.
     */
    ~EdgeFilter() override;

    /**
     * Applies the selected edge detection filter to an image.
     * Optionally, a blur can be applied before the edge detection to reduce noise.
     * @param img The image to which the edge detection filter will be applied.
     * @param apply_blur Whether to apply a blur to the image before edge detection.
     * @return
     */
    void apply(Image &img, bool apply_blur);

private:
    EdgeType edgeType; // The selected type of edge detection filter.

    /**
     * Builds the Sobel filter kernel matrix.
     * @param is_in_X Indicates whether to build the kernel for X or Y direction.
     * @return The Sobel kernel matrix for the specified direction.
     */
    Matrix<float> buildSobel(bool is_in_X);

    /**
     * Builds the Prewitt filter kernel matrix.
     * @param is_in_X Indicates whether to build the kernel for X or Y direction.
     * @return The Prewitt kernel matrix for the specified direction.
     */
    Matrix<float> buildPrewitt(bool is_in_X);

    /**
     * Builds the Scharr filter kernel matrix.
     * @param is_in_X Indicates whether to build the kernel for X or Y direction.
     * @return The Scharr kernel matrix for the specified direction.
     */
    Matrix<float> buildScharr(bool is_in_X);

    /**
     * Builds the Roberts Cross filter kernel matrix.
     * @param is_in_X Indicates whether to build the kernel for X or Y direction (ignored for Roberts Cross).
     * @return The Roberts Cross kernel matrix.
     */
    Matrix<float> buildRobertsCross(bool is_in_X);

    /**
     * Preprocesses the image before applying the edge detection filter.
     * This may include converting the image to greyscale and optionally applying a blur.
     * @param img The image to preprocess.
     * @param apply_blur Whether to apply a blur to the image as part of preprocessing.
     * @return
     */
    void preprocessImage(Image &img, bool apply_blur = false);
};

#endif //EDGES_H

