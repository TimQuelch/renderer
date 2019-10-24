#pragma once

#include <cmath>
#include <random>

namespace renderer {
    using Rng = std::mt19937;

    constexpr float epsilon = 0.001;

    template <typename T>
    [[nodiscard]] constexpr T degToRad(T deg) noexcept {
        return static_cast<T>(M_PI) * deg / static_cast<T>(180.0);
    }

    template <typename T>
    [[nodiscard]] constexpr T radToDeg(T rad) noexcept {
        return rad * static_cast<T>(180.0) / static_cast<T>(M_PI);
    }

    template <typename T>
    [[nodiscard]] constexpr T ipow(T x, T n) {
        T v = 1;
        for (int i = 0; i < n; i++) {
            v *= x;
        }
        return v;
    }
} // namespace renderer
