#pragma once
#include <string>
#include <memory>
#include <algorithm>
#include <cmath>

namespace wb {

class AbsorptionModel {
public:
    virtual ~AbsorptionModel() = default;
    virtual float absorption(float wavelength, float S) const = 0;
    virtual std::string name() const = 0;
};

class SimpleAbsorptionModel : public AbsorptionModel {
public:
    float absorption(float wavelength, float S) const override;
    std::string name() const override { return "Simple"; }
};

std::shared_ptr<AbsorptionModel> createDefaultModel();

} 
