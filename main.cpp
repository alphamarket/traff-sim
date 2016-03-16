#include "inc/stdafx.hpp"
#include <thread>
#include <signal.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

#include "inc/city.hpp"

#define SOCKET_MAIN
#ifdef SOCKET_MAIN


using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{

    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2004));

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    acceptor.accept(socket);


    std::string message = "DARIUSH: " + make_daytime_string();

    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(message), ignored_error);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request, response;
    std::ostream request_stream(&request), response_stream(&response);
    request_stream << "GET " << "127.0.0.1" << " HTTP/1.0\r\n";
    request_stream << "Host: " << "127.0.0.1" << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, request);
    request_stream << "GET " << "127.0.0.1" << " HTTP/1.0\r\n";
    request_stream << "Host: " << "127.0.0.1" << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n????";
    boost::asio::write(socket, request);

    while(socket.is_open()) {
        boost::system::error_code error;
        std::string s;
        while (boost::asio::read_until(socket, response, "\n", error)) {
            std::istream is(&response);
            is >> s;
            std::cout << "[RECEIVE]" << s <<endl;
            request_stream << "[SENT]" << s <<endl;
            boost::asio::write(socket, request);
        }
    }


    return 0;
}

#else

atomic<bool> stop(false);

int main(int argc, char** argv) {
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
    {
        city c(24, 24);
        thread t([&]() {
            while(!stop) {
                string s;
                cin >> s;
                if(s.length() < 2) { clear_screen(); cout<<(s + " : [ IGNORED ]\n"); continue; }
                switch(s[0]) {
                case 's':
                    c.time_step(stof(s.substr(1)));
                    cout << "\nDONE\n";
                    break;
                case 'd':
                    c.cluster_delay(stof(s.substr(1)));
                    cout << "\nDONE\n";
                    break;
                }
            }
        });
        t.detach();
        cout<<c.add_cars(10)<<endl;
        cout<<c.status()<<endl;
        c.flow_start();
        signal(SIGINT, [](int) { cout<<"EXITING....."<<endl; stop = true; });
        while(!stop);
    }
    cout<<"EXITED!"<<endl;
    return EXIT_SUCCESS;
}

#endif
