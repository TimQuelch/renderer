#pragma once

#include "colour.h"

namespace renderer {
    struct Material {
        Colour emission;
        Colour diffusion;
        float reflectionIndex = {0.0};
        float reflectionConeAngle = {0.0};
    };
} // namespace renderer
