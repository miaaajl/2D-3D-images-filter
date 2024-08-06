#include "edge_test.h"

// Helper function, creates a 5x5 image
// with a 3x3 bright (white, value = 255) central area, surrounded by a dark (black, value = 0) border.
Image createTestImage() {
    Image img;
    std::vector<Matrix<uint8_t>> testData;

    // Initialize Matrix with a two-dimensional vector to simulate complex image features
    std::vector<std::vector<uint8_t>> pixelsR = {
        {0, 0, 0, 0, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 0, 0, 0, 0},
    };
    std::vector<std::vector<uint8_t>> pixelsG = {
        {0, 0, 0, 0, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 0, 0, 0, 0},
    };
    std::vector<std::vector<uint8_t>> pixelsB = {
        {0, 0, 0, 0, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 0, 0, 0, 0},
    };

    // Convert these three two-dimensional vectors into Matrix and add them to the test data
    testData.push_back(Matrix<uint8_t>(pixelsR));
    testData.push_back(Matrix<uint8_t>(pixelsG));
    testData.push_back(Matrix<uint8_t>(pixelsB));

    // Set the image's data and shape
    img.set_data(testData);
    img.set_shape(std::make_tuple(5, 5, 3)); // Set the image size to 5x5, with 3 channels

    return img;
}

// Helper function to verify edge detection results
bool verifyEdgeDetection(Image& image) {
    auto data = image.get_data();
    uint8_t centerPixel = data[0](2, 2); // Get the pixel value at the center of the processed image
    uint8_t edgePixel = data[0](2, 1); // Get the pixel value at an edge of the processed image
    if(centerPixel < 10 && edgePixel > 200 ){
        return true;
    }
    return false;
}

bool run_edge_test() {
    // Test each edge detection type in the Edges class
    std::vector<EdgeFilter::EdgeType> types = {EdgeFilter::Sobel, EdgeFilter::Prewitt, EdgeFilter::Scharr, EdgeFilter::Roberts};
    for (auto type : types) {
        std::cout << type << std::endl;
        // Create test image
        Image testImg = createTestImage();
        EdgeFilter edgeFilter(type);
        auto [h,w,c] = testImg.shape();
        // Apply edge detection algorithm
        edgeFilter.apply(testImg, false); // The second parameter decides whether to apply blur preprocessing

        // Verify edge detection results
        if(!verifyEdgeDetection(testImg)){
            std::cout << "Edge detection failed for type: " << type << std::endl;
            return false;
        }
        std::cout << "Edge detection applied and verified for type: " << type << std::endl;
    }
    return true;   
}
