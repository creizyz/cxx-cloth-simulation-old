#pragma once

#include <cstddef>
#include "maths/math.h"

struct AngularData
{
    math::quat p; // position
    math::quat v; // velocity
    math::vec3 t; // torque
    math::mat  I; // inverse of inertia matrix
};

class AngularMotionSystem
{
public:
    explicit AngularMotionSystem(size_t size_angular_data_pool);
    ~AngularMotionSystem();

    size_t new_angular_data(const math::quat& position, math::quat& velocity, const math::mat& inertiaMatrix);
    void free_angular_data(size_t i);
    void stop_angular_update(size_t i);
    void resume_angular_update(size_t i);
    math::quat get_angular_position(size_t i);
    void add_torque(size_t i, const math::vec3& torque);
    void set_inertia_matrix(size_t i, const math::mat& inertiaMatrix);
    void update_data();
    bool wrong_init();

private:
    size_t angularData_maxNbr;
    AngularData* angularData_pool;
    bool* angularData_used;
    size_t angularData_firstUnused;
    bool* angularData_skip;
};
