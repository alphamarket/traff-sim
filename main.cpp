#include "inc/stdafx.hpp"

#include "inc/TCU.hpp"
#include "inc/Car.hpp"
#include "inc/Joint.hpp"
#include "inc/Street.hpp"

int main(int, char**) {
    // city contains some joints connecting the streets
    // streets contain zero or more cars
    // streets are two-way in each direction
    // streets DRIVES the cars that are bound to them
    // a traffic report sent from streets/joints to TCU
    // TCU monitors/predicts/changes the lights/reports status
    return EXIT_SUCCESS;
}
