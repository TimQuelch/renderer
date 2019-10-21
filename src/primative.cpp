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

    auto Triangle::intersect(Ray const& ray) const noexcept -> std::optional<Intersection> {
        auto const h = ray.direction.cross(vVec_);
        auto const a = uVec_.dot(h);

        if (std::abs(a) < epsilon) {
            return std::nullopt;
        }

        auto const backfacing = a < epsilon;

        auto const f = 1 / a;
        auto const s = ray.origin - vertices_[0];
        auto const u = f * s.dot(h);

        if (u < 0.0F || u > 1.0F) {
            return std::nullopt;
        }

        auto const q = s.cross(uVec_);
        auto const v = f * ray.direction.dot(q);

        if (v < 0.0F || u + v > 1.0F) {
            return std::nullopt;
        }

        auto const d = f * vVec_.dot(q);
        if (d < epsilon) {
            return std::nullopt;
        }

        auto const position = ray.origin + d * ray.direction;

        return Intersection{position, backfacing ? -normal_ : normal_, material()};
    }

    RectangularPrism::RectangularPrism(Vec p1, Vec p2, Material const& material)
        : Primative{material} {
        triangles_.reserve(12);

        auto const addTri = [this, material](Vec const& v1, Vec const& v2, Vec const& v3) {
            triangles_.push_back(std::make_unique<Triangle>(v1, v2, v3, material));
        };

        addTri({p1[0], p1[1], p1[2]}, {p2[0], p1[1], p1[2]}, {p1[0], p2[1], p1[2]});
        addTri({p2[0], p2[1], p1[2]}, {p2[0], p1[1], p1[2]}, {p1[0], p2[1], p1[2]});

        addTri({p1[0], p1[1], p1[2]}, {p2[0], p1[1], p1[2]}, {p1[0], p1[1], p2[2]});
        addTri({p2[0], p1[1], p2[2]}, {p2[0], p1[1], p1[2]}, {p1[0], p1[1], p2[2]});

        addTri({p1[0], p1[1], p1[2]}, {p1[0], p1[1], p2[2]}, {p1[0], p2[1], p1[2]});
        addTri({p1[0], p2[1], p2[2]}, {p1[0], p1[1], p2[2]}, {p1[0], p2[1], p1[2]});

        addTri({p2[0], p2[1], p2[2]}, {p1[0], p2[1], p2[2]}, {p2[0], p1[1], p2[2]});
        addTri({p1[0], p1[1], p2[2]}, {p1[0], p2[1], p2[2]}, {p2[0], p1[1], p2[2]});

        addTri({p2[0], p2[1], p2[2]}, {p1[0], p2[1], p2[2]}, {p2[0], p2[1], p1[2]});
        addTri({p1[0], p2[1], p1[2]}, {p1[0], p2[1], p2[2]}, {p2[0], p2[1], p1[2]});

        addTri({p2[0], p2[1], p2[2]}, {p2[0], p2[1], p1[2]}, {p2[0], p1[1], p2[2]});
        addTri({p2[0], p1[1], p1[2]}, {p2[0], p2[1], p1[2]}, {p2[0], p1[1], p2[2]});
    }

    auto RectangularPrism::intersect(Ray const& ray) const noexcept -> std::optional<Intersection> {

        auto closest = std::optional<Intersection>{};
        auto dClosest = std::numeric_limits<float>::max();

        for (auto const& t : triangles_) {
            if (auto intersection = t->intersect(ray)) {
                auto const d = (intersection->position - ray.origin).norm();
                if (d < dClosest) {
                    closest = intersection;
                    dClosest = d;
                }
            }
        }

        return closest;
    }
} // namespace renderer
