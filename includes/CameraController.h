#pragma once

#include "Camera.h"

typedef enum
{
  MOVE_UP         = 0,
  MOVE_DOWN       = 1,
  MOVE_LEFT       = 2,
  MOVE_RIGHT      = 3,
  MOVE_FORWARD    = 4,
  MOVE_BACKWARD   = 5,
  ZOOM_IN         = 6,
  ZOOM_OUT        = 7,
  ROTATION_X_CW   = 8,
  ROTATION_Y_CW   = 9,
  ROTATION_X_CCW  = 10,
  ROTATION_Y_CCW  = 11,
  CENTER_VIEW     = 12
} CAMERA_ACTION;
#define COUNT_CAMERA_ACTIONS 13

class CameraController
{
public:
  CameraController(Camera3D & c) : camera(c)
  {
    clear_logs();
    actionkey[MOVE_FORWARD]     = GLFW_KEY_W;
    actionkey[MOVE_BACKWARD]    = GLFW_KEY_S;
    actionkey[MOVE_LEFT]        = GLFW_KEY_A;
    actionkey[MOVE_RIGHT]       = GLFW_KEY_D;

    actionkey[MOVE_UP]          = GLFW_KEY_R;
    actionkey[MOVE_DOWN]        = GLFW_KEY_F;

    actionkey[ROTATION_Y_CCW]   = GLFW_KEY_LEFT;
    actionkey[ROTATION_Y_CW]    = GLFW_KEY_RIGHT;
    actionkey[ROTATION_X_CCW]   = GLFW_KEY_DOWN;
    actionkey[ROTATION_X_CW]    = GLFW_KEY_UP;

    actionkey[CENTER_VIEW]      = GLFW_KEY_SPACE;

    actionkey[ZOOM_IN]          = GLFW_KEY_1;
    actionkey[ZOOM_OUT]         = GLFW_KEY_2;
  }

  void init_speeds(float Ts, float Rs, float Zs)
  {
    T_speed = Ts;
    R_speed = Rs;
    Z_speed = Zs;
  }
  void init_camera(float fov, float aspect, float near, float far)
  {
    camera.set_fieldOfView(fov);
    camera.set_nearAndFarPlane(near, far);
    camera.set_viewportAspect(aspect);
  }

  void log_action(CAMERA_ACTION a)
  {
    log[a] = true;
  }
  void unlog_action(CAMERA_ACTION a)
  {
    log[a] = false;
  }
  void clear_logs()
  {
    for (int i = 0; i < COUNT_CAMERA_ACTIONS; i++) log[i] = false;
  }

  void update_camera(float dT)
  {
    // TRANSLATION
    math::vec3 T_res(0.f, 0.f, 0.f);
    if      (log[MOVE_UP] && !log[MOVE_DOWN])           T_res += math::vec3(0.f, -1.f, 0.f);
    else if (log[MOVE_DOWN])                            T_res += math::vec3(0.f, 1.f, 0.f);
    if      (log[MOVE_LEFT] && !log[MOVE_RIGHT])        T_res += math::vec3(-1.f, 0.f, 0.f);
    else if (log[MOVE_RIGHT])                           T_res += math::vec3(1.f, 0.f, 0.f);
    if      (log[MOVE_FORWARD] && !log[MOVE_BACKWARD])  T_res += math::vec3(0.f, 0.f, -1.f);
    else if (log[MOVE_BACKWARD])                        T_res += math::vec3(0.f, 0.f, 1.f);
    camera.translate(T_res * (T_speed * dT));
    // ZOOM
    if      (log[ZOOM_IN] && !log[ZOOM_OUT]) camera.increase_fieldOfView(Z_speed * dT);
    else if (log[ZOOM_OUT])                  camera.increase_fieldOfView(-Z_speed * dT);
    // ROTATION
    float rX = 0.f;
    if      (log[ROTATION_X_CW] && !log[ROTATION_X_CCW])  rX += R_speed * dT;
    else if (log[ROTATION_X_CCW])                         rX -= R_speed * dT;
    float rY = 0.f;
    if      (log[ROTATION_Y_CW] && !log[ROTATION_Y_CCW])  rY += R_speed * dT;
    else if (log[ROTATION_Y_CCW])                         rY -= R_speed * dT;
    camera.rotate(rX, rY);
    // CENTER VIEW
    if (log[CENTER_VIEW]) camera.look_at(math::vec3(0.f, 0.f, 0.f));
  }

  int actionkey[COUNT_CAMERA_ACTIONS];
private:
  Camera3D & camera;
  
  float T_speed;
  float R_speed;
  float Z_speed;

  bool log[COUNT_CAMERA_ACTIONS];
};