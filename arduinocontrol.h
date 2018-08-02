#ifndef ARDUINOCONTROL_H
#define ARDUINOCONTROL_H
#include <firmata.h>
#include <firmserial.h>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <iostream>
#include <memory>
/*  This is a class to generate the messages to send ot the arduino over the
 *  firmata serial protocol. It makes use of the firmatacpp library to generate said signals
 *  
 */
class arduinoControl
{
public:
    arduinoControl();
    ~arduinoControl();//hope this is called by the cleanup stuff.
    boost::asio::io_service io;
    bool activate();
    void keepAlive();
    bool stayinAlive = false;
    std::vector<firmata::PortInfo> ports;
    std::shared_ptr<firmata::Firmata<firmata::Base, firmata::I2C>> f = nullptr;
    std::shared_ptr<firmata::FirmSerial> serialio = nullptr;
	std::vector<uint32_t> doors = {0,180,0,180,0,180};
    void switchDiffuser(bool on);
    void setDigitalPin(uint8_t id, uint32_t value);
    void setCOMport(int id);
	void exceptionHandler(firmata::IOException e);
};

#endif // ARDUINOCONTROL_H
