#pragma once

#include "3D/graphics.h"
#include "3D/openGL.h"
#include "3D/shader.h"
#include "maths/math.h"
#include "physics/motion_system.h"

#define USE_IMPULSE
#define USE_IMPULSE_TO_FIX_POINTS
//#define UPDATE_ALL_AT_ONCE

struct Cloth final : public Object3D
{
    LinearMotionSystem& _lms;
    bool initialised;

    // STATIC VARIABLES
    float color[4];
    float density;
    float triangleStiffness;
    float edgeStiffness;
    float thickness;
    size_t nbrOfPoints;
    size_t nbrOfTriangles;
    size_t nbrOfEdges;
    size_t width;

    size_t* triangles;
    size_t* edges;
    float* invNbrAdjTriangles;
    float* invNbrAdjEdges;
    float* mass;

    // DYNAMIC VARIABLES
    math::vec3* posInit;
    size_t* posCur;
    gl::GLfloat* vertex_t; // only positions, use IBO
    gl::GLfloat* vertex_e; // only positions, no IBO because 1 color per edge

#ifdef UPDATE_ALL_AT_ONCE
  math::vec3 * correction;
#endif

    // OPENGL VARIABLES
    static constexpr size_t UNIFORM_COLOR = 0;
    static constexpr size_t STRAIN_COLOR  = 1;
    std::array<std::shared_ptr<Program>, 2> program;
    std::array<unsigned int, 2>             VAO;
    std::array<unsigned int, 2>             VBO;
    unsigned int                            IBO; // <- used by triangles only

    void allocateSpace(size_t size_h, size_t size_w);
    void setColor(float r, float g, float b, float a);
    void updateMass();
    void setDensity(float _density);
    void setStiffness(float triangle, float edge);
    void setThickness(float _thickness);
    void updateNbrTriangleAndEdgePerPoint();

    explicit Cloth(LinearMotionSystem& lms);
    Cloth(LinearMotionSystem& lms, const math::vec3& pos, const math::vec3& axis_h, const math::vec3& axis_w, size_t size_h, size_t size_w, float step_h, float step_w);
    ~Cloth() override;

    void initGL(std::shared_ptr<Program> uniformColorProgram, std::shared_ptr<Program> strainColorProgram);
    void render(const math::mat& projMatrix) const override;

    void applyTriangleShapeMatching(size_t iTriangle);
    void triangle2DCorrection(size_t iTriangle);
    void edgeCorrection(size_t iEdge);
    void update();
};
