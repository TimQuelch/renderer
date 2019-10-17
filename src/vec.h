#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace renderer {
    using Vec = Eigen::Vector3f;

    struct Axis {
        Vec x;
        Vec y;
        Vec z;
    };
}
