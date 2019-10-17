#pragma once

#include <Eigen/Core>

namespace renderer {
    using Colour = Eigen::Vector3f;

    struct Material {
        Colour emission;
        Colour diffusion;
    };
}
