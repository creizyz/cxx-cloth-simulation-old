#pragma once

//#include "openGL.h"
#include "constants.h"
#include "math.h"

typedef struct
{
  math::vec3 p; // position
  math::vec3 v; // velocity
  math::vec3 f; // force
  float im; // inverse of mass
} LINEAR_DATA;
typedef struct
{
  math::quat p; // position
  math::quat v; // velocity
  math::vec3 t; // torque
  math::mat I; // inverse of inertia matrix
} ANGULAR_DATA;

class AngularMotionSystem
{
public:
  AngularMotionSystem(size_t size_angular_data_pool) :
    angularData_maxNbr(0),
    angularData_pool(nullptr),
    angularData_used(nullptr),
    angularData_skip(nullptr)
  {
    try
    {
      angularData_maxNbr = size_angular_data_pool;
      angularData_pool = new ANGULAR_DATA[angularData_maxNbr];
      angularData_used = new bool[angularData_maxNbr];
      memset(angularData_used, 0, angularData_maxNbr * sizeof(bool));
      angularData_firstUnused = 0;
      angularData_skip = new bool[angularData_maxNbr];
      memset(angularData_skip, 0, angularData_maxNbr * sizeof(bool));
    }
    catch (const std::exception&)
    {
      SAFE_DELETE_TAB(angularData_pool);
      SAFE_DELETE_TAB(angularData_used);
      SAFE_DELETE_TAB(angularData_skip);
      angularData_maxNbr = 0;
      angularData_pool = nullptr;
      angularData_used = nullptr;
      angularData_skip = nullptr;
    }

  }
  ~AngularMotionSystem()
  {
    SAFE_DELETE_TAB(angularData_pool);
    SAFE_DELETE_TAB(angularData_used);
    SAFE_DELETE_TAB(angularData_skip);
  }
  
  size_t new_angular_data(const math::quat & position, math::quat & velocity, const math::mat & inertiaMatrix)
  {
    for (size_t n = angularData_firstUnused; n < angularData_maxNbr; n++)
    {
      if (!angularData_used[n])
      {
        angularData_firstUnused = n + 1;
        angularData_used[n] = true;
        angularData_pool[n].p = position;
        angularData_pool[n].v = velocity;
        angularData_pool[n].I = inertiaMatrix;
        return n;
      }
    }
    return -1;
  }
  void free_angular_data(size_t i)
  {
    DBG_ASSERT(i < angularData_maxNbr);
    DBG_ASSERT(angularData_used[i]);

    if (i < angularData_maxNbr)
    {
      angularData_used[i] = false;
      angularData_skip[i] = false;
      if (i < angularData_firstUnused) angularData_firstUnused = i;
    }
  }
  void stop_angular_update(size_t i)
  {
    DBG_ASSERT(i < angularData_maxNbr);

    if (i < angularData_maxNbr && angularData_used[i])
    {
      angularData_skip[i] = true;
    }
  }
  void resume_angular_update(size_t i)
  {
    DBG_ASSERT(i < angularData_maxNbr);

    if (i < angularData_maxNbr)
    {
      angularData_skip[i] = false;
    }
  }
  math::quat get_angular_position(size_t i)
  {
    DBG_ASSERT(i < angularData_maxNbr);

    if (i < angularData_maxNbr && angularData_used[i])
    {
      return angularData_pool[i].p;
    }
    return math::quat();
  }
  void add_torque(size_t i, const math::vec3 & torque)
  {
    DBG_ASSERT(i < angularData_maxNbr);
    DBG_VALID_VEC(torque);

    if (i < angularData_maxNbr)
    {
      angularData_pool[i].t += torque;
    }
  }
  void set_inertia_matrix(size_t i, const math::mat & inertiaMatrix)
  {
    DBG_ASSERT(i < angularData_maxNbr);
    DBG_VALID_mat(inertiaMatrix);

    if (i < angularData_maxNbr)
    {
      angularData_pool[i].I = inertiaMatrix;
    }
  }
  void update_data()
  {
    for (size_t n = 0; n < angularData_maxNbr; n++)
    {
      if (angularData_used[n] && !angularData_skip[n])
      {
        angularData_pool[n].v = (angularData_pool[n].v * PHYSICS_DAMPING_FACTOR) * math::quat(0.f, (angularData_pool[n].I * angularData_pool[n].t * PHYSICS_TIME_STEP));
        angularData_pool[n].p = angularData_pool[n].p * (angularData_pool[n].v * PHYSICS_TIME_STEP);
      }
    }
  }
  bool wrong_init()
  {
    return angularData_maxNbr == 0;
  }
private:
  size_t angularData_maxNbr;
  ANGULAR_DATA * angularData_pool;
  bool * angularData_used;
  size_t angularData_firstUnused;
  bool * angularData_skip;
};
class LinearMotionSystem
{
public:
  LinearMotionSystem(size_t size_linear_data_pool) :
      linearData_maxNbr(0),
      linearData_pool(nullptr),
      linearData_used(nullptr),
      linearData_skip(nullptr)
    {
      try
      {
        linearData_maxNbr = size_linear_data_pool;
        linearData_pool = new LINEAR_DATA[linearData_maxNbr];
        linearData_used = new bool[linearData_maxNbr];
        memset(linearData_used, 0, linearData_maxNbr * sizeof(bool));
        linearData_firstUnused = 0;
        linearData_skip = new bool[linearData_maxNbr];
        memset(linearData_skip, 0, linearData_maxNbr * sizeof(bool));
      }
      catch (const std::exception&)
      {
        SAFE_DELETE_TAB(linearData_pool);
        SAFE_DELETE_TAB(linearData_used);
        SAFE_DELETE_TAB(linearData_skip);

        linearData_maxNbr = 0;
        linearData_pool = nullptr;
        linearData_used = nullptr;
        linearData_skip = nullptr;
      }
  }
  ~LinearMotionSystem()
  {
    SAFE_DELETE_TAB(linearData_pool);
    SAFE_DELETE_TAB(linearData_used);
    SAFE_DELETE_TAB(linearData_skip);
  }

