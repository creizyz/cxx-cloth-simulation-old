#pragma once

#include <array>
#include <string_view>
#include <filesystem>
#include <map>
#include <optional>
#include <glbinding/gl/gl.h>

namespace ShaderUtils
{
    /**
     * Represents the types of shaders supported by the system.
     *
     * This enumeration defines various shader types that can be used in
     * rendering pipelines or computational tasks. Each shader type corresponds
     * to a specific shader stage in the graphics pipeline. An additional
     * `UnknownType` value is provided to represent unrecognized or unsupported
     * shader types.
     */
    enum class Type
    {
        VertexShader,
        FragmentShader,
        GeometryShader,
        ComputeShader,
        TesselationControlShader,
        TesselationEvaluationShader,
        UnknownType,
    };

    struct ExtensionToTypeMapping
    {
        std::string_view extension;
        Type             type;
    };

    constexpr std::array<ExtensionToTypeMapping, 17> EXTENSION_TO_TYPE = {
        ExtensionToTypeMapping{".vert", Type::VertexShader},
        ExtensionToTypeMapping{".vs",   Type::VertexShader},
        ExtensionToTypeMapping{".vsh",  Type::VertexShader},
        ExtensionToTypeMapping{".frag", Type::FragmentShader},
        ExtensionToTypeMapping{".fs",   Type::FragmentShader},
        ExtensionToTypeMapping{".fsh",  Type::FragmentShader},
        ExtensionToTypeMapping{".geom", Type::GeometryShader},
        ExtensionToTypeMapping{".gs",   Type::GeometryShader},
        ExtensionToTypeMapping{".gsh",  Type::GeometryShader},
        ExtensionToTypeMapping{".comp", Type::ComputeShader},
        ExtensionToTypeMapping{".cs",   Type::ComputeShader},
        ExtensionToTypeMapping{".tesc", Type::TesselationControlShader},
        ExtensionToTypeMapping{".tc",   Type::TesselationControlShader},
        ExtensionToTypeMapping{".tcs",  Type::TesselationControlShader},
        ExtensionToTypeMapping{".tese", Type::TesselationEvaluationShader},
        ExtensionToTypeMapping{".te",   Type::TesselationEvaluationShader},
        ExtensionToTypeMapping{".tes",  Type::TesselationEvaluationShader},
    };

    struct TypeToGlMapping
    {
        Type       type;
        gl::GLenum value;
    };

    constexpr std::array<TypeToGlMapping, 6> TYPE_TO_GL_ENUM = {
        TypeToGlMapping{Type::VertexShader,                gl::GL_VERTEX_SHADER},
        TypeToGlMapping{Type::FragmentShader,              gl::GL_FRAGMENT_SHADER},
        TypeToGlMapping{Type::GeometryShader,              gl::GL_GEOMETRY_SHADER},
        TypeToGlMapping{Type::ComputeShader,               gl::GL_COMPUTE_SHADER},
        TypeToGlMapping{Type::TesselationControlShader,    gl::GL_TESS_CONTROL_SHADER},
        TypeToGlMapping{Type::TesselationEvaluationShader, gl::GL_TESS_EVALUATION_SHADER},
    };

    /**
     * Maps a file extension to its corresponding shader type.
     *
     * This function iterates through a predefined list of shader file extensions and
     * attempts to find a mapping to a `ShaderUtils::Type` value. If the provided
     * extension matches one in the list, the corresponding shader type is returned.
     * If no match is found, the function returns `Type::UnknownType`.
     *
     * @param extension The file extension to map, provided as a `std::string_view`.
     * @return The shader type corresponding to the given extension. If the extension
     *         is not recognized, returns `Type::UnknownType`.
     */
    constexpr Type get_type_from_extension(std::string_view extension);

    /**
     * Converts a shader type to its corresponding OpenGL enumerator value.
     *
     * This function takes a shader type, searches for its mapping in the predefined
     * array `TYPE_TO_GL_ENUM`, and returns the associated OpenGL enumeration value
     * if found. If the given shader type is not found in the mapping, the function
     * returns `gl::GL_INVALID_ENUM`.
     *
     * @param type The shader type to convert, specified as a value of the `Type` enum.
     * @return The OpenGL enumeration value corresponding to the given shader type.
     *         Returns `gl::GL_INVALID_ENUM` if the type is not recognized.
     */
    constexpr gl::GLenum get_gl_enum_from_type(Type type);
}

class Shader
{
private:
    ShaderUtils::Type type;
    unsigned int      handle;

public:
    Shader();
    Shader(ShaderUtils::Type type, gl::GLuint handle);
    ~Shader();

    [[nodiscard]] unsigned int get_handle() const;

    static std::shared_ptr<Shader> load_from_file(const std::filesystem::path & path);
    static std::shared_ptr<Shader> load_from_source(std::string_view sources, ShaderUtils::Type type);
};

class Program
{
private:
    std::string  name;
    unsigned int handle;

public:
    Program();
    explicit Program(std::string  name, gl::GLuint handle);
    ~Program();

    [[nodiscard]] const std::string & get_name() const;
    [[nodiscard]] unsigned int get_handle() const;

    [[nodiscard]] int get_location(std::string_view name, gl::GLenum interface) const;

    void use() const;

    static std::shared_ptr<Program> build_from_files(const std::string & name, const std::vector<std::filesystem::path> & shaderFiles);
};
