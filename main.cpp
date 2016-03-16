#include "inc/stdafx.hpp"
#include <thread>
#include <signal.h>

#include "inc/city.hpp"
#include "inc/server.hpp"
#include "inc/http_request.hpp"

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

#define MULTILINE(...) #__VA_ARGS__
        {
            string in = MULTILINE(
                                    POST / HTTP/1.1\n
                                    Host: 127.0.0.1:2004\n
                                    Connection: keep-alive\n
                                    Content-Length: 6\n
                                    Pragma: no-cache\n
                                    Cache-Control: no-cache\n
                                    Origin: http:\\\\127.0.0.1:3333\n
                                    User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.80 Safari/537.36\n
                                    Content-Type: application/x-www-form-urlencoded; charset=UTF-8\n
                                    Referer: http:\\\\127.0.0.1:3333/\n
                                    Accept-Encoding: gzip, deflate\n
                                    Accept-Language: en-US,en;q=0.8,fa;q=0.6,ru;q=0.4\n\n
                                    id=122);
            http_request x(in);
            return 0;
        }
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
