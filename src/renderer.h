#pragma once

namespace renderer {
    class Scene;
    class Camera;

    struct RenderParams {
        int width;
        int height;
        int maxDepth;
        int nSamples;
    };

    auto render(Scene const& scene, Camera const& camera, RenderParams const& params);
} // namespace renderer
