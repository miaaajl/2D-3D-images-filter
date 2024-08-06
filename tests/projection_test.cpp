#include "projection_test.h"

// Helper function to create a test volume with simple patterns
Volume create_test_volume() {
    Volume vol;
    // Layer 1
    vol.data.push_back(Matrix<uint8_t>({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }));
    // Layer 2
    vol.data.push_back(Matrix<uint8_t>({
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    }));
    // Layer 3
    vol.data.push_back(Matrix<uint8_t>({
        {2, 4, 6},
        {8, 10, 12},
        {14, 16, 18}
    }));
    
    vol.load_full_volume = true;
    vol.setN(3);
    return vol;
}

bool test_maximum_projection() {
    Volume testVol = create_test_volume();
    Projection proj(Projection::maximum);

    Image resultImg = proj.execute(testVol);

    Matrix<uint8_t> expected({
        {9, 8, 7},
        {8, 10, 12},
        {14, 16, 18}
    });
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (resultImg.get_data()[0](i, j) != expected(i, j)) {
                return false;
            }
        }
    }
    return true;
}

bool test_minimum_projection(){
    Volume testVol = create_test_volume();
    Projection proj(Projection::minimum);

    Image resultImg = proj.execute(testVol);

    Matrix<uint8_t> expected({
        {1, 2, 3},
        {4, 5, 4},
        {3, 2, 1}
    });

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (resultImg.get_data()[0](i, j) != expected(i, j)) {
                return false;
            }
        }
    }
    return true;
}
bool test_mean_projection(){
    Volume testVol = create_test_volume();
    Projection proj(Projection::mean);
    Image resultImg = proj.execute(testVol);
    Matrix<uint8_t> expected({
        {4, 4, 5},  //Note: The average needs to be rounded to the nearest whole number
        {6, 6, 7},
        {8, 8, 9}
    });
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (resultImg.get_data()[0](i, j) != expected(i, j)) {
                return false;
            }
        }
    }
    return true;
}
bool test_median_projection(){
    Volume testVol = create_test_volume();
    Projection proj(Projection::median);
    Image resultImg = proj.execute(testVol);

    Matrix<uint8_t> expected({
        {2, 4, 6}, 
        {6, 5, 6},
        {7, 8, 9}
    });
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (resultImg.get_data()[0](i, j) != expected(i, j)) {
                return false;
            }
        }
    }
    return true;
}

bool run_projection_test() {
    create_test_volume();
    if(!test_maximum_projection()){
        std::cerr << "Maximum projection test failed.\n";
        return false;
    }
    if (!test_minimum_projection()){
        std::cerr << "Minimum projection test failed.\n";
        return false;
    }
    if (!test_mean_projection()){
        std::cerr << "Mean projection test failed.\n";
        return false;
    }
    if (!test_median_projection()){
        std::cerr << "Median projection test failed.\n";
        return false;
    }
    std::cout << "All projection tests passed!" << std::endl;
    return true;
}
