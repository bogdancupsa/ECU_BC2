#include "Scheduler.h"
#include "SomeipService.h"
// #include "CanService.h"
#include "Tasks.h"
#include "udpSock.h"
#include <iostream>

void Scheduler::initialize (void) 
{  
    highResTimer.start();

    // TODO - initialize canService
}

void Scheduler::run (void) 
{
    while (!exitCondition) 
    {
        if ( eventSetter.check1msEvent() ) 
        {
            execute1msTask();
        }

        if ( eventSetter.check10msEvent() ) 
        {
            execute10msTask();
        }

        SomeIPMessage request_msg = receive_someip_msg();

        if (REQUEST == request_msg.someip_header.message_type)
        {
            printf("Request for pedal status received");

            SomeIPMessage response_msg;
            response_msg.someip_header.message_id = 1;
            response_msg.someip_header.length = 5;
            response_msg.someip_header.protocol_version = 1;
            response_msg.someip_header.message_type = RESPONSE;
            response_msg.someip_header.return_code = 0;

            std::memcpy(response_msg.someip_payload, "OK", 2);

            send_someip_msg(&response_msg, "192.168.1.10", 12345);

        }

    }

}

void Scheduler::shutdown (void) 
{
    highResTimer.stop();
}

int main (void) 
{
    Scheduler scheduler;
    
    scheduler.initialize();
    scheduler.run();
    scheduler.shutdown();

    return 0;
}