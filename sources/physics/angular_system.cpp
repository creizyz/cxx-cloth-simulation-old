#include "physics/angular_system.h"
#include "physics/constants.h"
#include "macro.h"

#include <cstring>
#include <exception>

AngularMotionSystem::AngularMotionSystem(size_t size_angular_data_pool)
    : angularData_maxNbr(0)
    , angularData_pool(nullptr)
    , angularData_used(nullptr)
    , angularData_skip(nullptr)
{
    try
    {
        angularData_maxNbr = size_angular_data_pool;
        angularData_pool = new AngularData[angularData_maxNbr];
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

AngularMotionSystem::~AngularMotionSystem()
{
    SAFE_DELETE_TAB(angularData_pool);
    SAFE_DELETE_TAB(angularData_used);
    SAFE_DELETE_TAB(angularData_skip);
}

size_t AngularMotionSystem::new_angular_data(const math::quat& position, math::quat& velocity, const math::mat& inertiaMatrix)
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

void AngularMotionSystem::free_angular_data(size_t i)
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

void AngularMotionSystem::stop_angular_update(size_t i)
{
    DBG_ASSERT(i < angularData_maxNbr);

    if (i < angularData_maxNbr && angularData_used[i])
    {
        angularData_skip[i] = true;
    }
}

void AngularMotionSystem::resume_angular_update(size_t i)
{
    DBG_ASSERT(i < angularData_maxNbr);

    if (i < angularData_maxNbr)
    {
        angularData_skip[i] = false;
    }
}

math::quat AngularMotionSystem::get_angular_position(size_t i)
{
    DBG_ASSERT(i < angularData_maxNbr);

    if (i < angularData_maxNbr && angularData_used[i])
    {
        return angularData_pool[i].p;
    }
    return math::quat();
}

void AngularMotionSystem::add_torque(size_t i, const math::vec3& torque)
{
    DBG_ASSERT(i < angularData_maxNbr);
    DBG_VALID_VEC(torque);

    if (i < angularData_maxNbr)
    {
        angularData_pool[i].t += torque;
    }
}

void AngularMotionSystem::set_inertia_matrix(size_t i, const math::mat& inertiaMatrix)
{
    DBG_ASSERT(i < angularData_maxNbr);
    DBG_VALID_MAT(inertiaMatrix);

    if (i < angularData_maxNbr)
    {
        angularData_pool[i].I = inertiaMatrix;
    }
}

void AngularMotionSystem::update_data()
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

bool AngularMotionSystem::wrong_init()
{
    return angularData_maxNbr == 0;
}
