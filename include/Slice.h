#ifndef SLICE_H
#define SLICE_H

#include "utils/stb_image_write.h"
#include "Volume.h"

class Slice {
public:
    enum SliceDirection {X, Y, Z};

    Slice();

    /**
     * Generates a 2D slice from a 3D volume.
     * 
     * @param volume volume to slice.
     * @param index index of the slice.
     * @param direction The direction in which to slice the volume.
     * @return A 2D slice as a Matrix<uint8_t>.
     */
    Matrix<uint8_t> generate_slice(Volume& volume, int index, SliceDirection direction);

    /**
     * Writes a slice to disk.
     * 
     * @param slice The slice to write.
     * @param output_path The path to write the slice to.
     */
    void write_slice(Matrix<uint8_t>& slice, const char * output_path);

private:
    Matrix<uint8_t> slice_x(Volume& volume, int index, int height, int depth);
    Matrix<uint8_t> slice_y(Volume& volume, int index, int width, int depth);
    Matrix<uint8_t> slice_z(Volume& volume, int index, int height, int width);
};

#endif // SLICE_H