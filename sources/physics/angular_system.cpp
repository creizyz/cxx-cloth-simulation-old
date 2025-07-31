#include "physics/angular_system.h"
#include "physics/constants.h"
#include "macro.h"

#include <cstring>
#include <exception>

AngularMotionSystem::AngularMotionSystem(size_t size)
    : size{ size }
    , firstAvailable{ 0u }
    , angularDataPool{ std::make_unique<AngularData[]>(size) }
    , angularDataUsed{ std::make_unique<bool[]>(size) }
    , angularDataSkip{ std::make_unique<bool[]>(size) }
{ }

size_t AngularMotionSystem::new_angular_data(const math::quat& position, const math::quat& velocity, const math::mat& inertiaMatrix)
{
    for (size_t n = this->firstAvailable; n < this->size; n++)
    {
        if (!angularDataUsed[n])
        {
            this->firstAvailable = n + 1;
            angularDataUsed[n] = true;
            angularDataPool[n].p = position;
            angularDataPool[n].v = velocity;
            angularDataPool[n].I = inertiaMatrix;
            return n;
        }
    }
    return -1;
}

void AngularMotionSystem::free_angular_data(size_t i)
{
    DBG_ASSERT(i < this->size);
    DBG_ASSERT(angularDataUsed[i]);

    if (i < this->size)
    {
        angularDataUsed[i] = false;
        angularDataSkip[i] = false;
        if (i < this->firstAvailable)
        {
            this->firstAvailable = i;
        }
    }
}

void AngularMotionSystem::stop_angular_update(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size && angularDataUsed[i])
    {
        angularDataSkip[i] = true;
    }
}

void AngularMotionSystem::resume_angular_update(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size)
    {
        angularDataSkip[i] = false;
    }
}

math::quat AngularMotionSystem::get_angular_position(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size && angularDataUsed[i])
    {
        return angularDataPool[i].p;
    }
    return {};
}

void AngularMotionSystem::add_torque(size_t i, const math::vec3& torque)
{
    DBG_ASSERT(i < this->size);
    DBG_VALID_VEC(torque);

    if (i < this->size)
    {
        angularDataPool[i].t += torque;
    }
}

void AngularMotionSystem::set_inertia_matrix(size_t i, const math::mat& inertiaMatrix)
{
    DBG_ASSERT(i < this->size);
    DBG_VALID_MAT(inertiaMatrix);

    if (i < this->size)
    {
        angularDataPool[i].I = inertiaMatrix;
    }
}

void AngularMotionSystem::update_data()
{
    for (size_t n = 0; n < this->size; n++)
    {
        if (angularDataUsed[n] && !angularDataSkip[n])
        {
            angularDataPool[n].v = angularDataPool[n].v * PHYSICS_DAMPING_FACTOR * math::quat(0.f, angularDataPool[n].I * angularDataPool[n].t * PHYSICS_TIME_STEP);
            angularDataPool[n].p = angularDataPool[n].p * angularDataPool[n].v * PHYSICS_TIME_STEP;
        }
    }
}

bool AngularMotionSystem::wrong_init() const
{
    return this->size == 0;
}
