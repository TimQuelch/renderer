#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace renderer {
    using Vec = Eigen::Vector3f;

    struct Axis {
        Vec x;
        Vec y;
        Vec z;

        static Axis fromZ(Vec const& z) noexcept {
            auto ax = Axis{};
            ax.z = z.normalized();

            auto const other = [&]() {
                auto const xAxis = Vec{1, 0, 0};
                auto const yAxis = Vec{0, 1, 0};
                if (std::fabs(ax.z.dot(xAxis)) > 0.99f) {
                    return yAxis;
                }
                return xAxis;
            }();

            ax.z = z.normalized();
            ax.x = other.cross(ax.z).normalized();
            ax.y = ax.z.cross(ax.x);
            return ax;
        }

        static Axis fromZY(Vec const& z, Vec const& y) noexcept {
            auto ax = Axis{};
            ax.z = z.normalized();
            ax.x = y.cross(ax.z).normalized();
            ax.y = ax.z.cross(ax.x);
            return ax;
        }
    };

    struct Ray {
        Vec origin;
        Vec direction;
    };
} // namespace renderer
