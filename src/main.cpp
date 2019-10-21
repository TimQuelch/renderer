#include <random>

#include "camera.h"
#include "png-write.h"
#include "renderer.h"
#include "scene.h"

using namespace renderer;

int main() {
    auto rng = std::mt19937{std::random_device{}()};
    auto const camera = Camera{{0, 3, -8}, {0, 0, 0}, {0, 1, 0}, 40.0, 858, 480};

    auto const scene = []() {
        auto s = Scene{{0.9, 0.9, 0.9}};
        s.addSphere({0, 1, 0}, 1.0, {{0, 0, 0}, {0.9, 0.9, 0.9}});
        s.addSphere({2.0, 1, -0.3}, 1.0, {{0, 0, 0}, {0.3, 0.3, 0.8}});
        s.addSphere({-2.0, 1, -0.3}, 1.0, {{0, 0, 0}, {0.3, 0.8, 0.3}});
        s.addSphere({-1.0, 0.3, -2}, 0.3, {{5, 5, 5}, {0, 0, 0}});
        s.addRectangularPrism({-1000, 0.0, -1000}, {1000, -0.1, 1000}, {{0, 0, 0}, {0.8, 0.6, 0.6}});
        s.addRectangularPrism({2.5, 0, -2}, {2.0, 0.5, -1.5}, {{0, 0, 0}, {0.6, 0.6, 0.6}});
        return s;
    }();

    auto const renderParams = RenderParams{"out.png", 5, 1024};
    auto const image = render(scene, camera, renderParams, rng);

    writePng(renderParams.outputFile, image);

    return 0;
}
