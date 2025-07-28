#pragma once

#include <map>
#include "openGL.h"
#include "shader.h"

static const std::map<std::string, std::vector<std::string>> SHADER_PROGRAMS_MANIFEST = {
    {"uniform_color", {"uniform_color.vert", "basic.frag"}},
    {"vertex_color",  {"basic.vert",         "basic.frag"}},
    {"strain_color",  {"strain_color.vert",  "basic.frag"}},
};

class GraphicsSystem
{
private:
    std::map<std::string, std::shared_ptr<Program>> programs;

public:
    GraphicsSystem();

    bool init();
    bool init_shaders();
    void release();

    [[nodiscard]] std::shared_ptr<Program> get_shader_program(const std::string & name) const;
};