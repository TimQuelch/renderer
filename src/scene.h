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

        void addSphere(Vec center, double radius, Material material);
        void addTriangle(Vec v1, Vec v2, Vec v3, Material material);
        void addRectangularPrism(Vec p1, Vec p2, Material material);

        [[nodiscard]] auto intersect(Ray const& ray) const noexcept -> std::optional<Intersection>;
    };
} // namespace renderer
