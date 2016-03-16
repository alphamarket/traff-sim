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

#include <boost/asio.hpp>

struct Client
{
    boost::asio::io_service& io_service;
    boost::asio::ip::tcp::socket socket;

    Client(boost::asio::io_service& svc, std::string const& host, std::string const& port)
        : io_service(svc), socket(io_service)
    {
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(boost::asio::ip::tcp::resolver::query(host, port));
        boost::asio::connect(this->socket, endpoint);
    };

    void send(std::string const& message) {
        socket.send(boost::asio::buffer(message));
    }
};


#include <iostream>

static const int PORT = 2004;

void client_thread() {
    boost::asio::io_service svc;
    Client client(svc, "127.0.0.1", std::to_string(PORT));

    client.send("hello world\n");
    client.send("bye world\n");
}

void server_thread() {
    try
    {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));

        {
            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);

            boost::asio::streambuf sb, sr;
            boost::system::error_code ec;
            while (true) {
                boost::asio::read_until(socket, sb, "\n", ec);
                if (ec) {
                    std::cout << "status: " << ec.message() << "\n";
                    break;
                }
                stringstream ss;
                ss << &sb;
                std::cout << "received: '" << ss.str() << "'\n";
                ostream sss(&sr);
                sss << "send: '" << ss.str() << "'\n";
                boost::asio::write(socket, sr, ec);

                if (ec) {
                    std::cout << "status: " << ec.message() << "\n";
                    break;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

#include <boost/thread.hpp>

int main() {
    boost::thread_group tg;
    tg.create_thread(server_thread);

//    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
//    tg.create_thread(client_thread);

    tg.join_all();
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
