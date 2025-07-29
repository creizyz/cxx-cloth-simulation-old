#include "3D/openGL.h"
#include "3D/shader.h"
#include "cloth.h"
#include "BVH.h"

#include "3D/grid.h"
#include "3D/graphics.h"
#include "tools/config.h"

std::chrono::high_resolution_clock::time_point last_update;
float update_time_correction = 0.f;

#define CLOTH_RESOLUTION 5
#define CLOTH_EDGE_SIZE .1f
#define CLOTH_THICKNESS .05f

#define COLLISION

LinearMotionSystem lms(10000);

double timerClothUpdate(0);
double minTm(1000000);
double maxTm(0);
int nbrOfFrames(0);

static constexpr auto configFile = "assets/config.ini";

int main()
{
    if (!Config::get_instance()->load(configFile))
    {
        std::cerr << "failed to load config from '" << configFile << "'. aborting." << std::endl;
        DBG_HALT;
        exit(EXIT_FAILURE);
    }

    auto graphics = GraphicsSystem::get_instance();
    if (!graphics->init())
    {
        std::cerr << "failed graphics system init. aborting." << std::endl;
        DBG_HALT;
        exit(EXIT_FAILURE);
    }

    const auto uniformColorProgram = graphics->get_shader_program("uniform_color");
    const auto strainColorProgram = graphics->get_shader_program("strain_color");

    const auto grid = std::make_shared<Grid>();
    grid->init(
        uniformColorProgram,
        math::vec3(0.f, 0.f, 0.f),
        math::vec3(1.f, 0.f, 0.f),
        math::vec3(0.f, 0.f, 1.f),
        1.f, 50, .5f, .5f, 1.f, .1f,
        true);
    graphics->add_to_scene(grid);

    const auto cloth = std::make_shared<Cloth>(lms,
        math::vec3(0.f, 1.f, 0.f),
        math::vec3(0.f, 0.f, 1.f),
        math::vec3(1.f, 0.f, 0.f),
        CLOTH_RESOLUTION, CLOTH_RESOLUTION,
        CLOTH_EDGE_SIZE, CLOTH_EDGE_SIZE);
    cloth->setColor(1.f, 1.f, 1.f, .5f);
    cloth->setDensity(1.f);
    cloth->setStiffness(1.f, 1.f);
    cloth->setThickness(CLOTH_THICKNESS);
    cloth->initGL(uniformColorProgram, strainColorProgram);
    graphics->add_to_scene(cloth);

#ifdef COLLISION
    auto clothCollisionModel = std::make_shared<ClothCollisionModel>(lms);
    clothCollisionModel->init(*cloth);
    clothCollisionModel->initGL(uniformColorProgram);
    clothCollisionModel->setStiffess(1.f);
    graphics->add_to_scene(cloth);
#endif

    last_update = std::chrono::high_resolution_clock::now();

    while (!graphics->should_close())
    {

        std::chrono::high_resolution_clock::time_point beginning_current_update = std::chrono::high_resolution_clock::now();
        float elapsed_seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(beginning_current_update - last_update)
            .count() * (float)1e-9 + update_time_correction;
        int nbrOfUpdates = (int)(elapsed_seconds / PHYSICS_TIME_STEP);

        // double tmClothUpdate(0.f);
        double tmClothCollision(0.f);
        for (int n = 0; n < nbrOfUpdates; n++)
        {
            lms.update_data();
            auto t1 = std::chrono::high_resolution_clock::now();
            cloth->update();
            double tmp = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - t1).count() * 1e-9;
            //tmClothUpdate += tmp / (double) nbrOfUpdates;
            timerClothUpdate += tmp;
            nbrOfFrames++;

            if (tmp > maxTm) maxTm = tmp;
            else if (tmp < minTm) minTm = tmp;
#ifdef COLLISION
            auto t2 = std::chrono::high_resolution_clock::now();
            clothCollisionModel->resolveInternalCollisions();
            tmClothCollision += std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - t2).count() * 1e-9 / (double)nbrOfUpdates;
#endif
        }
        update_time_correction = (elapsed_seconds - (nbrOfUpdates * PHYSICS_TIME_STEP));

        //timerClothUpdate += tmClothUpdate;
        //nbrOfFrames++;

        auto avgClothUpdateTime = nbrOfFrames > 0
                                      ? timerClothUpdate / static_cast<double>(nbrOfFrames)
                                      : 0.0;
        std::cout << avgClothUpdateTime << " over " << nbrOfFrames << " (min: " << minTm << " - max: "
            << maxTm << " )" << std::endl;

        last_update = beginning_current_update;

        graphics->update();
    }

    graphics->release();

    exit(EXIT_SUCCESS);
}
