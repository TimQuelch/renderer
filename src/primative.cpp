#include "primative.h"

#include <optional>

#include "math-utils.h"

namespace renderer {
    auto Sphere::intersect(Ray const& ray) const noexcept -> std::optional<Intersection> {
        auto const a = ray.direction.squaredNorm();
        auto const b = 2 * ray.direction.dot(ray.origin - center_);
        auto const c = (ray.origin - center_).squaredNorm() - static_cast<float>(radius_ * radius_);

        if (b * b - 4 * a * c < 0) {
            return std::nullopt;
        }

        auto const dPos = (-b + std::sqrt(b * b - 4 * a * c)) / (2 * a);
        auto const dNeg = (-b - std::sqrt(b * b - 4 * a * c)) / (2 * a);

        if (dPos < epsilon && dNeg < epsilon) {
            return std::nullopt;
        }

        auto const d = [dPos, dNeg]() {
            if (dNeg < epsilon) {
                return dPos;
            }
            return dNeg;
        }();

        auto const position = ray.origin + d * ray.direction;
        auto const normal = (position - center_).normalized();

        return Intersection{position, normal, material()};
    }
} // namespace renderer
