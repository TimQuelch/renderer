#pragma once

#include <filesystem>

#include "math-utils.h"
#include "frame.h"

namespace renderer {
    class Scene;
    class Camera;
    class Frame;

    struct RenderParams {
        std::filesystem::path outputFile = "out.png";
        int maxDepth = {1};
        int nSamples = {1};
    };

    [[nodiscard]] auto
    render(Scene const& scene, Camera const& camera, RenderParams const& params, Rng& rng) -> Frame;
} // namespace renderer
