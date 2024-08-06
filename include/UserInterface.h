#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include "Volume.h"
#include "Slice.h"
#include "BrightnessFilter.h"
#include "Conv2DFilter.h"
#include "Conv3DFilter.h"
#include "Filter.h"
#include "GreyscaleFilter.h"
#include "HistogramEqualisationFilter.h"
#include "Image.h"
#include "Matrix.h"
#include "NoiseFilter.h"
#include "Projection.h"
#include "utils/stb_image.h"
#include "ThresholdingFilter.h"
#include "EdgeFilter.h"


class Interface {
    public:
        /**
         * Apply 2D image processing
         */
        void apply2DImageProcessing();
        /**
         * Apply 3D image processing
         */
        void apply3DImageProcessing();
        /**
         * option to apply blurring for 3D image processing
         */
        bool blurringOption();
        /**
         * Create a volume object from the given image path
         * @param imagePath the path to the image
         * @param OutputPath the path to the output image
         * @param batchingOption the option to apply batching
         */
        Volume createVolumeObject(std::string imagePath, std::string OutputPath, bool batchingOption);
        /**
         * Create a volume object from the given image path
         * @param imagePath the path to the image
         */
        void applyProjectionSlicing(std::string path);
        /**
         * Run the user interface
         */
        void runInterface();
    };

#endif //INTERFACE_H
