#include "renderer.h"

#include "camera.h"
#include "math-utils.h"
#include "scene.h"

namespace renderer {
    namespace {
        [[nodiscard]] auto generateRay(Intersection const& intersection, Rng& rng) -> Ray {

            auto direction = Vec{};
            return {intersection.position, direction.normalized()};
        }

        [[nodiscard]] auto castRay(Ray const& ray,
                                   Scene const& scene,
                                   RenderParams const& params,
                                   Rng& rng,
                                   int depth = 0) -> Colour {
            if (depth > params.maxDepth) {
                return scene.background();
            }

            if (auto intersection = scene.intersect(ray)) {
                auto colour = Colour{};
                for (auto i = 0; i < params.nSamples; i++) {
                    // Generate new rays
                    // Cast rays
                    // Accumulate results
                }

                return colour;
            } else {
                return scene.background();
            }
        }
    } // namespace

    auto render(Scene const& scene, Camera const& camera, RenderParams const& params, Rng& rng) {
        auto frame = std::vector<std::vector<Colour>>{};
        frame.reserve(camera.height());
        for (auto y = 0; y < camera.height(); y++) {
            auto row = std::vector<Colour>{};
            row.reserve(camera.width());
            for (auto x = 0; x < camera.width(); x++) {
                auto const ray = camera.rayThroughPixel(x, y);
                auto const colour = castRay(ray, scene, params, rng);
                row.push_back(colour);
            }
            frame.push_back(row);
        }
        return frame;
    }
} // namespace renderer
