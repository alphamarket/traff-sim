#ifndef JOINT_KILL_HPP
#define JOINT_KILL_HPP

#include "joint.hpp"

/**
 * @brief The joint_kill class, kills every car that passed to be dispatched
 */
class joint_kill : public joint {
public:
    street* dispatch_car(car_ptr, const street *src) override { return const_cast<street*>(src); }
};

#endif // JOINT_KILL_HPP

