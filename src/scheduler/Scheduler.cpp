#include "Scheduler.h"
#include "SomeipService.h"
#include "CanInterface.h"
#include "Tasks.h"
#include "udpSock.h"
#include "DoIP.h"
#include <iostream>
//bogdan
void Scheduler::initialize (void) 
{  
    highResTimer.start();
}

void Scheduler::run (void) 
{
    UDPSocket receiveSocket(12345, true);

    while (!exitCondition) 
    {
        if (eventSetter.check1msEvent()) 
        {
            execute1msTask();
        }

        if (eventSetter.check10msEvent()) 
        {
            execute10msTask();
        }

        std::string message = receiveSocket.receive();

#if TEST_SESSION_ACTIVE == 0

        SomeIPMessage request_msg = receive_someip_msg();

        if (REQUEST == request_msg.someip_header.message_type)
        {
            printf("Request for pedal status received");

            SomeIPMessage response_msg;
            response_msg.someip_header.message_id = 1;
            response_msg.someip_header.length = 2;
            response_msg.someip_header.protocol_version = 1;
            response_msg.someip_header.message_type = RESPONSE;
            response_msg.someip_header.return_code = 0;

            std::memcpy(response_msg.someip_payload, "OK", 2);
            
            send_someip_msg(&response_msg, "192.168.1.10", 12346);
        }

        else
        {
            std::cout << "No valid request received" << std::endl;
        }
#elif TEST_SESSION_ACTIVE == 1
        DoIPMessage request_msg = receive_doip(13400);
        
        if (0x0003 == request_msg.doip_header.payload_type)
        {
            std::cout << "Received vin request" << std::endl;

            DoIPMessage response_msg;
            response_msg.doip_header.protocol_version = 0x02;
            response_msg.doip_header.inverse_protocol_version = 0xFD;
            response_msg.doip_header.payload_type = 0x0004; /* vin response */
            response_msg.doip_header.payload_length = 1;
            response_msg.doip_payload[0] = 0x11;

            send_doip(&response_msg, "192.168.1.10", 13401);
        } 
        

#endif

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
