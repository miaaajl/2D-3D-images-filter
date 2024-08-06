#include "UserInterface.h"

bool Interface::blurringOption() {
    std::string blurChoice;
    std::cout << "Would you like to apply blurring before edge detection? (yes/no): ";
    std::getline(std::cin, blurChoice);
    bool blurImage;
    if (blurChoice == "yes") {
        return true;
    } else {
        return false;
    }
}


void Interface::apply2DImageProcessing() {
    bool flag = true;
    std::string imagePath;
    std::cout << "Enter the path to the image file: ";
    std::getline(std::cin, imagePath);

    // New check for image path validity
    std::ifstream infile(imagePath);
    if (!infile.good()) {
        std::cout << "Error: The provided image path is invalid or the file does not exist." << std::endl;
        return;
    }
    infile.close();

    std::string colorSpace;
    bool validColorSpace = false;
    while (!validColorSpace) {
        std::cout << "Is your image in color or grayscale? (color/grayscale): ";
        std::getline(std::cin, colorSpace);

        if (colorSpace == "color" || colorSpace == "grayscale") {
            validColorSpace = true;
        } else {
            std::cout << "Invalid input. Please enter 'color' or 'grayscale'." << std::endl;
        }
    }

    Image img;
    if (colorSpace == "color") {
        img = Image(imagePath, 3);
    } else {
        img = Image(imagePath, 1);
    }

    bool applyMoreFilters = true;

    while (applyMoreFilters) {
        int choice;
        std::cout << "\nSelect a filter number:\n";
        std::cout << "  1. Grayscale\n";
        std::cout << "  2. Brightness\n";
        std::cout << "  3. Histogram equalisation\n";
        std::cout << "  4. Thresholding\n";
        std::cout << "  5. Salt and Pepper Noise\n";
        std::cout << "  6. Median blur\n";
        std::cout << "  7. Box blur\n";
        std::cout << "  8. Gaussian blur\n";
        std::cout << "  9. Edge detection - Sobel\n";
        std::cout << "  10. Edge detection - Prewitt\n";
        std::cout << "  11. Edge detection - Scharr\n";
        std::cout << "  12. Edge detection - Roberts' Cross\n";
        std::cout << "Enter your choice number (1-12): ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) { // If Grayscale filter is chosen
            if (colorSpace == "color") {
                auto gray_filter = GreyscaleFilter();
                gray_filter.apply(img);
                img.write("../Output/Interface_Outputs/2D/image_greyscale.png", 1);
                colorSpace = "grayscale";
            } else {
                flag = false;
                std::cout << "Image is already in grayscale.\n";
            }
        }

        else if (choice == 2) { // If Brightness filter is chosen
            int brightness;
            std::cout << "Enter desired brightness level (-255 to 255): ";
            std::cin >> brightness;
            std::cin.ignore();
            BrightnessFilter bf(brightness);
            bf.apply(img);
            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_brightened.png", 3);
            } else {
                img.write("../Output/Interface_Outputs/2D/image_brightened.png", 1);
            }
        }

        else if (choice == 3) { // If Histogram equalisation filter is chosen
            HistogramEqualisationFilter he_filter;

            // Ask the user to choose between HSV and HSL color spaces
            std::string colorSpaceChoice;
            std::cout << "Choose color space for histogram equalisation ('HSV' or 'HSL'): ";
            std::getline(std::cin,colorSpaceChoice);

            if (colorSpaceChoice == "HSL") {
                he_filter.setColorSpace("HSL");  // Set color space to HSL if chosen
            }
            // No need to set for HSV since it is default or already set

            // Apply the histogram equalization filter
            he_filter.apply(img);

            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_histogram_equalised.png", 3);
            } else {
                img.write("../Output/Interface_Outputs/2D/image_histogram_equalised.png", 1);
            }
        }

        else if (choice == 4) { // If Thresholding filter is chosen
            int threshold;
            std::cout << "Enter desired Threshold level (0 to 255) as an integer: ";
            std::cin >> threshold;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            ThresholdingFilter th(threshold);
            th.apply(img);
            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_thresholded.png", 3);
            } else {
                std::cout << "image must be in colour" << std::endl;
                flag = false;
            }
        }

        else if (choice == 5) { // If Salt and Pepper Noise filter is chosen
            uint8_t noisePercentage;
            bool isValidPercentage = false;

            while (!isValidPercentage) {
                std::cout << "Enter the percentage of pixels that should have noise (0-100) as an integer: ";
                std::cin >> noisePercentage;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (noisePercentage <= 100) {
                    isValidPercentage = true;
                    auto noise_filter = NoiseFilter(noisePercentage);
                    noise_filter.apply(img);
                } else {
                    std::cout << "Invalid noise percentage. Please enter a value between 0 and 100.\n";
                }
            }
            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_salt_and_pepper_noised.png", 3);
            } else {
                img.write("../Output/Interface_Outputs/2D/image_salt_and_pepper_noised.png", 1);
            }
        }

        else if (choice == 6) { // If Median blur filter is chosen
            int kernelSize;
            while (true) { // Start an infinite loop to keep asking for input until a valid value is entered
                std::cout << "Enter the kernel size for the Median blur (must be an odd integer greater than 1): ";
                std::cin >> kernelSize;
                std::cin.ignore();

                // Check if the kernel size is valid
                if (kernelSize > 1 && kernelSize % 2 != 0) {
                    // If valid, break out of the loop
                    break;
                } else {
                    // If not valid, inform the user and the loop will continue
                    std::cout << "Invalid kernel size. It must be an odd number greater than 1. Please try again.\n";
                }
            }

            // Now that a valid kernel size is entered, apply the filter
            auto conv_filter = Conv2DFilter(Conv2DFilter::median, kernelSize);
            conv_filter.apply(img);

            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_median_blurred.png", 3);
            } else {
                img.write("../Output/Interface_Outputs/2D/image_median_blurred.png", 1);
            }
        }

        else if (choice == 7) { // If Box blur filter is chosen
            int kernelSize;
            while (true) { // Start an infinite loop to keep asking for input until a valid value is entered
                std::cout << "Enter the kernel size for the Box blur (must be an odd number greater than 1): ";
                std::cin >> kernelSize;
                std::cin.ignore();

                // Check if the kernel size is valid
                if (kernelSize > 1 && kernelSize % 2 != 0) {
                    // If valid, break out of the loop
                    break;
                } else {
                    // If not valid, inform the user and the loop will continue
                    std::cout << "Invalid kernel size. It must be an odd number greater than 1. Please try again.\n";
                }
            }

            // Now that a valid kernel size is entered, apply the filter
            auto conv_filter = Conv2DFilter(Conv2DFilter::box, kernelSize);
            conv_filter.apply(img);

            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_box_blurred.png", 3);
            } else {
                img.write("../Output/Interface_Outputs/2D/image_box_blurred.png", 1);
            }
        }

        else if (choice == 8) { // If Gaussian blur filter is chosen
            int kernelSize;
            while (true) { // Start an infinite loop to keep asking for input until a valid value is entered
                std::cout << "Enter the kernel size for the Gaussian blur (must be an odd number greater than 1): ";
                std::cin >> kernelSize;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Check if the kernel size is valid
                if (kernelSize > 1 && kernelSize % 2 != 0) {
                    // If valid, break out of the loop
                    break;
                } else {
                    // If not valid, inform the user and the loop will continue
                    std::cout << "Invalid kernel size. It must be an odd number greater than 1. Please try again.\n";
                }
            }

            std::cout << "Enter the sigma value for the Gaussian blur: ";
            float sigma;
            std::cin >> sigma;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Now that a valid kernel size is entered, apply the filter
            auto conv_filter = Conv2DFilter(Conv2DFilter::gaussian, kernelSize, sigma);
            conv_filter.apply(img);

            if (colorSpace == "color") {
                img.write("../Output/Interface_Outputs/2D/image_gaussian_blurred.png", 3);
            } else {
                img.write("../Output/Interface_Outputs/2D/image_gaussian_blurred.png", 1);
            }
        }

        else if (choice == 9) { // If Edge detection - Sobel filter is chosen
            EdgeFilter edges(EdgeFilter::Sobel);
            edges.apply(img, blurringOption()); // true for blurring || false for no blurring
            img.write("../Output/Interface_Outputs/2D/image_sobel_edges.png", 1);
            colorSpace = "grayscale";
        }

        else if (choice == 10) { // If Edge detection - Prewitt filter is chosen
            EdgeFilter edges_p(EdgeFilter::Prewitt);
            edges_p.apply(img, blurringOption()); // true for blurring || false for no blurring
            img.write("../Output/Interface_Outputs/2D/image_prewitt_edges.png", 1);
            colorSpace = "grayscale";
        }

        else if (choice == 11) { // If Edge detection - Scharr filter is chosen
            EdgeFilter edges_s(EdgeFilter::Scharr);
            edges_s.apply(img, blurringOption()); // true for blurring || false for no blurring
            img.write("../Output/Interface_Outputs/2D/image_scharr_edges.png", 1);
            colorSpace = "grayscale";
        }

        else if (choice == 12) { // If Edge detection - Roberts' Cross filter is chosen
            EdgeFilter edges_r(EdgeFilter::Roberts);
            edges_r.apply(img, false); // this must be false for roberts as no blurring needed
            img.write("../Output/Interface_Outputs/2D/image_roberts_cross_edges.png", 1);
            colorSpace = "grayscale";
        }

        else {
            flag = false;
            std::cout << "No processing was applied to the 2D data.\n";
        }

        std::string moreFilters;
        std::cout << "\nWould you like to apply more filters to the most recent filtered image? (yes/no): ";
        std::getline(std::cin, moreFilters);

        if (moreFilters == "no") {
            applyMoreFilters = false;
        }
    }
    std::cout << "2D data has been processed and saved successfully in the 'Output' folder.\n";
}


