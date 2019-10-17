#pragma once

#include <cmath>

namespace renderer {
    template <typename T>
    [[nodiscard]] constexpr T degToRad(T deg) noexcept {
        return M_PI * deg / 180.0;
    }

    template <typename T>
    [[nodiscard]] constexpr T radToDeg(T rad) noexcept {
        return rad * 180.0 / M_PI;
    }
} // namespace renderer
