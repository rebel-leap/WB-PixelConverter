#include "WBConverter.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace wb {

WBConverter::WBConverter() : model_(createDefaultModel()) {}
WBConverter::WBConverter(std::shared_ptr<AbsorptionModel> model) : model_(std::move(model)) {}

RGB WBConverter::computeWBColor(float S) const {
    float R = 0, G = 0, B = 0;
    float refR = 0, refG = 0, refB = 0;

    for (int i = 0; i < WAVELENGTH_COUNT; ++i) {
        int wl = WAVELENGTH_MIN + i * WAVELENGTH_STEP;
        RGB rgb = wavelengthToRGB(wl);
        float abs = model_->absorption(float(wl), S);

        R += rgb.r * (1.0f - abs);
        G += rgb.g * (1.0f - abs);
        B += rgb.b * (1.0f - abs);

        refR += rgb.r;
        refG += rgb.g;
        refB += rgb.b;
    }

    if (refR > 0.0f) R /= refR;
    if (refG > 0.0f) G /= refG;
    if (refB > 0.0f) B /= refB;

    if (S >= 0.999f) {
        return {0.0f, 0.0f, 0.0f};
    }

    R = std::clamp(R, 0.0f, 1.0f);
    G = std::clamp(G, 0.0f, 1.0f);
    B = std::clamp(B, 0.0f, 1.0f);

    return {R, G, B};
}

float WBConverter::rgbToS(uint8_t r, uint8_t g, uint8_t b) const {
    float tR = r / 255.0f, tG = g / 255.0f, tB = b / 255.0f;
    if (tR > 0.99f && tG > 0.99f && tB > 0.99f) return 0.0f;
    if (tR < 0.01f && tG < 0.01f && tB < 0.01f) return 1.0f;

    float bestS = 0.0f, bestErr = std::numeric_limits<float>::max();
    for (int i = 0; i <= 100; ++i) {
        float S = i / 100.0f;
        RGB wb = computeWBColor(S);
        float err = (wb.r - tR)*(wb.r - tR) + (wb.g - tG)*(wb.g - tG) + (wb.b - tB)*(wb.b - tB);
        if (err < bestErr) { bestErr = err; bestS = S; }
    }
    return bestS;
}

void WBConverter::sToRgb(float S, uint8_t& r, uint8_t& g, uint8_t& b) const {
    RGB wb = computeWBColor(S);
    r = uint8_t(std::clamp(wb.r * 255.0f, 0.0f, 255.0f));
    g = uint8_t(std::clamp(wb.g * 255.0f, 0.0f, 255.0f));
    b = uint8_t(std::clamp(wb.b * 255.0f, 0.0f, 255.0f));
}

void WBConverter::convertBufferCPU(const uint8_t* input, uint8_t* output, int width, int height) const {
    int total = width * height;
    for (int i = 0; i < total; ++i) {
        float S = rgbToS(input[i*3], input[i*3+1], input[i*3+2]);
        sToRgb(S, output[i*3], output[i*3+1], output[i*3+2]);
    }
}

} 