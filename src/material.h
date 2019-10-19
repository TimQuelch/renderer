#pragma once

#include "colour.h"

namespace renderer {
    struct Material {
        Colour emission;
        Colour diffusion;
    };
}
