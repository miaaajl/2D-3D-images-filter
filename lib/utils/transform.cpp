#include "utils/transform.h"
#include <tuple>      // For std::tuple
#include <algorithm>  // For std::max and std::min
#include <cstdint>    // For uint8_t
#include <cmath>

std::tuple<float, float, float> convertToHSV(uint8_t r, uint8_t g, uint8_t b) {
    float fr = r / 255.0f;
    float fg = g / 255.0f;
    float fb = b / 255.0f;

    float max = std::max({fr, fg, fb});
    float min = std::min({fr, fg, fb});
    float d = max - min;

    // Hue calculation
    float h;
    if (d == 0) h = 0;
    else if (max == fr) h = 60 * (fmod(((fg - fb) / d), 6));
    else if (max == fg) h = 60 * ((fb - fr) / d + 2);
    else h = 60 * ((fr - fg) / d + 4);

    // Saturation calculation
    float s = max == 0 ? 0 : d / max;

    // Value calculation
    float v = max;

    return {h, s, v};
}

std::tuple<uint8_t, uint8_t, uint8_t> convertFromHSV(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;
    float r_prime, g_prime, b_prime;

    if (h >= 0 && h < 60) {
        r_prime = c; g_prime = x; b_prime = 0;
    } else if (h >= 60 && h < 120) {
        r_prime = x; g_prime = c; b_prime = 0;
    }else if (h >= 120 && h < 180) {
        r_prime = 0; g_prime = c; b_prime = x;
    } else if (h >= 180 && h < 240) {
        r_prime = 0; g_prime = x; b_prime = c;
    } else if (h >= 240 && h < 300) {
        r_prime = x; g_prime = 0; b_prime = c;
    } else { // h >= 300 && h < 360
        r_prime = c; g_prime = 0; b_prime = x;
    }

    return {static_cast<uint8_t>((r_prime + m) * 255), static_cast<uint8_t>((g_prime + m) * 255), static_cast<uint8_t>((b_prime + m) * 255)};
}

std::tuple<float, float, float> convertToHSL(uint8_t r, uint8_t g, uint8_t b) {
    float fr = r / 255.0f;
    float fg = g / 255.0f;
    float fb = b / 255.0f;

    float max1 = std::max(fr, fg); // Finds the max of the first two
    float max = std::max(max1, fb); // Finds the max between max1 and the third value

    float min1 = std::min(fr, fg); // Finds the max of the first two
    float min = std::min(min1, fb); // Finds the max between max1 and the third value

    float h, s, l;
    h = s = l = (max + min) / 2.0f;

    if (max == min) {
        h = s = 0; // achromatic
    } else {
        float d = max - min;
        s = (l > 0.5f) ? d / (2.0f - max - min) : d / (max + min);
        if (max == fr) {
            h = (fg - fb) / d + (fg < fb ? 6 : 0);
        } else if (max == fg) {
            h = (fb - fr) / d + 2;
        } else if (max == fb) {
            h = (fr - fg) / d + 4;
        }
        h /= 6.0f;
    }

    return {h, s, l};
}

std::tuple<uint8_t, uint8_t, uint8_t> convertFromHSL(float h, float s, float l) {
    auto hue2rgb = [](float p, float q, float t) {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    };

    float r, g, b;

    if (s == 0) {
        r = g = b = l; // achromatic
    } else {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3.0f);
    }

    return {static_cast<uint8_t>(r * 255.0f), static_cast<uint8_t>(g * 255.0f), static_cast<uint8_t>(b * 255.0f)};
}