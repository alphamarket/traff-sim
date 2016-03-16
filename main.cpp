#include "inc/stdafx.hpp"
#include <thread>
#include <signal.h>

#include "inc/city.hpp"

#define SOCKET_MAIN
#ifdef SOCKET_MAIN

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ServerHost {
    const size_t _port;
    size_t _socket_count = 0;
    boost::asio::io_service _io_service;
    unordered_map<size_t, tcp::socket*> _sockets;
public:
    ServerHost(size_t port) : _port(port){ }
    size_t accept() {
        tcp::socket* socket = new tcp::socket(this->_io_service);
        tcp::acceptor acceptor(this->_io_service, tcp::endpoint(tcp::v4(), this->_port));
        acceptor.accept(*socket);
        this->_sockets[++this->_socket_count] = socket;
        return this->_socket_count;
    }
    string receive(size_t socket_id, boost::system::error_code* ec = nullptr, string until = "\n") {
        boost::asio::streambuf sb;
        tcp::socket* socket = this->get_socket(socket_id);
        boost::asio::read_until(*socket, sb, until, *ec);
        stringstream ss;
        ss << &sb;
        return ss.str();
    }

    size_t send(size_t socket_id, string msg, boost::system::error_code* ec = nullptr) {
        return boost::asio::write(*this->get_socket(socket_id), boost::asio::buffer(msg), *ec);
    }

    tcp::socket* get_socket(size_t id) {
        if(!this->_sockets.count(id)) throw runtime_error("undefined socket id!");
        return this->_sockets[id];
    }

    void close(size_t socket_id) {
        tcp::socket* socket = this->get_socket(socket_id);
        socket->close();
        this->_sockets.erase(socket_id);
        delete socket;
    }

    bool is_open(size_t socket_id) {
        return this->get_socket(socket_id)->is_open();
    }
    tcp::socket* operator [](size_t socket_id) { return this->get_socket(socket_id); }
    tcp::socket* operator ()(size_t socket_id) { return this->get_socket(socket_id); }
};

void server_thread() {
    try
    {
        {
            ServerHost h(2004);
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
}

#include <boost/thread.hpp>

int main() {
    boost::thread_group tg;
    tg.create_thread(server_thread);
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
