#include "renderer.h"

#include <cmath>
#include <random>

#include "camera.h"
#include "math-utils.h"
#include "scene.h"

namespace renderer {
    namespace {
        [[nodiscard]] auto generateRayDiffuse(Intersection const& intersection, Rng& rng) -> Ray {
            auto const cosTheta = std::uniform_real_distribution<float>{0, 1}(rng);
            auto const sinTheta = std::sqrt(1 - cosTheta * cosTheta);

            auto const phi = std::uniform_real_distribution<float>{0, 2 * M_PI}(rng);
            auto const x = sinTheta * std::cos(phi);
            auto const y = sinTheta * std::sin(phi);

            auto const baseDir = Vec{x, y, cosTheta};

            auto const transform = [normal = intersection.normal]() {
                auto const normalAxis = Axis::fromZ(normal);
                auto m = Eigen::Matrix3f{};
                m.col(0) = normalAxis.x;
                m.col(1) = normalAxis.y;
                m.col(2) = normalAxis.z;
                return m;
            }();

            auto const direction = transform * baseDir;

            return {intersection.position, direction};
        }

        [[nodiscard]] auto generateRayReflective(Intersection const& intersection,
                                                 Ray const& incoming,
                                                 Rng& rng) -> Ray {
            auto const cosThetaMax =
                1 + (std::cos(degToRad<float>(intersection.material.reflectionConeAngle)) - 1);
            auto const cosTheta = std::uniform_real_distribution<float>{1, cosThetaMax}(rng);
            auto const sinTheta = std::sqrt(1 - cosTheta * cosTheta);

            auto const phi = std::uniform_real_distribution<float>{0, 2 * M_PI}(rng);
            auto const x = sinTheta * std::cos(phi);
            auto const y = sinTheta * std::sin(phi);

            auto const baseDir = Vec{x, y, cosTheta};

            auto const transform = [&]() {
                auto const normalAxis = Axis::fromZ(
                    incoming.direction -
                    intersection.normal * 2 * intersection.normal.dot(incoming.direction));
                auto m = Eigen::Matrix3f{};
                m.col(0) = normalAxis.x;
                m.col(1) = normalAxis.y;
                m.col(2) = normalAxis.z;
                return m;
            }();

            auto const direction = transform * baseDir;

            return {intersection.position, direction};
        }

        [[nodiscard]] auto
        generateRay(Intersection const& intersection, Ray const& incoming, Rng& rng) {
            auto const R = intersection.material.reflectionIndex;
            auto const coneAngle = degToRad(intersection.material.reflectionConeAngle);
            auto const r = std::uniform_real_distribution<double>{0, 1}(rng);

            Ray ray = {};
            double pdf = {};

            if (r < R) {
                ray = generateRayReflective(intersection, incoming, rng);
                auto const cosTheta = static_cast<double>(ray.direction.dot(intersection.normal));
                auto const sinTheta = std::sqrt(1 - cosTheta * cosTheta);
                pdf = R * sinTheta / (2 * M_PI * (1.0 - std::cos(coneAngle)));
            } else {
                ray = generateRayDiffuse(intersection, rng);
                pdf = (1 - R) / (2 * M_PI);
            }

            return std::pair{ray, pdf};
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
                auto const [newRay, pdf] = generateRay(*intersection, ray, rng);
                incoming += newRay.direction.dot(intersection->normal) *
                            castRay(newRay, scene, params, rng, depth + 1) / pdf;
            }
            incoming /= nSamples;

            auto const brdf = 1 / M_PI;

            return intersection->material.emission +
                   incoming.cwiseProduct(intersection->material.diffusion) * brdf;
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
