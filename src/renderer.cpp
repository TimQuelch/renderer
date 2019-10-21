#include "renderer.h"

#include <random>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "camera.h"
#include "math-utils.h"
#include "scene.h"

namespace renderer {
    namespace {
        [[nodiscard]] auto generateRay(Intersection const& intersection, Rng& rng) -> Ray {
            auto const cosTheta = std::uniform_real_distribution<float>{0, 1}(rng);
            auto const sinTheta = std::sqrt(1 - cosTheta * cosTheta);

            auto const phi = std::uniform_real_distribution<float>{0, 2 * M_PI}(rng);
            auto const x = sinTheta * std::cos(phi);
            auto const y = sinTheta * std::sin(phi);

            auto const baseDir = Vec{x, y, cosTheta};
            // fmt::print("Hemisphere vector\n{}\nNorm: {}\n", baseDir, baseDir.norm());

            auto const transform = [normal = intersection.normal]() {
                auto const normalAxis = Axis::fromZ(normal);
                auto m = Eigen::Matrix3f{};
                m.col(0) = normalAxis.x;
                m.col(1) = normalAxis.y;
                m.col(2) = normalAxis.z;
                return m;
            }();

            auto const direction = transform * baseDir;

            // fmt::print("Transformed vector\n{}\nNorm: {}\n", direction, direction.norm());

            return {intersection.position, direction};
        }

        [[nodiscard]] auto castRay(Ray const& ray,
                                   Scene const& scene,
                                   RenderParams const& params,
                                   Rng& rng,
                                   int depth = 0) -> Colour {
            if (depth > params.maxDepth) {
                return scene.background();
            }

            auto const intersection = scene.intersect(ray);

            if (!intersection) {
                return scene.background();
            }

            auto incoming = Colour{0, 0, 0};
            // auto const nSamples = std::max(1, params.nSamples / ipow(2, depth));
            auto const nSamples = 1;
            for (auto i = 0; i < nSamples; i++) {
                auto const newRay = generateRay(*intersection, rng);
                auto const pdf = 1 / (2 * M_PI);
                incoming += newRay.direction.dot(intersection->normal) *
                            castRay(newRay, scene, params, rng, depth + 1) / pdf;
            }
            incoming /= nSamples;

            return intersection->material.emission +
                   incoming.cwiseProduct(intersection->material.diffusion) / M_PI;
            // return intersection->material.emission;
        }
    } // namespace

    auto render(Scene const& scene, Camera const& camera, RenderParams const& params, Rng& rng)
        -> Frame {
        auto frame = Frame{camera.width(), camera.height()};

        for (auto y = 0; y < camera.height(); y++) {
            for (auto x = 0; x < camera.width(); x++) {
                frame.at(x, y) = {0, 0, 0};
                for (int i = 0; i < params.nSamples; i++) {

                    auto const ray = camera.rayThroughPixel(x, y, rng);
                    auto const colour = castRay(ray, scene, params, rng);
                    frame.at(x, y) += colour / params.nSamples;
                }
            }
        }

        return frame;
    }
} // namespace renderer
