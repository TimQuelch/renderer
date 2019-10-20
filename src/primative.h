#pragma once

#include <optional>

#include "intersection.h"
#include "material.h"

namespace renderer {
    class Primative {
    private:
        Material material_;

    public:
        Primative(Material material)
            : material_{std::move(material)} {}

        virtual ~Primative() = default;

        [[nodiscard]] auto material() const noexcept { return material_; }

        [[nodiscard]] virtual auto intersect(Ray const& ray) const noexcept
            -> std::optional<Intersection> = 0;
    };

    class Sphere : public Primative {
    private:
        Vec center_;
        double radius_;

    public:
        Sphere(Vec center, double radius, Material material)
            : Primative{std::move(material)}
            , center_{std::move(center)}
            , radius_{radius} {}

        [[nodiscard]] auto center() const noexcept { return center_; }
        [[nodiscard]] auto radius() const noexcept { return radius_; }

        [[nodiscard]] auto intersect(Ray const& ray) const noexcept
            -> std::optional<Intersection> override;
    };
} // namespace renderer
