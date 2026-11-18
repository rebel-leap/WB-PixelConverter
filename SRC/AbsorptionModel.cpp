#include "AbsorptionModel.h"
#include <algorithm>
#include <cmath>

namespace wb {

static float gaussian(float x, float center, float width) {
    float d = (x - center) / width;
    return std::exp(-0.5f * d * d);
}

float SimpleAbsorptionModel::absorption(float wavelength, float S) const {

    float wl = wavelength;

    float blueAbs = gaussian(wl, 450.0f, 60.0f);
    float blueS = std::clamp(S * 3.0f, 0.0f, 1.0f); 

    float greenAbs = gaussian(wl, 550.0f, 70.0f);
    float greenS = std::clamp((S - 0.3f) * 3.0f, 0.0f, 1.0f); 

    float redAbs = gaussian(wl, 650.0f, 80.0f);
    float redS = std::clamp((S - 0.6f) * 3.0f, 0.0f, 1.0f); 

    float transmittance =
        (1.0f - blueAbs * blueS) *
        (1.0f - greenAbs * greenS) *
        (1.0f - redAbs * redS);

    return std::clamp(1.0f - transmittance, 0.0f, 1.0f);
}


std::shared_ptr<AbsorptionModel> createDefaultModel() {
    return std::make_shared<SimpleAbsorptionModel>();
}

}
