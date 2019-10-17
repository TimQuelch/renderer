#pragma once

#include <memory>

#include "material.h"
#include "primative.h"

namespace renderer {
    class Scene {
        std::vector<std::unique_ptr<Primative>> primatives_;
        Colour background_;

    public:
        Scene(Colour background)
            : background_{background} {}
    };
} // namespace renderer