Volume Interface::createVolumeObject(std::string imagePath, std::string OutputPath, bool batchingOption) {
    Volume vol;
    std::string bacthBool;

    if (batchingOption) {
        std::cout << "\nWould you like to use batches or load the whole volume data?\n";
        std::cout << "  1. Batches\n";
        std::cout << "  2. Load whole volume\n";
        std::cout << "Enter your choice (1-2): ";
        std::cin >> bacthBool;
        }
    else {
        bacthBool = "2";
    }

    if (bacthBool == "1") {
        std::cout << "\nEnter the batch size: ";
        int batchSize;
        std::cin >> batchSize;
        std::cout << "\nLoading in volume...\n";
        vol = Volume(
                imagePath,
                OutputPath,
                -1,-1,
                batchSize,  // can take any batch_size
                "png",
                false  // choose based on memory limitations and size of volume
        );
    } else {
        std::cout << "\nLoading in volume...\n";
        vol = Volume(
                imagePath,
                OutputPath,
                -1,-1,
                125,  // can take any batch_size but is pointless
                "png",
                true  // choose based on memory limitations and size of volume
        );
    }
    return vol;
}

void Interface::apply3DImageProcessing() {
    std::string imagePath;
    std::cout << "\nEnter the path to the 3D image file: ";
    std::getline(std::cin, imagePath);

    std::ifstream infile(imagePath);
    if (!infile.good()) {
        std::cout << "Error: The provided image path is invalid or the file does not exist." << std::endl;
        return;
    }
    infile.close();

    bool filterApplied = false;

    std::string filterChoice;
    std::cout << "\nWould you like to apply a 3D filter? (yes/no): ";
    std::cin >> filterChoice;
    std::cin.ignore();

    if (filterChoice == "yes") {
        filterApplied = true;  // A filter will be applied, so set the flag to true
        // Display available 3D filters and ask user to choose one
        std::cout << "\nSelect a 3D filter:\n";
        std::cout << "  1. 3D Gaussian\n";
        std::cout << "  2. 3D Median\n";
        int filterSelection;
        std::cout << "Enter your choice number (1-2): ";
        std::cin >> filterSelection;
        std::cin.ignore();

        // Apply selected 3D filter
        if (filterSelection == 1) {
            int kernelSize;
            while (true) {
                std::cout << "\nEnter the kernel size for the Gaussian blur (must be an odd number greater than 1): ";
                std::cin >> kernelSize;
                std::cin.ignore();

                // Check if the kernel size is valid
                if (kernelSize > 1 && kernelSize % 2 != 0) {
                    // If valid, break out of the loop
                    break;
                } else {
                    // If not valid, inform the user and the loop will continue
                    std::cout << "\nInvalid kernel size. It must be an odd number greater than 1. Please try again.\n";
                }
            }

            Volume vol = createVolumeObject(imagePath, "../Output/Interface_Outputs/3D/Gaussian/3D_Gaussian", true);
            Conv3DFilter filter = Conv3DFilter(Conv3DFilter::gaussian_separated, kernelSize);
            filter.apply(vol);
            // Show image using slicing
            
            applyProjectionSlicing(imagePath);

            std::cout << "\n3D data has been processed and saved successfully in the 'Output' folder.\n";

        } else if (filterSelection == 2) {
                int kernelSize;
                while (true) {
                    std::cout << "\nEnter the kernel size for the Median blur (must be an odd number greater than 1): ";
                    std::cin >> kernelSize;
                    std::cin.ignore();

                    // Check if the kernel size is valid
                    if (kernelSize > 1 && kernelSize % 2 != 0) {
                        // If valid, break out of the loop
                        break;
                    } else {
                        // If not valid, inform the user and the loop will continue
                        std::cout << "\nInvalid kernel size. It must be an odd number greater than 1. Please try again.\n";
                    }
                }

                Volume vol = createVolumeObject(imagePath, "../Output/Interface_Outputs/3D/Median/3D_Median", true);
                Conv3DFilter filter = Conv3DFilter(Conv3DFilter::median, kernelSize);
                filter.apply(vol);

                applyProjectionSlicing(imagePath);

                std::cout << "\n3D data has been processed and saved successfully in the 'Output' folder.\n";
        } else {
            std::cout << "\nInvalid filter selection. Skipping filtering.\n";
        }
    }
    else if (filterChoice == "no") {
        std::cout << "\nNo 3D filter will be applied.\n";
        applyProjectionSlicing(imagePath);
    }

    else {
        std::cout << "\nInvalid choice. Exiting.\n";
    }
}


