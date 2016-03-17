#include "inc/server.hpp"

#include <boost/array.hpp>

SOCKET server::accept() {
    tcp::socket* socket = new tcp::socket(*this->_io_service);
    tcp::acceptor acceptor(*this->_io_service, tcp::endpoint(tcp::v4(), this->_port));
    acceptor.accept(*socket);
    if(this->_socket_count > this->_sockets.size())
        this->_socket_count = this->_sockets.size();
    this->_sockets[++this->_socket_count] = socket_ptr(socket);
    return this->_socket_count;
}

string server::receive(SOCKET handler, boost::system::error_code& ec) {
    stringstream ss;
    boost::asio::streambuf sb;
    socket_ptr socket = this->get_socket(handler);
    while (boost::asio::read(*socket, sb,
          boost::asio::transfer_at_least(1), ec))
        ss << &sb;
    return ss.str();
}

string server::receive(SOCKET handler, boost::system::error_code& ec, string until) {
    boost::asio::streambuf sb;
    socket_ptr socket = this->get_socket(handler);
    boost::asio::read_until(*socket, sb, until, ec);
    stringstream ss;
    ss << &sb;
    return ss.str();
}

string server::receive(SOCKET handler, boost::system::error_code& ec, size_t& size) {
    stringstream ss;
    socket_ptr socket = this->get_socket(handler);
    boost::array< char, 1024 > buf;
    size = boost::asio::read(*socket, boost::asio::buffer(buf), boost::asio::transfer_all(), ec);
    ss << buf.data();
    return ss.str();
}

void server::close(SOCKET handler) {
    socket_ptr socket = this->get_socket(handler);
    socket->close();
    this->_sockets.erase(handler);
}
