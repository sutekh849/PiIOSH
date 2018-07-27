#include "arduinocontrol.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arduinoControl::~arduinoControl()
{
    io.run();
    if (f!= nullptr)
    {

        f.reset();

        f = nullptr;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arduinoControl::arduinoControl()
{
    //virtual constructor sufficient
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arduinoControl::setCOMport(int id)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arduinoControl::keepAlive()
{
	while(true)
	{
		if(f != nullptr)
		{
			setDigitalPin(8,door1);
			setDigitalPin(9,door2);
			setDigitalPin(10,door3);
			setDigitalPin(11,door4);
            setDigitalPin(10,door5);
            setDigitalPin(11,door6);
		}
        else
        {
            std::cerr <<" f null, keepalive thread breaking.."<<std::endl;
            break;
        }
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool arduinoControl::activate()
{
        ports = firmata::FirmSerial::listPorts();
    for (auto &port : ports) {
            std::cerr << port.port << std::endl;

            if (f != nullptr) {
                f.reset();
                f = nullptr;
            }

            try {
                serialio = std::make_shared<firmata::FirmSerial>(port.port.c_str());

    #ifndef WIN32
                if (serialio->available()) {
                    sleep(3); // Seems necessary on linux
                    f = std::make_shared<firmata::Firmata<firmata::Base, firmata::I2C>>(serialio.get());
                }
    #else

                f = std::make_shared < firmata::Firmata<firmata::Base, firmata::I2C>>(serialio.get());
    #endif
               // f->setSamplingInterval(5700);
			   
            }
            catch(firmata::IOException e) {
                std::cerr << e.what() << std::endl;
                if(f!= nullptr)
                {
                    f.reset();
                    f = nullptr;
                }
                return false;
            }
            catch(firmata::NotOpenException e) {
                std::cerr << e.what() << std::endl;
                if(f!= nullptr)
                {
                    f.reset();
                    f=nullptr;
                }
                return false;
            }
            catch(std::exception& e )
            {
                std::cerr << e.what() << std::endl;
                if(f!= nullptr)
                {
                    f.reset();
                    f=nullptr;
                }
                return false;
            }
            if (f != nullptr && f->ready()) {
                break;
            }
        }
    if (f == nullptr || !f->ready()) {
        std::cerr << "f is null or not ready\n";
        if(f!= nullptr)
        {
            std::cerr << "f was not ready \n";
            f.reset();
            serialio->close();

            f=nullptr;
        }
        else
        {

        }
        return false;
    }

	//f->analogWrite(6,255);
	if(!stayinAlive)//first run, prevent threadbomb and listen to the beegees
	{
		stayinAlive = true;
		f->setSamplingInterval(5700);
		f->pinMode(7,MODE_PWM);
		f->pinMode(6,MODE_PWM);
		f->pinMode(9,MODE_SERVO);
		f->pinMode(8,MODE_SERVO);
		f->pinMode(10,MODE_SERVO);
		f->pinMode(11,MODE_SERVO);
        f->pinMode(12,MODE_SERVO);
        f->pinMode(13,MODE_SERVO);
		f->analogWrite(8,door1);
		f->analogWrite(9,door2);
		f->analogWrite(10,door3);
		f->analogWrite(11,door4);
        f->analogWrite(12,door5);
        f->analogWrite(13,door6);
		std::thread p (&arduinoControl::keepAlive,this);
		p.detach();
	}
    return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void arduinoControl::switchDiffuser(bool on)
{
    try{
        if(f!= nullptr)
        {
            if(on)
            {
                std::cerr << "activating diffuser\n";
                f->analogWrite(6,255);
            }
            else
            {
                std::cerr << "deactivating diffuser..\n";
                f->analogWrite(6,0);
            }
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << "\n attempting diffuser shutdown.." << std::endl;
        try {
             f->analogWrite(6,0);

        } catch (std::exception &e) {
            std::cerr << e.what() << "\n shutdown failed." << std::endl;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arduinoControl::setDigitalPin(uint8_t id, uint32_t value)
{
	try{
		if(f!=nullptr)
		{
			f->analogWrite(id,value);
			if(id == 8)
			{
				door1 = value;
			}
			else if(id == 9)
			{
				door2 = value;
			}
			else if (id==10)
			{
				door3 = value;
			}
			else if(id==11)
			{
				door4 = value;
			}
            else if(id==12)
            {
                door5 = value;
            }
            else if(id ==13)
            {
                door6 = value;
            }
		}
	}
	catch (firmata::IOException e)
    {
        exceptionHandler(e);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arduinoControl::exceptionHandler(firmata::IOException e)
{

	if(!activate())
		std::cerr<< "Program failed, please reset the program" << std::endl;
	
}
