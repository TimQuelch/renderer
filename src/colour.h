#pragma once

#include <Eigen/Core>
#include <cstdint>

namespace renderer {
    using Colour = Eigen::Vector3f;
    using Pixel = Eigen::Matrix<std::uint8_t, 3, 1>;

    [[nodiscard]] auto toPixel(Colour const& colour) -> Pixel;
}
