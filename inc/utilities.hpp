#ifndef UTILITIES_H
#define UTILITIES_H

#include "stdafx.hpp"

typedef enum course {
    HEAD = 0, TAIL = 1,
    NORTH, EAST, SOUTH, WEST,
    NILL
} course;

string to_string(course);
course inverse_course(course);

#define invalid_course() throw runtime_error("invalid course!")

#endif // UTILITIES_H
