#pragma once

#include "colour.h"

namespace renderer {
    struct Material {
        Colour emission;
        Colour diffusion;
        double reflectionIndex = {0.0};
        double reflectionConeAngle = {0.0};
    };
} // namespace renderer
