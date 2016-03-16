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
                    string in = h.receive(handle, &ec);
                    if(ec) { cerr<<"ERROR: "<<ec<< endl; break; }
                    cout<<"[R] "<<in<<endl;
                    string out = "{\"FUCKU\": 1}";
                    stringstream ss;
                    ss  << "HTTP/1.x 200 OK\n"
                        << "Connection: close\n"
                        << "Content-Type: application/json\n"
                        << "Access-Control-Allow-Origin: *\n"
                        << "Content-Length: " + std::to_string(out.length())
                        << endl << endl;
                    h.send(handle, ss.str() + out, &ec);
                    if(ec) { cerr<<"ERROR: "<<ec<< endl; break; }
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
