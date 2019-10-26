#pragma once

#include <algorithm>
#include <memory>
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

    class Triangle : public Primative {
    private:
        std::array<Vec, 3> vertices_;
        Vec uVec_;
        Vec vVec_;
        Vec normal_;

    public:
        Triangle(std::array<Vec, 3> const& vertices, Material material)
            : Primative{std::move(material)}
            , vertices_{vertices}
            , uVec_{vertices[1] - vertices[0]}
            , vVec_{vertices[2] - vertices[0]}
            , normal_{uVec_.cross(vVec_).normalized()} {}

        Triangle(Vec v1, Vec v2, Vec v3, Material material)
            : Triangle{std::array{std::move(v1), std::move(v2), std::move(v3)},
                       std::move(material)} {}

        [[nodiscard]] auto vertices() const noexcept { return vertices_; }

        [[nodiscard]] auto intersect(Ray const& ray) const noexcept
            -> std::optional<Intersection> override;
    };

    class TriangleMesh : public Primative {
    private:
        std::vector<std::unique_ptr<Triangle>> triangles_ = {};

    protected:
        void addTriangle(Vec const& v1, Vec const& v2, Vec const& v3);

    public:
        TriangleMesh(Material const& material)
            : Primative{material} {}

        [[nodiscard]] auto intersect(Ray const& ray) const noexcept
            -> std::optional<Intersection> override;
    };

    class RectangularPlane : public TriangleMesh {
    public:
        RectangularPlane(Vec p1, Vec p2, Vec p3, Vec p4, Material const& material);
    };

    class RectangularPrism : public TriangleMesh {
    public:
        RectangularPrism(Vec p1, Vec p2, Material const& material);
    };
} // namespace renderer
