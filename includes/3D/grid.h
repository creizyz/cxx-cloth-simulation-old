#pragma once

#include "graphics.h"
#include "openGL.h"
#include "shader.h"
#include "maths/math.h"

struct Grid final : Object3D
{
    Grid();
    ~Grid() override;

    void init(std::shared_ptr<Program> shaderProgram, const math::vec3& gridPos, const math::vec3& axisA, const math::vec3& axisB, float sizeSquare, size_t nbrOfSquare, float r, float g, float b, float a, bool bothDirection = true);
    void render(const math::mat& projectionMatrix) const override;


    std::shared_ptr<Program> program;
    gl::GLint colorUniform;
    gl::GLint matrixUniform;
    gl::GLint positionAttribute;

    std::array<float, 4> color;

    gl::GLuint VAO;
    gl::GLuint VBO;
    gl::GLuint IBO;

    size_t drawCount;

    bool initialised;
};
