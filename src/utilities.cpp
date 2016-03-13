#include "inc/utilities.hpp"

string to_string(course d) {
    switch(d) {
        case EAST: return "EAST";
        case WEST: return "WEST";
        case HEAD: return "HEAD";
        case TAIL: return "TAIL";
        case SOUTH: return "SOUTH";
        case NORTH: return "NORTH";
        case NILL: return "NILL";
        default: throw runtime_error("invalid direction!");
    }
}
