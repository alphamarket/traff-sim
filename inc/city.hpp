#ifndef CITY_H
#define CITY_H

#include "stdafx.hpp"
#include "joint.hpp"
#include "street.hpp"

class city
{
    const size_t
        _height,
        _weight;
    joint_ptr** _blocks;
protected:
    typedef enum branch {
        BRANCH_RIGHT = 0,
        BRANCH_DOWN = 1,
        BRANCH_LEFT = 2,
        BRANCH_UP   = 3
    } branch;
public:
    ~city();
    city(size_t height, size_t weight);
};

#endif // CITY_H
