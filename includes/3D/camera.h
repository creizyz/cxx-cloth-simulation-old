#pragma once
#pragma once

#include "maths/math.h"

constexpr float MAX_VERTICAL_ANGLE = 85.0f; //must be less than 90 to avoid gimbal lock

class Camera3D
{
public:
    Camera3D();

    void set_nearAndFarPlane(float near, float far);
    void set_fieldOfView(float field_of_view);
    void increase_fieldOfView(float increment);
    void set_position(const math::vec3& position);
    void set_viewportAspect(float aspect);
    void translate(const math::vec3& t);
    void rotate(float aroundX, float aroundY);
    void look_at(const math::vec3& point);

    math::mat matrix();
    math::mat view();
    math::mat projection();
    math::mat orientation();

private:
    math::vec3 pos;
    float orientX;
    float orientY;

    float fov;
    float nearPlane;
    float farPlane;
    float viewportAspect;

    void normalize_angles();
};
