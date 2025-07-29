#include "3D/graphics.h"

#include <iostream>
#include <algorithm>
#include "macro.h"
#include "tools/config.h"

GraphicsSystem * GraphicsSystem::instance = nullptr;
std::mutex GraphicsSystem::mutex;

GraphicsSystem* GraphicsSystem::get_instance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
        instance = new GraphicsSystem();
    }
    return instance;
}

GraphicsSystem::GraphicsSystem()
    : window{ nullptr }
    , programs{ }
    , camera{ }
    , camera_control{ camera }
    , lastUpdateTimestamp{ std::chrono::high_resolution_clock::now() }
    , scene{ }
{ }

bool GraphicsSystem::init()
{
    if (window != nullptr)
    {

    }

    auto config = Config::get_instance();
    auto width = config->get_int("graphics", "width", 400);
    auto height = config->get_int("graphics", "height", 200);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        DBG_HALT;
        return false;
    }

    window = glfwCreateWindow(width, height, "cxx-clothes", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        release();
        DBG_HALT;
        return false;
    }

    glfwMakeContextCurrent(window);
    glBindingInit();

    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    if (!init_shaders())
    {
        std::cerr << "failed to init shader programs" << std::endl;
        release();
        DBG_HALT;
        return false;
    }

    if (!init_camera())
    {
        std::cerr << "failed to init camera" << std::endl;
        release();
        DBG_HALT;
        return false;
    }

    return true;
}

bool GraphicsSystem::init_shaders()
{
    auto folder = Config::get_instance()->get("graphics", "shader_folder", ".");
    std::cout << "shader folder set to '" << folder << "'." << std::endl;

    bool success = true;
    for (const auto & shader : SHADER_PROGRAMS_MANIFEST)
    {
        std::cout << "building shader program '" << shader.first << "'" << std::endl;

        std::vector<std::filesystem::path> files;
        std::transform(
            shader.second.begin(), shader.second.end(),
            std::back_inserter(files),
            [folder](const auto & str) { return std::filesystem::path{ folder } / std::filesystem::path{ str }; }
        );

        auto program = Program::build_from_files(shader.first, files);
        if (!program)
        {
            std::cerr << "failed to build shader program '" << shader.first << "'" << std::endl;
            success = false;
            DBG_HALT;
            continue;
        }

        programs.emplace(shader.first, std::move(program));
    }

    std::cout << "built " << programs.size() << " shader programs" << std::endl;

    return success;
}

bool GraphicsSystem::init_camera()
{
    int w_width, w_height;
    glfwGetWindowSize(window, &w_width, &w_height);

    camera_control.init_camera(1.f, ((float)w_width / (float)w_height), 1.f, 1000.f);
    camera_control.init_speeds(2.f, 1.57f, 1.f);
    camera.translate(math::vec3(/*CLOTH_RESOLUTION * .5f * CLOTH_EDGE_SIZE*/0.f, .8f, 2.f));

    return true;
}

void GraphicsSystem::update()
{
    // check for exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }

    // check inputs
    for (int action = 0; action < COUNT_CAMERA_ACTIONS; action++)
    {
        auto keyStatus = glfwGetKey(window, camera_control.actionkey[action]);
        if (keyStatus == GLFW_PRESS)
        {
            camera_control.log_action(static_cast<CAMERA_ACTION>(action));
        }
        else if (keyStatus == GLFW_RELEASE)
        {
            camera_control.unlog_action(static_cast<CAMERA_ACTION>(action));
        }
    }

    // pull inputs for the next frame
    glfwPollEvents();

    // update camera
    const auto now = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastUpdateTimestamp).count();
    lastUpdateTimestamp = now;
    camera_control.update_camera(elapsed);

    // render current frame
    auto projectionMatrix = camera.matrix();
    gl::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    for (const auto & object : scene)
    {
        object->render(projectionMatrix);
    }
    glfwSwapBuffers(window);
}

void GraphicsSystem::release()
{
    programs.clear();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void GraphicsSystem::add_to_scene(std::shared_ptr<Object3D> object)
{
    scene.push_back(std::move(object));
}

bool GraphicsSystem::should_close() const
{
    return glfwWindowShouldClose(window);
}

GLFWwindow * GraphicsSystem::get_window() const
{
    return window;
}

std::shared_ptr<Program> GraphicsSystem::get_shader_program(const std::string& name) const
{
    auto found = programs.find(name);
    if (found == programs.end())
    {
        std::cerr << "could not find shader program '" << name << "'" << std::endl;
        DBG_HALT;
        return nullptr;
    }
    return found->second;
}

