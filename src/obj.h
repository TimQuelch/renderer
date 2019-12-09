#pragma once

#include <filesystem>

#include "scene.h"

namespace renderer {
    [[nodiscard]] auto loadObj(std::filesystem::path const& objFile,
                               Colour const& background,
                               Material const& material) -> Scene;
} // namespace renderer
