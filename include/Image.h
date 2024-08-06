#ifndef IMAGE_H
#define IMAGE_H

#include "Matrix.h"
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

/**
 * The Image class represents an image in memory, providing functionality for loading, manipulating,
 * and saving images. It supports operations on the image data as well as accessing and modifying image properties.
 */
class Image {
public:
    /**
     * Default constructor. Initializes an empty Image object.
     */
    Image();

    /**
     * Constructor that initializes an Image object from a file.
     *
     * @param path Path to the image file.
     * @param desired_channels The number of color channels desired in the loaded image.
     *                         Set to 0 to auto-detect and load all available channels.
     */
    explicit Image(const std::string &path, int desired_channels = 0);

    /**
     * Destructor for the Image class.
     */
    ~Image();

    /**
     * Writes the image data to a file.
     *
     * @param path Path where the image will be saved.
     * @param out_channels The number of color channels to save; must match the image's channel count.
     * @return 0 on success, non-zero on failure.
     */
    int write(const std::string &path, int out_channels);

    /**
     * Constructs the internal image tensor from a raw buffer.
     *
     * @param buffer Pointer to the raw image data buffer.
     * @return
     */
    void construct_tensor(const unsigned char* buffer);

    /**
     * Retrieves a pointer to the internal buffer containing the image data.
     *
     * @return Pointer to the image data buffer.
     */
    unsigned char* retrieve_buffer();

    /**
     * Gets the image data as a vector of matrices, one per channel.
     *
     * @return Vector containing one matrix per image channel.
     */
    std::vector<Matrix<uint8_t>> get_data();

    /**
     * Sets the image data from a vector of matrices.
     *
     * @param new_data Vector of matrices containing the image data, one per channel.
     * @return
     */
    void set_data(std::vector<Matrix<uint8_t>> &new_data);

    /**
     * Gets the shape of the image as a tuple of height, width, and channel count.
     *
     * @return A tuple containing the height, width, and channel count.
     */
    std::tuple<int, int, int> shape();

    /**
     * Sets the number of channels in the image.
     *
     * @param channels The new channel count.
     * @return
     */
    void set_channels(int channels);

    /**
     * Sets the shape (dimensions) of the image.
     *
     * @param shape A tuple containing the new height, width, and channel count.
     * @return
     */
    void set_shape(std::tuple<int, int, int> shape);

    /**
     * Generates a summary of the image properties.
     *
     * @return A string summarizing the image properties, such as dimensions and channel count.
     */
    std::string summary() const;

private:
    int h{0}, w{0}, c{0}; // Image dimensions: height (h), width (w), and number of channels (c).
    std::vector<Matrix<uint8_t>> data{}; // Image data, stored as a vector of matrices (one per channel).

};


#endif //IMAGE_H