void Interface::applyProjectionSlicing(std::string path) {
    bool projectionApplied = false;

    std::string projectionChoice;
    std::cout << "\nWould you like to apply projection or slicing? yes/no: ";
    std::cin >> projectionChoice;
    std::cin.ignore();

    if (projectionChoice == "yes") {
        bool flag = true;
        projectionApplied = true;  // A projection will be applied, so we set the flag to true
        int projectionChoice;
        std::cout << "\nSelect a projection or slicing option: \n";
        std::cout << "  1. Maximum Intensity Projection (MIP)\n";
        std::cout << "  2. Minimum Intensity Projection (MinIP)\n";
        std::cout << "  3. Average Intensity Projection (AIP)\n";
        std::cout << "  4. AIP for Median Projection\n";
        std::cout << "  5. Slice\n";
        std::cout << "Enter your choice (1-5): ";

        std::cin >> projectionChoice;
        std::cin.ignore();

        if (projectionChoice == 1) {
            Volume vol = createVolumeObject(path, "../Output/Interface_Outputs/3D/Projections_Slices", false);
            Projection p = Projection(Projection::maximum);
            Image img = p.execute(vol);
            img.write("../Output/Interface_Outputs/3D/projection_maximum.png", 1);

        } else if (projectionChoice == 2) {
            Volume vol = createVolumeObject(path, "../Output/Interface_Outputs/3D/Projections_Slices", false);
            Projection p = Projection(Projection::minimum);
            Image img = p.execute(vol);
            img.write("../Output/Interface_Outputs/3D/projection_minimum.png", 1);

        } else if (projectionChoice == 3) {
            Volume vol = createVolumeObject(path, "../Output/Interface_Outputs/3D/Projections_Slices", false);
            Projection p = Projection(Projection::mean);
            Image img = p.execute(vol);
            img.write("../Output/Interface_Outputs/3D/projection_mean.png", 1);

        } else if (projectionChoice == 4) {
            Volume vol = createVolumeObject(path, "../Output/Interface_Outputs/3D/Projections_Slices", false);
            Projection p = Projection(Projection::median);
            Image img = p.execute(vol);
            img.write("../Output/Interface_Outputs/3D/projection_median.png", 1);
        } else if (projectionChoice == 5) {
            Slice slicer;
            std::cout << "\nSelect a slice option:\n";
            std::cout << "  1. YZ Plane - Same X\n";
            std::cout << "  2. XZ Plane - Same Y\n";
            std::cout << "  3. XY Plane - Same Z\n";
            std::cout << "Enter your choice (1-3): ";
            int sliceChoice;
            std::cin >> sliceChoice;

            std::cout << "\nWhat image index would you like to slice? ";
            int index;
            std::cin >> index;

            Volume vol = createVolumeObject(path, "../Output/Interface_Outputs/3D/Projections_Slices", true);

            if (sliceChoice == 1) {
                // Generate a slice in the YZ plane (same X)
                auto slice = slicer.generate_slice(vol, index, Slice::X);
                slicer.write_slice(slice, "../Output/Interface_Outputs/3D/slice_output_YZ.png");
            } else if (sliceChoice == 2) {
                // Generate a slice in the XZ plane (same Y)
                auto slice = slicer.generate_slice(vol, index, Slice::Y);
                slicer.write_slice(slice, "../Output/Interface_Outputs/3D/slice_output_XZ.png");
            } else if (sliceChoice == 3) {
                // Generate a slice in the XY plane (same Z)
                auto slice = slicer.generate_slice(vol, index, Slice::Z);
                slicer.write_slice(slice, "../Output/Interface_Outputs/3D/slice_output_XY.png");
            } else {
                std::cout << "\nInvalid projection/slice selection. Exiting.\n";
            }
        } else {
            std::cout << "\nInvalid projection/slice selection. Exiting.\n";
            flag = false;
        }
        if (flag) {
        std::cout << "\n3D data has been processed and saved successfully in the 'Output' folder.\n";
        }
    }
    else {
        std::cout << "\nNo projection/slice will be applied.\n";
    }
}

void Interface::runInterface() {
    while (true) {
        std::cout << "\nWelcome to the Image Processing Interface\n\n";
        std::cout << "Choose an option:\n";
        std::cout << "1. Apply 2D Image Processing\n";
        std::cout << "2. Apply 3D Image Processing\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice (1-3): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline character from the input buffer

        switch (choice) {
            case 1:
                apply2DImageProcessing();
                break;
            case 2:
                apply3DImageProcessing();
                break;
            case 3:
                std::cout << "Exiting the Image Processing Interface.\n";
                return;
            default:
                std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
                break;
        }
    }
}
