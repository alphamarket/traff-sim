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

template<typename K, typename V>
inline void json_build_str(K k, V v, size_t level, stringstream& ss, bool quote = true, bool colon = true)
{ ss << endl << std::string(level * 3, ' ') << k << (colon ? ": " : "") << (quote ? "«" : "") << v << (quote ? "»" : ""); }

inline void json_open_str(string name, size_t level, stringstream& ss) {
    json_build_str(name + (name.length() ? ": " : ""), "{", level, ss, false, false);
}
inline void json_close_str(size_t level, stringstream& ss) {
    json_build_str("", "}", level, ss, false, false);
}

template<typename K, typename V>
inline string json_build_str(K k, V v, size_t level, bool quote = true, bool colon = true) {
    stringstream ss;
    json_build_str(k,v,level,ss, quote, colon);
    return ss.str();
}

#define invalid_course() throw runtime_error("invalid course!")

#endif // UTILITIES_H
