#include "scene.h"
#include <optional>

namespace renderer {
    auto Scene::intersect(Ray const& ray) const noexcept -> std::optional<Intersection> {
        return std::nullopt;
    }
}
