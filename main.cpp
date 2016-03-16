#include "inc/stdafx.hpp"

#include <thread>
#include <signal.h>

#include <boost/thread.hpp>
#include "inc/main.helper.hpp"

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
    signal(SIGINT, [](int) { exit(EXIT_SUCCESS); });
    boost::thread_group tg;
    tg.create_thread(thread_simulation);
    tg.create_thread(thread_proxy_ui);
    tg.join_all();
    return EXIT_SUCCESS;
}
