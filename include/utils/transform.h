#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <tuple>
#include <cstdint> 

// Converts an RGB color to HSV.
// RGB values are in the range [0, 255]. H is in [0, 360], S and V are in [0, 1].
std::tuple<float, float, float> convertToHSV(uint8_t r, uint8_t g, uint8_t b);
// Converts an HSV color back to RGB.
std::tuple<uint8_t, uint8_t, uint8_t> convertFromHSV(float h, float s, float v);

// Converts an RGB color to HSL.
// RGB values are in the range [0, 255]. H is in [0, 360], S and L are in [0, 1].
std::tuple<float, float, float> convertToHSL(uint8_t r, uint8_t g, uint8_t b);
// Converts an HSL color back to RGB.
std::tuple<uint8_t, uint8_t, uint8_t> convertFromHSL(float h, float s, float l);

#endif // TRANSFORM_H
