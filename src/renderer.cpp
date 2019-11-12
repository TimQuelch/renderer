#include "renderer.h"

#include <atomic>
#include <chrono>
#include <cmath>
#include <random>
#include <thread>

#include <fmt/format.h>

#include "camera.h"
#include "math-utils.h"
#include "scene.h"

namespace renderer {
    namespace {
        class Progress {
        private:
            int total_;
            std::atomic<int> current_ = 0;
            std::atomic<bool> finished_ = false;
            std::thread printThread;

            template <typename TimePoint>
            auto elapsed(TimePoint start) {
                return std::chrono::duration<double>{std::chrono::steady_clock::now() - start};
            }

            template <typename TimePoint>
            auto eta(TimePoint start) {
                auto const currentElapsed = elapsed(start);
                using dur = decltype(currentElapsed);
                auto const rate = current() / currentElapsed.count();
                return dur(static_cast<typename dur::rep>((total() - current()) / rate));
            }

        public:
            Progress(int total)
                : total_{total} {
                printThread = std::thread{[&]() {
                    auto const start = std::chrono::steady_clock::now();
                    while (!finished_) {
                        fmt::print("{:.1f}% ({}/{} px). Elapsed {:.2f}s. Remaining {:.2f}s\n",
                                   percentage(),
                                   current(),
                                   Progress::total(),
                                   elapsed(start).count(),
                                   eta(start).count());
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }
                }};
            }

            Progress() = delete;
            Progress(Progress const&) = delete;
            Progress(Progress&&) = delete;
            Progress& operator=(Progress const&) = delete;
            Progress& operator=(Progress&&) = delete;

            ~Progress() {
                finished_ = true;
                printThread.join();
            }

            void increment() { current_++; };

            [[nodiscard]] int current() const noexcept { return current_; }
            [[nodiscard]] int total() const noexcept { return total_; }
            [[nodiscard]] double percentage() const noexcept {
                return static_cast<double>(current()) / total() * 100.0;
            }
        };

        [[nodiscard]] auto generateRayDiffuse(Intersection const& intersection, Rng& rng) {
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

            return std::pair{Ray{intersection.position, direction}, cosTheta};
        }

        [[nodiscard]] auto
        generateRayReflective(Intersection const& intersection, Ray const& incoming, Rng& rng) {
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

            return std::pair{Ray{intersection.position, direction}, cosTheta};
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
                auto const r = std::uniform_real_distribution<double>{0, 1}(rng);
                if (r < intersection->material.reflectionIndex) {
                    auto const [newRay, cosTheta] = generateRayReflective(*intersection, ray, rng);
                    incoming += cosTheta * castRay(newRay, scene, params, rng, depth + 1);
                } else {
                    auto const [newRay, cosTheta] = generateRayDiffuse(*intersection, rng);
                    incoming += cosTheta * castRay(newRay, scene, params, rng, depth + 1);
                }
            }
            incoming /= nSamples;

            return intersection->material.emission +
                   incoming.cwiseProduct(intersection->material.diffusion);
        }
    } // namespace

    auto render(Scene const& scene, Camera const& camera, RenderParams const& params, Rng& rng)
        -> Frame {
        auto frame = Frame{params.width, params.height};

        auto progress = Progress{params.width * params.height};

#pragma omp parallel for schedule(dynamic, 1)
        for (auto y = 0; y < params.height; y++) {
            for (auto x = 0; x < params.width; x++) {
                frame.at(x, y) = {0, 0, 0};
                for (int i = 0; i < params.nSamples; i++) {

                    auto const ray = camera.rayThroughPixel(x, y, params.width, params.height, rng);
                    auto const colour = castRay(ray, scene, params, rng);
                    frame.at(x, y) += colour / params.nSamples;
                }
                progress.increment();
            }
        }

        return frame;
    }
} // namespace renderer
