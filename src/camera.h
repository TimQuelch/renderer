#pragma once

#include "math-utils.h"
#include "vec.h"

namespace renderer {
    namespace {
        auto getAxis(Vec const& eye, Vec const& lookAt, Vec const& up) {
            auto ax = Axis{};
            ax.z = (lookAt - eye).normalized();
            ax.x = up.cross(ax.z).normalized();
            ax.y = ax.z.cross(ax.x);
            return ax;
        }
    } // namespace

    class Camera {
    private:
        Vec eye_;
        Axis axis_;
        int width_;
        int height_;
        double planeDist_;
        double aspectRatio_;

    public:
        Camera(Vec const& eye, Vec const& lookAt, Vec const& up, double fov, int width, int height)
            : eye_{eye}
            , axis_{getAxis(eye, lookAt, up)}
            , width_{width}
            , height_{height}
            , planeDist_{0.5 / tan(degToRad(fov) / 2)}
            , aspectRatio_{static_cast<double>(width) / height} {}

        [[nodiscard]] auto rayThroughPixel(int x, int y) const -> Ray;
   };
} // namespace renderer
