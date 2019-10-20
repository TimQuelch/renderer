#include "scene.h"

#include <limits>
#include <optional>

namespace renderer {
    void Scene::addSphere(Vec center, double radius, Material material) {
        primatives_.push_back(std::make_unique<Sphere>(center, radius, material));
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
