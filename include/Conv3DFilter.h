#ifndef CONV3DFILTER_H
#define CONV3DFILTER_H

#include "Filter.h"
#include "Volume.h"

/**
 * The Conv3DFilter class applies convolution operations on 3D volumes.
 * It supports different types of filters like median, gaussian, and box for volume processing tasks.
 * This class inherits from the Filter class and operates on Volume objects.
 */
class Conv3DFilter : public Filter<Volume> {
public:
    /**
     * Enumeration to define the type of filter applied during convolution.
     */
    enum FilterType {
        median,
        gaussian,
        gaussian_separated,
        box,
        box_separated
    };

    /**
     * Default constructor. Initializes the convolution filter with default settings.
     */
    Conv3DFilter();

    /**
     * Parameterized constructor for specifying the filter type and kernel size.
     * @param filter_type The type of filter to be applied.
     * @param kernel_size The size of the kernel matrix. Must be an odd number.
     */
    Conv3DFilter(FilterType filter_type, int kernel_size, bool write = true);

    /**
     * Destructor for the Conv3DFilter.
     */
    ~Conv3DFilter() override;

    void build_kernel(FilterType type);

    void build_callable();

    /**
     * Applies the specified 3D convolution filter to a volume.
     * @param vol The Volume object that the filter will be applied to.
     */
    void apply(Volume &vol) override;

    /**
    * Applies convolutions with a static predetermined 3D kernel_3d
    * to a batch of shape batch x height x width
    * @param batch
    * @param target_batch
    * @return
    */
    void convolve_static_3d(std::vector<Matrix<uint8_t>*> batch);

    void convolve_static_1d(std::vector<Matrix<uint8_t>*> batch, std::tuple<int, int, int> dims);

    /**
     * Applies convolutions inplace with a dynamic 3D kernel_3d based on the neighbourhood of pixels.
     * @param batch
     * @param target_batch
     * @return
     */
    void convolve_dynamic(std::vector<Matrix<uint8_t>*> batch);

    std::vector<Matrix<float>> gaussian_filter_3d();

    std::vector<Matrix<float>> box_filter_3d() const;

    std::vector<float> gaussian_filter_1d();

    std::vector<float> box_filter_1d();

    static std::vector<Matrix<uint8_t>> pad_zero(
        std::vector<Matrix<uint8_t>*> batch, std::tuple<unsigned int, unsigned int, unsigned int> padding_dim
    );

    void convolve_static(const std::vector<Matrix<uint8_t>*>& batch);


private:

    std::vector<Matrix<float>> kernel_3d{};

    std::vector<float> kernel_1d{};

    unsigned int kernel_size{3}, padding{1};

    bool write{true};

    FilterType filter_type{gaussian};

    std::function<void(std::vector<Matrix<uint8_t>*>)> convolve;

};

#endif //CONV3DFILTER_H
