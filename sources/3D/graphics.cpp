#include "3D/graphics.h"

#include <iostream>
#include <algorithm>
#include "macro.h"
#include "tools/config.h"

GraphicsSystem::GraphicsSystem()
    : programs{ }
{ }

bool GraphicsSystem::init()
{
    if (!init_shaders())
    {
        std::cerr << "failed to init shader programs" << std::endl;
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

void GraphicsSystem::release()
{
    programs.clear();
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

