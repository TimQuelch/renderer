#pragma once

#include <filesystem>

namespace renderer {
    class Scene;
    class Camera;

    struct RenderParams {
        std::filesystem::path outputFile = "out.png";
        int maxDepth = {1};
        int nSamples = {1};
    };

    auto render(Scene const& scene, Camera const& camera, RenderParams const& params);
} // namespace renderer
