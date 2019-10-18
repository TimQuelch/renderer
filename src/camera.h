#pragma once

#include "math-utils.h"
#include "vec.h"

namespace renderer {
    namespace detail {
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
        Camera(Vec const& eye, Vec const& lookAt, Vec const& up, double fov, int width, int height);

        [[nodiscard]] auto rayThroughPixel(int x, int y) const -> Ray;

        [[nodiscard]] auto width() const noexcept { return width_; }
        [[nodiscard]] auto height() const noexcept { return height_; }
        [[nodiscard]] auto aspectRatio() const noexcept { return aspectRatio_; }
    };
} // namespace renderer
