#pragma once

#include <map>
#include <mutex>
#include <chrono>

#include "openGL.h"
#include "shader.h"
#include "camera.h"
#include "camera_controls.h"

static const std::map<std::string, std::vector<std::string>> SHADER_PROGRAMS_MANIFEST = {
    {"uniform_color", {"uniform_color.vert", "basic.frag"}},
    {"vertex_color",  {"basic.vert",         "basic.frag"}},
    {"strain_color",  {"strain_color.vert",  "basic.frag"}},
};

class Object3D
{
public:
    virtual ~Object3D() = default;
    virtual void render(const math::mat & projectionMatrix) const = 0;
};

class GraphicsSystem
{
private:
    static GraphicsSystem * instance;
    static std::mutex mutex;
public:
    static GraphicsSystem * get_instance();
    GraphicsSystem(const GraphicsSystem &) = delete;
    GraphicsSystem & operator=(const GraphicsSystem &) = delete;

private:
    GLFWwindow * window;
    std::map<std::string, std::shared_ptr<Program>> programs;
    Camera3D camera;
    CameraController camera_control;
    std::chrono::high_resolution_clock::time_point lastUpdateTimestamp;
    std::vector<std::shared_ptr<Object3D>> scene;

    GraphicsSystem();
    bool init_shaders();
    bool init_camera();

public:
    bool init();
    void update();
    void release();

    void add_to_scene(std::shared_ptr<Object3D> object);

    [[nodiscard]] bool should_close() const;
    [[nodiscard]]GLFWwindow * get_window() const;
    [[nodiscard]] std::shared_ptr<Program> get_shader_program(const std::string & name) const;
};