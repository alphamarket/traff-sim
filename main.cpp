#include "inc/stdafx.hpp"

#include "inc/TCU.hpp"
#include "inc/Car.hpp"
#include "inc/Joint.hpp"
#include "inc/Street.hpp"

int main(int, char**) {
    // city contains some joints connecting the streets
    // streets contain zero or more cars
    // only cars can change it's directions
    // streets/joints reports to TCU
    // TCU monitors/predicts/changes the lights/reports status
    return EXIT_SUCCESS;
}
