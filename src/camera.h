#pragma once

#include "math-utils.h"
#include "vec.h"

namespace renderer {
    class Camera {
    private:
        Vec eye_;
        Axis axis_;
        double planeDist_;

    public:
        Camera(Vec const& eye, Vec const& lookAt, Vec const& up, double fov);

        [[nodiscard]] auto rayThroughPixel(int x, int y, int width, int height, Rng& rng) const
            -> Ray;
    };
} // namespace renderer
