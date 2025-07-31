#pragma once

#include <cstddef>
#include <memory>

#include "maths/math.h"

typedef struct
{
    math::vec3 p; // position
    math::vec3 v; // velocity
    math::vec3 f; // force
    float     im; // inverse of mass
} LinearData;


class LinearMotionSystem
{
public:
    explicit LinearMotionSystem(size_t size);

    size_t new_linear_data(const math::vec3& position, const math::vec3& velocity, float mass);
    void free_linear_data(size_t i);
    void stop_linear_update(size_t i);
    void resume_linear_update(size_t i);
    math::vec3 get_linear_position(size_t i);
    math::vec3 get_linear_velocity(size_t i);
    void move_linear_position(size_t i, const math::vec3& offset);
    void add_force(size_t i, const math::vec3& force);
    void add_velocity(size_t i, const math::vec3& velocity);
    void apply_impulse(size_t i, const math::vec3& impulse);
    void set_mass(size_t i, float mass);
    void update_data();

    [[nodiscard]] bool wrong_init() const;

private:
    size_t                        size;
    size_t                        firstAvailable;
    std::unique_ptr<LinearData[]> linearDataPool;
    std::unique_ptr<bool[]>       linearDataUsed;
    std::unique_ptr<bool[]>       linearDataSkip;
};
