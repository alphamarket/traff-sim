#ifndef SERVER_H
#define SERVER_H

#include "stdafx.hpp"
#include <unordered_map>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

typedef size_t SOCKET;
typedef shared_ptr<tcp::socket> socket_ptr;

class server
{
    /**
     * @brief _port the listening port
     */
    const size_t _port;
    /**
     * @brief _socket_count the number of socket that accepted
     */
    SOCKET _socket_count = 0;
    /**
     * @brief _sockets the accepted sockets packages
     */
    unordered_map<SOCKET, socket_ptr> _sockets;
public:
    /**
     * @brief server ctor
     * @param port the listening port
     */
    server(size_t port) : _port(port){ }
    /**
     * @brief accepts a new client
     * @return the client's handler
     */
    SOCKET accept();
    /**
     * @brief close closes a client
     * @param socket_id the client handler
     */
    void close(SOCKET handler);
    /**
     * @brief receives a message from a client
     * @param handler the client handler
     * @param ec the error code output
     * @param until read termination flag
     * @return the read message
     */
    string receive(SOCKET handler, boost::system::error_code* ec = nullptr, string until = "\n");
    /**
     * @brief send sends a message to a client
     * @param handler the client handler
     * @param msg the message to be sent
     * @param ec the error code output
     * @return the number byte sent
     */
    inline size_t send(SOCKET handler, string msg, boost::system::error_code* ec = nullptr) {
        return boost::asio::write(*this->get_socket(handler), boost::asio::buffer(msg), *ec);
    }
    /**
     * @brief get_socket get a client's socket
     * @param handler the client's handler
     */
    inline socket_ptr get_socket(SOCKET handler) const {
        if(!this->_sockets.count(handler)) throw runtime_error("undefined socket id!");
        return this->_sockets.at(handler);
    }
    /**
     * @brief is_open check if a client's open
     * @param handler the client's handler
     */
    inline bool is_open(SOCKET handler) { return this->get_socket(handler)->is_open(); }
    /**
     * @brief operator [] access to a client
     * @param handler the client's handler
     */
    inline socket_ptr operator [](SOCKET handler) { return this->get_socket(handler); }
};

#endif // SERVERHOST_H
