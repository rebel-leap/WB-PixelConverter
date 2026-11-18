#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include "AbsorptionModel.h"
#include "Spectrum.h"

namespace wb {

class WBConverter {
public:
    WBConverter();
    explicit WBConverter(std::shared_ptr<AbsorptionModel> model);

    float rgbToS(uint8_t r, uint8_t g, uint8_t b) const;
    void sToRgb(float S, uint8_t& r, uint8_t& g, uint8_t& b) const;

    void convertBufferCPU(const uint8_t* input, uint8_t* output, int width, int height) const;

    std::shared_ptr<AbsorptionModel> model() const { return model_; }
    void setModel(std::shared_ptr<AbsorptionModel> model) { model_ = model; }

private:
    std::shared_ptr<AbsorptionModel> model_;
    RGB computeWBColor(float S) const;
};

} 