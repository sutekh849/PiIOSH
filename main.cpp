 
#include <cstdlib>
#include <iostream>
#include "arduinocontrol.h"
#include "netthread.h"
int main(int argc, char ** argv)
{
    auto ac = std::make_shared<arduinoControl>();
    if(!ac->activate())
        return EXIT_FAILURE;
    auto nt = std::make_shared<netThread>(ac);
    
    nt -> setTime(2000);
    nt -> setDelay(2000);
    nt -> run();
    
    
   return EXIT_SUCCESS;
}
