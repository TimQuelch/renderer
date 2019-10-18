#pragma once

#include "material.h"
#include "vec.h"

namespace renderer {
    struct Intersection {
        Vec position;
        Vec normal;
        Material material;
    };
} // namespace renderer
