#include <random>

#include "camera.h"
#include "png-write.h"
#include "renderer.h"
#include "scene.h"

using namespace renderer;

int main() {
    auto rng = std::mt19937{std::random_device{}()};
    auto scene = Scene{{0.9, 0.9, 0.9}};
    auto camera = Camera{{0, 0, -7}, {0, 0, 0}, {0, 1, 0}, 40.0, 720, 480};

    // scene.addSphere({0, -1000, -600}, 1000.0, {{1, 1, 1}, {0.0, 0.0, 0.0}});
    scene.addSphere({0, 0, 0}, 1.5, {{0, 0, 0}, {0.9, 0.9, 0.9}});
    scene.addSphere({4, 0.0, -1.0}, 2.5, {{0, 0, 0}, {0.3, 0.3, 0.8}});
    scene.addSphere({-4, 0.0, -1.0}, 2.5, {{0, 0, 0}, {0.3, 0.8, 0.3}});

    auto renderParams = RenderParams{"out.png", 10, 1024};
    auto image = render(scene, camera, renderParams, rng);

    writePng(renderParams.outputFile, image);

    return 0;
}
