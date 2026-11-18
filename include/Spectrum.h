#pragma once
#include <array>
#include <cmath>
#include <algorithm>

namespace wb {

constexpr int WAVELENGTH_MIN = 380;
constexpr int WAVELENGTH_MAX = 780;
constexpr int WAVELENGTH_STEP = 5;
constexpr int WAVELENGTH_COUNT = (WAVELENGTH_MAX - WAVELENGTH_MIN) / WAVELENGTH_STEP + 1;

struct RGB {
    float r, g, b;
};

inline RGB wavelengthToRGB(int wavelength) {
    float wl = float(wavelength);
    float R = std::exp(-std::pow((wl - 600.0f) / 80.0f, 2.0f));
    float G = std::exp(-std::pow((wl - 550.0f) / 70.0f, 2.0f));
    float B = std::exp(-std::pow((wl - 450.0f) / 60.0f, 2.0f));

    if (wl < 400.0f) {
        float att = (wl - 380.0f) / 20.0f;
        R *= att; G *= att; B *= att;
    }
    if (wl > 700.0f) {
        float att = (780.0f - wl) / 80.0f;
        R *= att; G *= att; B *= att;
    }

    float sum = R + G + B;
    if (sum > 0.0f) { R /= sum; G /= sum; B /= sum; }
    return {R, G, B};
}

}
