#include "inc/stdafx.hpp"
#include <thread>

#include "inc/city.hpp"

int main(int, char**) {
#ifdef QTCTREATOR
    updateseed();
#else
    std::thread([](){ while(true) { updateseed(); usleep(100000 /*100 milliseconds*/); } }).detach();
#endif
    // city contains some joints connecting the streets
    // streets contain zero or more cars
    // streets are two-way in each direction
    // streets DRIVES the cars that are bound to them
    // a traffic report sent from streets/joints to TCU
    // TCU monitors/predicts/changes the lights/reports status
    city c(24, 24);
    cout<<c.add_cars(10000)<<endl;
    cout<<c.status();
    c.flow_start();
    this_thread::sleep_for(chrono::seconds(3));
    return EXIT_SUCCESS;
}
