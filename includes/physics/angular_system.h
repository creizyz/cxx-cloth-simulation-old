#pragma once

#include <cstddef>
#include <memory>

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

    size_t new_angular_data(const math::quat& position, const math::quat& velocity, const math::mat& inertiaMatrix);
    void free_angular_data(size_t i);
    void stop_angular_update(size_t i);
    void resume_angular_update(size_t i);
    math::quat get_angular_position(size_t i);
    void add_torque(size_t i, const math::vec3& torque);
    void set_inertia_matrix(size_t i, const math::mat& inertiaMatrix);
    void update_data();
    [[nodiscard]] bool wrong_init() const;

private:
    size_t                         size;
    size_t                         firstAvailable;
    std::unique_ptr<AngularData[]> angularDataPool;
    std::unique_ptr<bool[]>        angularDataUsed;
    std::unique_ptr<bool[]>        angularDataSkip;
};
