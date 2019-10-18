#pragma once

#include <optional>

#include "intersection.h"

namespace renderer {
    class Primative {

    public:
        virtual ~Primative() = default;

        [[nodiscard]] virtual auto intersect(Ray const& ray) const noexcept
            -> std::optional<Intersection>;
    };
} // namespace renderer
