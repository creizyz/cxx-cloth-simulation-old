#include "physics/linear_system.h"
#include "physics/constants.h"
#include "macro.h"

#include <cstring>
#include <exception>

LinearMotionSystem::LinearMotionSystem(size_t size)
    : size{ size }
    , firstAvailable{ 0u }
    , linearDataPool{ std::make_unique<LinearData[]>(size) }
    , linearDataUsed{ std::make_unique<bool[]>(size) }
    , linearDataSkip{ std::make_unique<bool[]>(size) }
{ }

size_t LinearMotionSystem::new_linear_data(const math::vec3& position, const math::vec3& velocity, const float mass)
{
    DBG_VALID_VEC(position);
    DBG_VALID_VEC(velocity);
    DBG_VALID_FLOAT(mass);
    DBG_ASSERT(mass != 0);

    if (mass != 0)
    {
        for (size_t n = this->firstAvailable; n < this->size; n++)
        {
            if (!linearDataUsed[n])
            {
                this->firstAvailable = n + 1;
                linearDataUsed[n] = true;
                linearDataPool[n].p = position;
                linearDataPool[n].v = velocity;
                linearDataPool[n].im = 1 / mass;
                return n;
            }
        }
    }
    return -1;
}

void LinearMotionSystem::free_linear_data(size_t i)
{
    DBG_ASSERT(i < this->size);
    DBG_ASSERT(linearDataUsed[i]);

    if (i < this->size)
    {
        linearDataUsed[i] = false;
        linearDataSkip[i] = false;
        if (i < this->firstAvailable)
        {
            this->firstAvailable = i;
        }
    }
}

void LinearMotionSystem::stop_linear_update(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size && linearDataUsed[i])
    {
        linearDataSkip[i] = true;
    }
}

void LinearMotionSystem::resume_linear_update(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size)
    {
        linearDataSkip[i] = false;
    }
}

math::vec3 LinearMotionSystem::get_linear_position(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size && linearDataUsed[i])
    {
        return linearDataPool[i].p;
    }
    return {};
}

math::vec3 LinearMotionSystem::get_linear_velocity(size_t i)
{
    DBG_ASSERT(i < this->size);

    if (i < this->size && linearDataUsed[i])
    {
        return linearDataPool[i].v;
    }
    return {};
}

void LinearMotionSystem::move_linear_position(size_t i, const math::vec3& offset)
{
    DBG_VALID_VEC(offset);

    DBG_ASSERT(i < this->size);
    DBG_ASSERT(linearDataUsed[i]);

    if (i < this->size)
    {
        linearDataPool[i].p += offset;
        linearDataPool[i].v += offset / PHYSICS_TIME_STEP;
    }
}

void LinearMotionSystem::add_force(size_t i, const math::vec3& force)
{
    DBG_VALID_VEC(force);
    DBG_ASSERT(i < this->size);

    if (i < this->size)
    {
        linearDataPool[i].f += force;
    }
}

void LinearMotionSystem::add_velocity(size_t i, const math::vec3& velocity)
{
    DBG_VALID_VEC(velocity);
    DBG_ASSERT(i < this->size);

    if (i < this->size)
    {
        linearDataPool[i].v += velocity;
    }
}

void LinearMotionSystem::apply_impulse(size_t i, const math::vec3& impulse)
{
    DBG_VALID_VEC(impulse);
    DBG_ASSERT(i < this->size);

    if (i < this->size)
    {
        linearDataPool[i].v += impulse * linearDataPool[i].im;
    }
}

void LinearMotionSystem::set_mass(size_t i, float mass)
{
    DBG_VALID_FLOAT(mass);
    DBG_ASSERT(i < this->size);
    DBG_ASSERT(mass != 0.f);

    if (i < this->size && mass != 0.f)
    {
        linearDataPool[i].im = 1.f / mass;
    }
}

void LinearMotionSystem::update_data()
{
    for (size_t n = 0; n < this->size; n++)
    {
        if (linearDataUsed[n] && !linearDataSkip[n])
        {
            linearDataPool[n].v = linearDataPool[n].v * PHYSICS_DAMPING_FACTOR + linearDataPool[n].f * linearDataPool[n].im * PHYSICS_TIME_STEP;
            linearDataPool[n].p += linearDataPool[n].v * PHYSICS_TIME_STEP;
            linearDataPool[n].f = math::vec3();
        }
    }
}

bool LinearMotionSystem::wrong_init() const
{
    return this->size == 0;
}
