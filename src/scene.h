#pragma once

#include <memory>
#include <optional>

#include "intersection.h"
#include "material.h"
#include "primative.h"

namespace renderer {
    class Scene {
        std::vector<std::unique_ptr<Primative>> primatives_;
        Colour background_;

    public:
        Scene(Colour background)
            : background_{std::move(background)} {}

        [[nodiscard]] auto background() const noexcept { return background_; }

        [[nodiscard]] auto intersect(Ray const& ray) const noexcept -> std::optional<Intersection>;
    };
} // namespace renderer
