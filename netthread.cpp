#include "netthread.h"

netThread::netThread(std::shared_ptr<arduinoControl> ac)
{
    AC = ac;
}
void netThread::setDelay(int Delay)
{
    delay = Delay;
    if(UDP_server)
        UDP_server->setDelay(Delay);
}
void netThread::setTime(int Time)
{
    time = Time;
    if(UDP_server)
        UDP_server->setTime(Time);
}
void netThread::run()
{
    
    try{
        boost::asio::io_service io_service;

        UDP_server = std::make_shared<udp_server>(io_service,AC);
        UDP_server->setDelay(delay);
        UDP_server->setTime(time);
        io_service.run();

    }
    catch(std::exception& e)
    {
        std::cerr<< e.what() << std::endl;
    }
}


