### Build/Installation

```c++
mkdir build && cd build
cmake .. && make
./advanced_programming_group_binary_search
```

### Example usage

### Image and 2D Filters

```c++
#include "Image.h"
#include "GreyscaleFilter.h"
#include "Conv2DFilter.h"

// read in image, with desired channels
Image img("Images/gracehopper.png", 3);

// get shape and print
std::cerr << img.summary() << "\n";

// greyscale filtering
GreyscaleFilter filter();
filter.apply(img);
img.write("Output/img1_grey.png", 1);

// now convolutional filtering
Conv2DFilter conv_filter(Conv2DFilter::gaussian, 5);
conv_filter.apply(img);
img.write("Output/blurred.png", 3);

// now edge filter
Conv2DFilter conv_filter(Conv2DFilter::gaussian, 5);
Edges edge(Edges::Sobel);

edge.apply(img, true); // bool for blurring or not
img.write("Output/sobel.png", 1);
```



### 3D Volume, Filters, Slices and Projections

```c++
#include "Volume.h"
#include "Projection.h"
#include "Slice.h"
#include "Conv3DFilter.h"

// read in volume with default args
Volume vol("Scans/fracture", "../Output/Scans/fracture/f_");

// 3D filter
int kernel_size = 5;
Conv3DFilter filter(Conv3DFilter::gaussian_separated, kernel_size);
filter.apply(vol);

// now median projection
Projection proj(Projection::median);
Image img = proj.execute(vol);
img.write("Output/median_projection.png", 3);

// slice in the x-direction
Slice slicer();
Matrix<uint8_t> slice = slicer.generate_slice(vol, idx1, Slice::X);
slicer.write_slice(slice, "Output/x_slice.png");
```


# 3D Volume Data: CT Scans
Download CT Scan datasets here:
https://imperiallondon-my.sharepoint.com/:u:/g/personal/tmd02_ic_ac_uk/EafXMuNsbcNGnRpa8K62FjkBvIKvCswl1riz7hPDHpHdSQ
