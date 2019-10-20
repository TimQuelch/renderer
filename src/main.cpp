#include <random>

#include "png-write.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"

using namespace renderer;

int main () {
    auto rng = std::mt19937{std::random_device{}()};
    auto scene = Scene{{0.6, 0.6, 0.7}};
    scene.addSphere({2, 0, 0}, 0.5, {{0, 0, 0}, {0.8, 0.2, 0.2}});
    auto camera = Camera{{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, 60.0, 1920, 1080};

    auto renderParams = RenderParams{};
    auto image = render(scene, camera, renderParams, rng);

    writePng(renderParams.outputFile, image);

    return 0;
}
