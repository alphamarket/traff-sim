#ifndef JOINT_HOLD_HPP
#define JOINT_HOLD_HPP

#include "joint.hpp"

/**
 * @brief The joint_kill class, never dispatches a car
 */
class joint_hold : public joint {
public:
    street* dispatch_car(car_ptr, const street*) override { return nullptr; }
};

#endif // JOINT_HOLD_HPP
