#pragma once

#include "openGL.h"
#include "maths/math.h"

struct Grid
{
    Grid();
    ~Grid();

    void init(gl::GLuint shaderProgram, const math::vec3& gridPos, const math::vec3& axisA, const math::vec3& axisB, float sizeSquare, size_t nbrOfSquare, float r, float g, float b, float a, bool bothDirection = true);
    void render(const math::mat& projMatrix);

    gl::GLuint program;
    gl::GLint colorUniform;
    float color[4];

    gl::GLuint VAO;
    gl::GLuint VBO;
    gl::GLuint IBO;
    size_t drawCount;

    bool initialised;
};
