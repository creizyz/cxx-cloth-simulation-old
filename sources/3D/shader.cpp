#include "3D/shader.h"

#include <fstream>
#include <iostream>
#include <utility>

#include "macro.h"

constexpr ShaderUtils::Type ShaderUtils::get_type_from_extension(std::string_view extension)
{
    for (const auto & mapping : EXTENSION_TO_TYPE)
    {
        if (mapping.extension == extension)
        {
            return mapping.type;
        }
    }

    return Type::UnknownType;
}

constexpr gl::GLenum ShaderUtils::get_gl_enum_from_type(Type type)
{
    for (const auto & mapping : TYPE_TO_GL_ENUM)
    {
        if (mapping.type == type)
        {
            return mapping.value;
        }
    }

    return gl::GL_INVALID_VALUE;
}

Shader::Shader()
    : type{ ShaderUtils::Type::UnknownType }
    , handle{ 0u }
{ }

Shader::Shader(ShaderUtils::Type type, unsigned int handle)
    : type{ type }
    , handle{ handle }
{ }

Shader::~Shader()
{
    if (handle != 0u)
    {
        gl::glDeleteShader(handle);
    }
}

unsigned int Shader::get_handle() const
{
    return handle;
}

std::shared_ptr<Shader> Shader::load_from_file(const std::filesystem::path & path)
{
    std::cout << "load shader from file '" << path.c_str() << "'" << std::endl;

    const auto extension = path.extension().string();
    const auto type = ShaderUtils::get_type_from_extension(extension);
    if (type == ShaderUtils::Type::UnknownType)
    {
        std::cerr << "shader compile failed (could not recognize shader type for '" << path << "')." << std::endl;
        DBG_HALT;
        return nullptr;
    }

    if (!std::filesystem::exists(path))
    {
        std::cerr << "shader compile failed (file '" << path << "' not found)." << std::endl;
        DBG_HALT;
        return nullptr;
    }

    std::ifstream file(path, std::ifstream::in);
    if (!file.is_open())
    {
        std::cerr << "shader compile failed (could not open '" << path << "')." << std::endl;
        DBG_HALT;
        return nullptr;
    }

    const std::string sources((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return load_from_source(sources, type);
}

std::shared_ptr<Shader> Shader::load_from_source(std::string_view sources, ShaderUtils::Type type)
{
    const auto shaderType = ShaderUtils::get_gl_enum_from_type(type);
    if (shaderType == gl::GL_INVALID_VALUE)
    {
        std::cerr << "shader compile failed (invalid shader type)" << std::endl;
        return nullptr;
    }

    const auto & shaderSources = sources.data();
    const auto shader = gl::glCreateShader(shaderType);
    gl::glShaderSource(shader, 1, &shaderSources, nullptr);
    gl::glCompileShader(shader);

    gl::GLint success;
    gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cerr << "shader compile failed with this message:" << std::endl;

        int maxLength = 0;
        gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        gl::glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cerr << &errorLog[0] << std::endl;

        DBG_HALT;

        gl::glDeleteShader(shader);
        return nullptr;
    }

    return std::make_shared<Shader>(type, shader);
}

Program::Program()
    : name{ "unknown" }
    , handle{ 0u }
{ }

Program::Program(std::string name, unsigned int handle)
    : name{std::move( name )}
    , handle{ handle }
{ }

Program::~Program()
{
    if (handle != 0u)
    {
        gl::glDeleteProgram(handle);
    }
}

const std::string & Program::get_name() const
{
    return name;
}

void Program::use() const
{
    gl::glUseProgram(handle);
}

std::shared_ptr<Program> Program::build_from_files(const std::string & name, const std::vector<std::filesystem::path> & shaderFiles)
{
    const auto program = gl::glCreateProgram();

    for (const auto & file : shaderFiles)
    {
        const auto shader = Shader::load_from_file(std::filesystem::absolute(file));
        if (!shader)
        {
            std::cerr << "failed to link shader program (shader unit compilation failed)." << std::endl;
            gl::glDeleteProgram(program);
            DBG_HALT;
            return nullptr;
        }

        gl::glAttachShader(program, shader->get_handle());
    }

    gl::glLinkProgram(program);

    gl::GLint success;
    gl::glGetProgramiv(program, gl::GL_LINK_STATUS, &success);
    if (!success)
    {
        std::cerr << "shader linking failed with this message:" << std::endl;
        std::vector<char> compilation_log(512);
        gl::glGetProgramInfoLog(program, compilation_log.size(), nullptr, &compilation_log[0]);
        std::cerr << &compilation_log[0] << std::endl;
        gl::glDeleteProgram(program);
        DBG_HALT;
        return nullptr;
    }

    return std::make_shared<Program>(name, program);
}

int Program::get_location(std::string_view name, gl::GLenum interface) const
{
    auto location = gl::glGetProgramResourceLocation(handle, interface, name.data());
    if (location == -1)
    {
        std::cerr << "failed to retrieve shader location for resource '" << name << "'" << std::endl;
        DBG_HALT;
    }

    return location;
}

unsigned int Program::get_handle() const
{
    return handle;
}
