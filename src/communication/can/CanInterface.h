#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <linux/can.h>
#include <string>
#include <iostream>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

class CanInterface {
    public:
        CanInterface      (const std::string& interface)       ; 
        bool initialize   (void)                               ;
        bool readMessage  (struct can_frame& can_socket_frame) ;

    private:
        std::string interface_;
        int socket_;

};

#endif // CAN_INTERFACE_H
