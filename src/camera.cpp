#include "camera.h"

#include <random>
#include <stdexcept>

#include <fmt/format.h>

namespace renderer {
    Camera::Camera(Vec const& eye, Vec const& lookAt, Vec const& up, double fov)
        : eye_{eye}
        , axis_{Axis::fromZY(lookAt - eye, up)}
        , planeDist_{0.5 / tan(degToRad(fov) / 2)} {}

    auto Camera::rayThroughPixel(int x, int y, int width, int height, Rng& rng) const -> Ray {
        if (x >= width || y >= height || x < 0 || y < 0) {
            throw std::invalid_argument{fmt::format(
                "Requested ray ({}, {}) is out of camera bounds ({}, {})", x, y, width, height)};
        }

        auto unit = std::uniform_real_distribution<double>{0, 1};

        auto const aspectRatio = static_cast<double>(width) / height;

        // y component is negative so down-right is positive in pixel coords, but up-right is
        // positive in space coords
        auto const xComp = axis_.x * aspectRatio * ((x + unit(rng)) / width - 0.5);
        auto const yComp = -axis_.y * ((y + unit(rng)) / height - 0.5);
        auto const zComp = axis_.z * planeDist_;
        auto const direction = xComp + yComp + zComp;

        return {eye_, direction.normalized()};
    } // namespace renderer
} // namespace renderer
