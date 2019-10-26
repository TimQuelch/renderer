#include <random>

#include "camera.h"
#include "png-write.h"
#include "renderer.h"
#include "scene.h"

using namespace renderer;

int main() {
    auto rng = std::mt19937{42};
    auto const camera = Camera{{0, 2, -8}, {0, 0.5, 0}, {0, 1, 0}, 40.0, 1920, 1080};

    auto const scene = []() {
        auto s = Scene{{0.5,  0.5, 0.5}};

        // Shapes
        s.addSphere({0, 1, 0}, 1.0, {{0, 0, 0}, {0.9, 0.9, 0.9}, 1.0, 1});
        s.addSphere({2.0, 1, -0.3}, 1.0, {{0, 0, 0}, {0.3, 0.3, 0.8}, 0.8, 20});
        s.addSphere({-2.0, 1, -0.3}, 1.0, {{0, 0, 0}, {0.3, 0.8, 0.3}});
        s.addRectangularPrism(
            {2.5, 0, -2}, {2.0, 0.5, -1.5}, {{0, 0, 0}, {0.9, 0.9, 0.9}, 0.3, 10});

        // Lights
        s.addSphere({-1.0, 0.3, -3}, 0.3, {{10, 10, 10}, {0, 0, 0}});
        s.addRectangularPrism({-1.5, 2.98, -2.0}, {1.5, 3, 1.0}, {{10, 10, 10}, {0, 0, 0}});

        // Floor, walls, and ceiling
        s.addRectangularPlane({-5, 0, -5},
                              {5, 0, -5},
                              {5, 0, 5},
                              {-5, 0, 5},
                              {{0, 0, 0}, {0.75, 0.75, 0.75}}); // floor
        s.addRectangularPlane({-5, 3, -5},
                              {5, 3, -5},
                              {5, 3, 5},
                              {-5, 3, 5},
                              {{0, 0, 0}, {0.75, 0.75, 0.75}}); // ceiling
        s.addRectangularPlane({-5, 0, 1.5},
                              {5, 0, 1.5},
                              {5, 3, 1.5},
                              {-5, 3, 1.5},
                              {{0, 0, 0}, {0.75, 0.75, 0.75}}); // back wall
        s.addRectangularPlane({-3.5, 0, -5},
                              {-3.5, 3, -5},
                              {-3.5, 3, 5},
                              {-3.5, 0, 5},
                              {{0, 0, 0}, {0.75, 0.25, 0.25}}); // left wall
        s.addRectangularPlane({3.5, 0, -5},
                              {3.5, 3, -5},
                              {3.5, 3, 5},
                              {3.5, 0, 5},
                              {{0, 0, 0}, {0.25, 0.75, 0.25}}); // right wall

        return s;
    }();

    auto const renderParams = RenderParams{"out.png", 5, 1024};
    auto const image = render(scene, camera, renderParams, rng);

    writePng(renderParams.outputFile, image);

    return 0;
}
