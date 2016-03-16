#include "inc/stdafx.hpp"
#include <thread>
#include <signal.h>

#include "inc/city.hpp"
#include "inc/server.hpp"

atomic<bool> stop(false);

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
    try
    {
        {
            server h(2004);
            cout<<"Initiating....."<<endl;
            while (true) {
                cout<<"Opening for clients....."<<endl;
                auto handle = h.accept();
                cout<<"Client# "<< handle<< " in bound!"<<endl;
                boost::system::error_code ec;
                while(h[handle]->is_open()) {
                    string in = h.receive(handle, &ec);
                    if(ec) { cerr<<"ERROR: "<<ec<< endl; break; }
                    cout<<"[R] "<<in<<endl;
                    h.send(handle, "[S] " + in, &ec);
                    if(ec) { cerr<<"ERROR: "<<ec<< endl; break; }
                }
                h.close(handle);
                cout<<"Client# "<< handle<< " closed!"<<endl;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
