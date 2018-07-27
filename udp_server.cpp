#include "udp_server.h"
using namespace boost::asio::ip;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udp_server::udp_server(boost::asio::io_service& io_context, std::shared_ptr<arduinoControl> AC) :socket_(io_context, udp::endpoint(udp::v4(),port))
{
    std::thread p(&udp_server::pulse, this);
    p.detach();
    ac = AC;
    start_recieve();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void udp_server::start_recieve()
{

    socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                boost::bind(&udp_server::handle_recieve,this,boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred)
                );
    std::cout << "Listening on port " << port << ".\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void udp_server::handle_recieve(const boost::system::error_code& error, std::size_t bytes_transferred)
{

     std::cerr << "recieving. number of bytes: "<<bytes_transferred << std::endl;

    if((!error || error == boost::asio::error::message_size) && bytes_transferred != 0)
    {
        std::cerr <<"packet recieve from unity..";
        for(size_t i =0; i< bytes_transferred; i++)
        {
            std::cerr << std::to_string(recv_buffer_[i]);
        }
        std::cerr<<  std::endl;
        int angle = 0;
        int door1Angle = 0;
        int door2Angle = 180;
        //handle incoming messages by writing to the pins.
        for(size_t i = 0; i < bytes_transferred;i++)
            //this is a bit more elegant than a huge switch case. 
            if(recv_buffer_[i] <= 60 && recv_buffer_[i] > 50)
            {
                std::cerr <<"recieved: "<< (int)recv_buffer_[i] <<'\n';
                 angle = (recv_buffer_[i]-51) * 10; //eg 51 = 0, 52 = 10, 53 = 20, 54 = 30, 55=40, 56 = 50, 57 = 60, 58 = 70, 59 = 80 and 60 = 90 degrees.
                 door1Angle = 0 + angle;
                 door2Angle = 180 - angle;
                 std::cerr << "angle = " << angle <<'\n';
                 std::cerr << "door 1 angle:" << door1Angle << '\n';
                 std::cerr << "door 2 angle:" << door2Angle << '\n';
                 ac->setDigitalPin(8,door1Angle);
                 ac->setDigitalPin(9,door2Angle);
                 ac->setDigitalPin(10,door1Angle);
                 ac->setDigitalPin(11,door2Angle);
                 ac->setDigitalPin(12,door1Angle);
                 ac->setDigitalPin(13,door2Angle);
            }
            else if(recv_buffer_[i] == 67 || recv_buffer_[i] == 66)
            {
                scentActive = true;
            }
    }
    else
    {
        if(error)
            std::cerr<<error.message() << std::endl;

    }
    start_recieve();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void udp_server::pulse()
{
    while(true)
    {
        if(scentActive)
        {
            scentActive = false;
            std::cerr << "Activating Diffuser\n";
            //activate diffuser
            ac->switchDiffuser(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(cyclic_rate));
            //deactivate diffuser
            ac->switchDiffuser(false);
            std::this_thread::sleep_for(std::chrono::milliseconds(duration));

        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void::udp_server::setDelay(unsigned int delay)
{
    std::cerr << "setting delay time to " << delay << "\n";
    duration = delay;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void::udp_server::setTime(int time)
{
    std::cerr << "setting diffuser time to " << time << "\n";
    cyclic_rate = time;
}
