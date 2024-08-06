#include "Slice.h"

Slice::Slice() = default;

Matrix<uint8_t> Slice::generate_slice(Volume& volume, int index, SliceDirection direction) {
    auto [volHeight, volWidth, volDepth] = volume.shape();

    switch (direction) {
        case X:
            return slice_x(volume, index, volHeight, volDepth);
        case Y:
            return slice_y(volume, index, volWidth, volDepth);
        case Z:
            return slice_z(volume, index, volHeight, volWidth);
        default:
            throw std::invalid_argument("Invalid slicing direction");
    }
}

Matrix<uint8_t> Slice::slice_x(Volume& volume, int x, int height, int depth) {
    Matrix<uint8_t> slice(height, depth);

    // Check if volume is fully loaded
    if (volume.load_full_volume) {
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < height; ++y) {
                slice(y, z) = volume.data[z](y, x);
            }
        }
        return slice;

    } else {
        // Load the volume in batches
        for (int z = 0; z < depth; ++z) {
            if (z % volume.batch_size == 0 && z != 0) { 
                volume.shift(); // Shift to the next batch
            }
            int sliceIndexInCurrentBatch = z % volume.batch_size;
            for (int y = 0; y < height; ++y) {
                slice(y, z) = (*volume.cache[sliceIndexInCurrentBatch])(y, x);
            }
        }
    }
    return slice;
}

Matrix<uint8_t> Slice::slice_y(Volume& volume, int y, int width, int depth) {
    Matrix<uint8_t> slice(width, depth);

    if (volume.load_full_volume) {
        for (int z = 0; z < depth; ++z) {
            for (int x = 0; x < width; ++x) {
                slice(x, z) = volume.data[z](y, x);
            }
        }
        return slice;
    } else {
        // Load the volume in batches
        for (int z=0; z < depth; ++z) {
            if (z % volume.batch_size == 0 && z != 0) { 
                volume.shift(); // Shift to the next batch
            }
            int sliceIndexInCurrentBatch = z % volume.batch_size;
            for (int x = 0; x < width; ++x) {
                slice(x, z) = (*volume.cache[sliceIndexInCurrentBatch])(y, x);
            }
        }
    }
    return slice;
}

Matrix<uint8_t> Slice::slice_z(Volume& volume, int z, int height, int width) {
    if (volume.load_full_volume) {
        return volume.data[z];
    } else {
        return *volume.cache.front();
    }
}

void Slice::write_slice(Matrix<uint8_t>& slice, const char * output_path) {
    int width, height;
    std::tie(height, width) = slice.shape();
    std::vector<uint8_t> image_data;
    image_data.reserve(height * width);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image_data.push_back(slice(y, x));
        }
    }
    stbi_write_png(output_path, width, height, 1, image_data.data(), width);
}