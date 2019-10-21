#include <random>

#include "camera.h"
#include "png-write.h"
#include "renderer.h"
#include "scene.h"

using namespace renderer;

int main() {
    auto rng = std::mt19937{std::random_device{}()};
    auto scene = Scene{{0.9, 0.9, 0.9}};
    auto camera = Camera{{0, -1.5, -8}, {0, 0, 0}, {0, 1, 0}, 40.0, 858, 480};

    // scene.addSphere({0, -1000, -600}, 1000.0, {{1, 1, 1}, {0.0, 0.0, 0.0}});
    // scene.addSphere({0, 0, 0}, 1.0, {{0, 0, 0}, {0.9, 0.9, 0.9}});
    // scene.addSphere({2.0, 0.0, -0.3}, 1.0, {{0, 0, 0}, {0.3, 0.3, 0.8}});
    scene.addSphere({-2.0, 0.0, -0.3}, 1.0, {{0, 0, 0}, {0.3, 0.8, 0.3}});
    // scene.addSphere({-1.0, 0.7, -2}, 0.3, {{5, 5, 5}, {0, 0, 0}});
    scene.addTriangle({0, 1.2, -50}, {-50, 1.0, 50}, {50, 1.0, 50}, {{0, 0, 0}, {0.8, 0.6, 0.6}});
    scene.addRectangularPrism({3.5, 1, 2}, {3.7, -1, -2.5}, {{0, 0, 0}, {0.6, 0.6, 0.6}});

    auto renderParams = RenderParams{"out.png", 2, 4};
    auto image = render(scene, camera, renderParams, rng);

    writePng(renderParams.outputFile, image);

    return 0;
}
