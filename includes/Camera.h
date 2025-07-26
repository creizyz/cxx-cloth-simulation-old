#pragma once

#include "openGL.h"
#include "math.h"
#include "macro.h"

#include <math.h>
#include <algorithm> // std::min, std::max

/*class Camera2D : public Camera
{
public:
  static math::mat construct_projection_matrix(float left, float right, float top, float bottom, float nearDist, float farDist)
  {
    DBG_VALID_FLOAT(left);
    DBG_VALID_FLOAT(right);
    DBG_VALID_FLOAT(top);
    DBG_VALID_FLOAT(bottom);
    DBG_VALID_FLOAT(nearDist);
    DBG_VALID_FLOAT(farDist);

    DBG_ASSERT(right - left > 0);
    DBG_ASSERT(top - bottom > 0);
    DBG_ASSERT(nearDist != farDist);

    math::mat projection_matrix(1);
    if (right - left > 0 && top - bottom > 0)
    {
      projection_matrix.data[0] = 2.f / (right - left);
      projection_matrix.data[5] = 2.f / (top - bottom);
      projection_matrix.data[10] = -2.f / (farDist - nearDist);
      projection_matrix.data[11] = -(farDist + nearDist) / (farDist - nearDist);
      projection_matrix.data[3] = -(right + left) / (right - left);
      projection_matrix.data[7] = -(top + bottom) / (top - bottom);
    }
    DBG_VALID_mat(projection_matrix);
    return projection_matrix;
  }

  Camera2D() : Camera() {}

  void look_at(const math::vec3 & point)
  {
    math::vec3 projected_point = point * (view_matrix * projection_matrix);
    translate(projected_point); // camera goes on the point
  }
  void rotate(const math::quat & rotation)
  {

  }
};*/

const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

class Camera3D
{
public:
  Camera3D(){}

  void set_nearAndFarPlane(float near, float far)
  {
    DBG_ASSERT(near > 0.f);
    DBG_ASSERT(far > near);
    nearPlane = std::min(0.1f, near);
    farPlane = std::max(nearPlane + 0.1f, far);
  }
  void set_fieldOfView(float field_of_view)
  {
    //DBG_ASSERT(field_of_view > 0.03f && field_of_view < 2.2f);
    fov = std::min(2.2f, std::max(0.03f, field_of_view));
  }
  void increase_fieldOfView(float increment)
  {
    set_fieldOfView(fov + increment);
  }
  void set_position(const math::vec3 & position)
  {
    pos = position;
  }
  void set_viewportAspect(float aspect)
  {
    DBG_ASSERT(aspect != 0.f);
    if (aspect != 0.f) viewportAspect = aspect;
  }

  void translate(const math::vec3 & t)
  {
    DBG_VALID_VEC(t);
    pos += (math::mat::transpose(view()) * t);
  }
  void rotate(float aroundX, float aroundY)
  {
    orientX += aroundX;
    orientY += aroundY;
    normalize_angles();
  }

  void look_at(const math::vec3 & point)
  {
    DBG_ASSERT(point != pos);
    if (point != pos)
    {
      math::vec3 dir = (point - pos).normalized();
      orientX = asin(-dir.y);
      orientY = atan2f(-dir.x, -dir.z);
      normalize_angles();
    }
  }

  math::mat matrix()
  {
    return projection() * view();
  }
  math::mat view()
  {
    return orientation() * math::mat::T_Transform(-pos);
  }
  math::mat projection()
  {
    DBG_VALID_FLOAT(fov);
    DBG_VALID_FLOAT(viewportAspect);
    DBG_VALID_FLOAT(nearPlane);
    DBG_VALID_FLOAT(farPlane);

    math::mat projection_matrix(4,4,1);
    if (fov > 0 && viewportAspect > 0 && nearPlane != farPlane)
    {
      float tanHalfFov = tan(0.5f * fov);

      projection_matrix.data[0] = 1.f / (tanHalfFov * viewportAspect);
      projection_matrix.data[5] = 1.f / tanHalfFov;
      projection_matrix.data[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
      projection_matrix.data[11] = -(2.f * farPlane * nearPlane) / (farPlane - nearPlane);
      projection_matrix.data[14] = -1.f;
      projection_matrix.data[15] = 0.f;
    }
    DBG_VALID_mat(projection_matrix);
    return projection_matrix;
  }
  math::mat orientation()
  {
    return math::mat::R_Transform(orientX, math::vec3(1.f, 0.f, 0.f)) * math::mat::R_Transform(orientY, math::vec3(0.f, 1.f, 0.f));
  }
private:
  math::vec3 pos;
  float orientX;
  float orientY;

  float fov;
  float nearPlane;
  float farPlane;
  float viewportAspect;

  void normalize_angles()
  {
    orientY = fmod(orientY, 360.f);
    //if (orientY < 0.f)  orientY += 360.f;
    orientX = std::min(MaxVerticalAngle, std::max(-MaxVerticalAngle, orientX));
  }
};