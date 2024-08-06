#ifndef VOLUME_H
#define VOLUME_H

#include "Matrix.h"
#include "utils/sorting.h"
#include "utils/auxiliary.h"
#include "utils/stb_image.h"
#include "utils/stb_image_write.h"
#include <filesystem>
#include <deque>
#include <thread>
#include <utility>

/**
 * The Volume class represents and manages a 3D volume of image data, providing functionalities for
 * loading, processing, and writing volumetric data either as a whole or in batches. It supports operations
 * such as dynamic data loading, batch processing, and disk I/O for slices of the volume.
 */
class Volume {
public:
    /**
     * Default constructor. Initializes an empty Volume object.
     */
    Volume();

    /**
     * Load whole volume into memory or batch process
     * @param path
     * @param output_path
     * @param batch_size
     * @param output_format
     * @param load_full_volume
     */
    Volume(
            std::string path,
            std::string output_path,
            int min_depth = -1,
            int max_depth = -1,
            int batch_size = 256,
            const std::string& output_format = "png",
            bool load_full_volume = true
    );

    /**
     * Destructor for the Volume class.
     */
    ~Volume();

    /**
     * Used to shift the data along in the Z direction for batch processing
     */
    void shift();

    /**
     * Write a single slice to disk.
     * @param slice
     * @param idx
     */
    void write_slice(Matrix<uint8_t> slice, int idx);

    /**
     * Write a batch to disk.
     * @param start
     * @param end
     */
    void write_batch(int start, int end);

    /**
     * Add data to the cache for dynamic loading of data
     * @param start
     */
    void load_cache(int start);

    /**
     * Load all data into memory
     */
    void load_data(int start, int end);

    void setN(int depth);

    std::vector<Matrix<uint8_t>*> cache;

    std::tuple<int, int, int> shape();

    std::vector<Matrix<uint8_t>> data{};

    /**
     * Get batch of data for either fully loaded or dynamically loaded data
     * Should be used to interface with the data member if possible.
     * @param start
     * @param end
     * @return vector of pointers to Matrix objects containing pointers to uint8 pixel values.
     */
    std::vector<Matrix<uint8_t>*> get_batch(int start, int end);

    int batch_size{0};

    bool load_full_volume{true};

    std::string summary();

private:
    std::string path;
    std::string output_path, output_format;
    int slice_idx{0};
    int n{0};
  
    std::vector<std::string> filepaths{};

    std::function<void(const std::string&, int, int, unsigned char*)> write_function;

    /**
     * Loads the file paths of individual slices from the volume directory.
     */
    void load_paths();

    /**
     * Loads a single slice of the volume from a file path.
     *
     * @param slice_path The path to the slice file.
     * @return A pointer to a Matrix object containing the slice data.
     */
    static Matrix<uint8_t>* load_slice(const std::string& slice_path);

    /**
     * Retrieves the raw data buffer from a slice for processing or writing.
     *
     * @param slice The slice from which to retrieve the buffer.
     * @param h The height of the slice.
     * @param w The width of the slice.
     * @return A pointer to the raw data buffer of the slice.
     */
    static unsigned char* retrieve_buffer(Matrix<uint8_t> &slice, int h, int w);

};

#endif //VOLUME_H
