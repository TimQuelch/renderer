#include "camera.h"

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
        , axis_{detail::getAxis(eye, lookAt, up)}
        , width_{width}
        , height_{height}
        , planeDist_{0.5 / tan(degToRad(fov) / 2)}
        , aspectRatio_{static_cast<double>(width) / height} {}

    [[nodiscard]] auto Camera::rayThroughPixel(int x, int y) const -> Ray {
        if (x < width_ || y < height_) {
            throw std::invalid_argument{fmt::format(
                "Requested ray ({}, {}) is out of camera bounds ({}, {})", x, y, width_, height_)};
        }

        auto xComp = axis_.x * aspectRatio_ * ((x + 0.5) / width_ - 0.5);
        auto yComp = axis_.y * ((y + 0.5) / height_ - 0.5);
        auto zComp = axis_.z * planeDist_;
        auto direction = xComp + yComp + zComp;

        return {eye_, direction.normalized()};
    }
} // namespace renderer
