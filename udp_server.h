#ifndef UDP_SERVER_H
#define UDP_SERVER_H
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include "arduinocontrol.h"
class udp_server
{
public:

    udp_server(boost::asio::io_service& io_context,std::shared_ptr<arduinoControl> AC);
    void setDelay(unsigned int delay);
    void setTime(int time);
private:
    std::shared_ptr<arduinoControl> ac;
    void start_recieve();
    void handle_recieve(const boost::system::error_code& error, std::size_t);
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    boost::array<char,1024> recv_buffer_;
    unsigned int cyclic_rate = 2000; //mili-seconds.
    unsigned int duration = 2000;
    bool scentActive = false;
    static const unsigned short port = 5005;
    void pulse();
};

#endif // UDP_SERVER_H
