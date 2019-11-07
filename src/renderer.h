#pragma once

#include <filesystem>

#include "math-utils.h"
#include "frame.h"

namespace renderer {
    class Scene;
    class Camera;

    struct RenderParams {
        int width = {1};
        int height = {1};
        int maxDepth = {1};
        int nSamples = {1};
        std::filesystem::path outputFile = "out.png";
    };

    [[nodiscard]] auto
    render(Scene const& scene, Camera const& camera, RenderParams const& params, Rng& rng) -> Frame;
} // namespace renderer
