#include "inc/utilities.hpp"

string to_string(course d) {
    switch(d) {
        case NILL: return "NILL";
        case EAST: return "EAST";
        case WEST: return "WEST";
        case HEAD: return "HEAD";
        case TAIL: return "TAIL";
        case SOUTH: return "SOUTH";
        case NORTH: return "NORTH";
        default: invalid_course();
    }
}

course inverse_course(course d) {
    switch(d) {
        case NILL: return NILL;
        case EAST: return WEST;
        case WEST: return EAST;
        case HEAD: return TAIL;
        case TAIL: return HEAD;
        case SOUTH: return NORTH;
        case NORTH: return SOUTH;
        default: invalid_course();
    }
}
