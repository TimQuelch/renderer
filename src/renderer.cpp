#include "renderer.h"

#include "camera.h"
#include "math-utils.h"
#include "scene.h"

namespace renderer {
    namespace {
        [[nodiscard, maybe_unused]] auto generateRay(Intersection const& intersection, [[maybe_unused]] Rng& rng) -> Ray {

            auto direction = Vec{};
            return {intersection.position, direction.normalized()};
        }

        [[nodiscard]] auto castRay(Ray const& ray,
                                   Scene const& scene,
                                   RenderParams const& params,
                                   [[maybe_unused]] Rng& rng,
                                   int depth = 0) -> Colour {
            if (depth > params.maxDepth) {
                return scene.background();
            }

            if (auto intersection = scene.intersect(ray)) {
                // auto colour = Colour{};
                // for (auto i = 0; i < params.nSamples; i++) {
                //     // Generate new rays
                //     // Cast rays
                //     // Accumulate results
                // }

                return intersection->material.diffusion;
            } else {
                return scene.background();
            }
        }
    } // namespace

    auto render(Scene const& scene, Camera const& camera, RenderParams const& params, Rng& rng)
        -> Frame {
        auto frame = Frame{camera.width(), camera.height()};

        for (auto y = 0; y < camera.height(); y++) {
            for (auto x = 0; x < camera.width(); x++) {
                auto const ray = camera.rayThroughPixel(x, y);
                auto const colour = castRay(ray, scene, params, rng);
                frame.at(x, y) = colour;
            }
        }

        return frame;
    }
} // namespace renderer
