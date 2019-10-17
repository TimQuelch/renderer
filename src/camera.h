#pragma once

#include "math.h"
#include "vec.h"

namespace renderer {
    namespace {
        auto getAxis(Vec eye, Vec lookAt, Vec up) {
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
        double planeDist_;
        int width_;
        int height_;

    public:
        Camera(Vec eye, Vec lookAt, Vec up, double fov, int width, int height)
            : eye_{eye}
            , axis_{getAxis(eye, lookAt, up)}
            , planeDist_{0.5 / tan(degToRad(fov) / 2)}
            , width_{width}
            , height_{height} {}
    };
} // namespace renderer