  size_t new_linear_data(const math::vec3 & position, const math::vec3 & velocity, float mass)
  {
    DBG_VALID_VEC(position);
    DBG_VALID_VEC(velocity);
    DBG_VALID_FLOAT(mass);
    DBG_ASSERT(mass != 0);

    if (mass != 0)
    {
      for (size_t n = linearData_firstUnused; n < linearData_maxNbr; n++)
      {
        if (!linearData_used[n])
        {
          linearData_firstUnused = n + 1;
          linearData_used[n] = true;
          linearData_pool[n].p = position;
          linearData_pool[n].v = velocity;
          linearData_pool[n].im = 1 / mass;
          return n;
        }
      }
    }
    return -1;
  }
  void free_linear_data(size_t i)
  {
    DBG_ASSERT(i < linearData_maxNbr);
    DBG_ASSERT(linearData_used[i]);

    if (i < linearData_maxNbr)
    {
      linearData_used[i] = false;
      linearData_skip[i] = false;
      if (i < linearData_firstUnused) linearData_firstUnused = i;
    }
  }
  void stop_linear_update(size_t i)
  {
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr && linearData_used[i])
    {
      linearData_skip[i] = true;
    }
  }
  void resume_linear_update(size_t i)
  {
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr)
    {
      linearData_skip[i] = false;
    }
  }
  math::vec3 get_linear_position(size_t i)
  {
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr  && linearData_used[i])
    {
      return linearData_pool[i].p;
    }
    return math::vec3();
  }
  math::vec3 get_linear_velocity(size_t i)
  {
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr  && linearData_used[i])
    {
      return linearData_pool[i].v;
    }
    return math::vec3();
  }
  void move_linear_position(size_t i, const math::vec3 & offset)
  {
    DBG_VALID_VEC(offset);

    DBG_ASSERT(i < linearData_maxNbr);
    DBG_ASSERT(linearData_used[i]);

    if (i < linearData_maxNbr)
    {
      linearData_pool[i].p += offset;
      linearData_pool[i].v += offset / PHYSICS_TIME_STEP;
    }
  }
  void add_force(size_t i, const math::vec3 & force)
  {
    DBG_VALID_VEC(force);
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr)
    {
      linearData_pool[i].f += force;
    }
  }
  void add_velocity(size_t i, const math::vec3 & velocity)
  {
    DBG_VALID_VEC(velocity);
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr)
    {
      linearData_pool[i].v += velocity;
    }
  }
  void apply_impulse(size_t i, const math::vec3 & impulse)
  {
    DBG_VALID_VEC(impulse);
    DBG_ASSERT(i < linearData_maxNbr);

    if (i < linearData_maxNbr)
    {
      linearData_pool[i].v += impulse * linearData_pool[i].im;
    }
  }
  void set_mass(size_t i, float mass)
  {
    DBG_VALID_FLOAT(mass);
    DBG_ASSERT(i < linearData_maxNbr);
    DBG_ASSERT(mass != 0.f);

    if (i < linearData_maxNbr && mass != 0.f)
    {
      linearData_pool[i].im = 1.f / mass;
    }
  }
  void update_data()
  {
    for (size_t n = 0; n < linearData_maxNbr; n++)
    {
      if (linearData_used[n] && !linearData_skip[n])
      {
        linearData_pool[n].v = linearData_pool[n].v * PHYSICS_DAMPING_FACTOR + linearData_pool[n].f * linearData_pool[n].im * PHYSICS_TIME_STEP;
        linearData_pool[n].p += linearData_pool[n].v * PHYSICS_TIME_STEP;
        linearData_pool[n].f = math::vec3();
      }
    }
  }
  bool wrong_init()
  {
    return linearData_maxNbr == 0;
  }
private:
  size_t linearData_maxNbr;
  LINEAR_DATA  * linearData_pool;
  bool * linearData_used;
  size_t linearData_firstUnused;
  bool * linearData_skip;
};