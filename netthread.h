#ifndef NETTHREAD_H
#define NETTHREAD_H
#include "udp_server.h"
#include "arduinocontrol.h"
#include <boost/asio.hpp>
//Class to run the thread containing the networking system and to send appropriate
//messages to the arduino
class netThread
{
public:
    netThread(std::shared_ptr<arduinoControl> ac);
    void run();
    void setTime(int Time);
    void setDelay(int Delay);
    std::shared_ptr<udp_server> UDP_server= nullptr;
private:
    int time = 2000;
    int delay = 2000;
    std::shared_ptr<arduinoControl> AC;

};

#endif // NETTHREAD_H
