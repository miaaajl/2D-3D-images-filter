#include "slice_test.h"

// Helper function to create a test volume with simple patterns
Volume createMockVolume() {
    Volume vol;
    // Layer 1
    vol.data.push_back(Matrix<uint8_t>({
        {0, 1, 1},
        {0, 1, 1},
        {0, 1, 1}
    }));
    // Layer 2
    vol.data.push_back(Matrix<uint8_t>({
        {10, 10, 10},
        {5, 5, 5},
        {10, 10, 10}
    }));
    // Layer 3
    vol.data.push_back(Matrix<uint8_t>({
        {8, 4, 4},
        {4, 8, 4},
        {4, 4, 8}
    }));
    vol.load_full_volume = true;
    vol.setN(3);
    return vol;
}

bool test_slice_x_direction() {
    // Create the test volume
    Volume vol = createMockVolume();
    Slice slicer;

    // Generate a slice from the volume in X direction
    Matrix<uint8_t> slice = slicer.generate_slice(vol, 1, Slice::X);

    Matrix<uint8_t> expected({
        {1, 1, 1},
        {10, 5, 10},
        {4, 8, 4}
    });
    // Verify the slice's content in X direction
    for (int z = 0; z < 3; ++z) {
        for (int y = 0; y < 3; ++y) {
            if (static_cast<int>(slice(y, z)) != static_cast<int>(expected(z,y))){
                return false;
            }
        }
    }
    return true;
}

bool test_slice_y_direction() {
    // Create the test volume
    Volume vol = createMockVolume();
    Slice slicer;

    // Generate a slice from the volume in Y direction
    Matrix<uint8_t> slice = slicer.generate_slice(vol, 1, Slice::Y);

    Matrix<uint8_t> expected({
        {0, 1, 1},
        {5, 5, 5},
        {4, 8, 4}
    });
    // Verify the slice's content in Y direction
    for (int z = 0; z < 3; ++z) {
        for (int x = 0; x < 3; ++x) {
            if (static_cast<int>(slice(x, z)) != static_cast<int>(expected(z,x))){
                return false;
            }
        }
    }
    return true;
}

bool test_slice_z_direction() {
    // Create the test volume
    Volume vol = createMockVolume();
    Slice slicer;

    // Generate a slice from the volume in Z direction
    Matrix<uint8_t> slice = slicer.generate_slice(vol, 1, Slice::Z);

    Matrix<uint8_t> expected({
        {10, 10, 10},
        {5, 5, 5},
        {10, 10, 10}
    });
    // Verify the slice's content in Z direction
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (static_cast<int>(slice(x, y)) != static_cast<int>(expected(x , y))){
                return false;
            }
        }
    }
    return true;
}

bool run_slice_test() {
    if (!test_slice_x_direction()) {
        std::cerr << "Slice in x_direction failed." << std::endl;
        return false;
    }
    if (!test_slice_y_direction()) {
        std::cerr << "Slice in y_direction failed." << std::endl;
        return false;
    }
    if (!test_slice_z_direction()) {
        std::cerr << "Slice in z_direction failed." << std::endl;
        return false;
    }
    std::cerr << "All tests passed." << std::endl;
    return true;
}
