#include "scene.h"

#include <limits>
#include <optional>

namespace renderer {
    void Scene::addSphere(Vec center, double radius, Material material) {
        primatives_.push_back(std::make_unique<Sphere>(std::move(center), radius, material));
    }

    void Scene::addTriangle(Vec v1, Vec v2, Vec v3, Material material) {
        primatives_.push_back(
            std::make_unique<Triangle>(std::move(v1), std::move(v2), std::move(v3), material));
    }

    void Scene::addRectangularPlane(Vec p1, Vec p2, Vec p3, Vec p4, Material material) {
        primatives_.push_back(std::make_unique<RectangularPlane>(
            std::move(p1), std::move(p2), std::move(p3), std::move(p4), material));
    }

    void Scene::addRectangularPrism(Vec p1, Vec p2, Material material) {
        primatives_.push_back(
            std::make_unique<RectangularPrism>(std::move(p1), std::move(p2), material));
    }

    auto Scene::intersect(Ray const& ray) const noexcept -> std::optional<Intersection> {
        auto closest = std::optional<Intersection>{};
        auto dClosest = std::numeric_limits<float>::max();

        for (auto const& p : primatives_) {
            if (auto intersection = p->intersect(ray)) {
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
