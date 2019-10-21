#include "camera.h"

#include <random>
#include <stdexcept>

#include <fmt/format.h>

namespace renderer {
    Camera::Camera(Vec const& eye,
                   Vec const& lookAt,
                   Vec const& up,
                   double fov,
                   int width,
                   int height)
        : eye_{eye}
        , axis_{Axis::fromZY(lookAt - eye, up)}
        , width_{width}
        , height_{height}
        , planeDist_{0.5 / tan(degToRad(fov) / 2)}
        , aspectRatio_{static_cast<double>(width) / height} {}

    [[nodiscard]] auto Camera::rayThroughPixel(int x, int y, Rng& rng) const -> Ray {
        if (x >= width_ || y >= height_ || x < 0 || y < 0) {
            throw std::invalid_argument{fmt::format(
                "Requested ray ({}, {}) is out of camera bounds ({}, {})", x, y, width_, height_)};
        }

        auto unit = std::uniform_real_distribution<double>{0, 1};

        // y component is negative so down-right is positive in pixel coords, but up-right is
        // positive in space coords
        auto const xComp = axis_.x * aspectRatio_ * ((x + unit(rng)) / width_ - 0.5);
        auto const yComp = -axis_.y * ((y + unit(rng)) / height_ - 0.5);
        auto const zComp = axis_.z * planeDist_;
        auto const direction = xComp + yComp + zComp;

        return {eye_, direction.normalized()};
    }
} // namespace renderer
