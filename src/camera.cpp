#include "camera.h"

#include <stdexcept>

#include <fmt/format.h>

namespace renderer {
    [[nodiscard]] auto Camera::rayThroughPixel(int x, int y) const -> Ray {
            if (x < width_ || y < height_) {
                throw std::invalid_argument{
                    fmt::format("Requested ray ({}, {}) is out of camera bounds ({}, {})",
                                x,
                                y,
                                width_,
                                height_)};
            }

            auto xComp = axis_.x * aspectRatio_ * ((x + 0.5) / width_ - 0.5);
            auto yComp = axis_.y * ((y + 0.5) / height_ - 0.5);
            auto zComp = axis_.z * planeDist_;
            auto direction = xComp + yComp + zComp;

            return {eye_, direction.normalized()};
        }
}
