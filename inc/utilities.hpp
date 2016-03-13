#ifndef UTILITIES_H
#define UTILITIES_H

#include "stdafx.hpp"

typedef enum course {
    NORTH, EAST, SOUTH, WEST,
    HEAD, TAIL,
    NILL
} course;

string to_string(course);

#define invalid_course() throw runtime_error("invalid course!")

#endif // UTILITIES_H
