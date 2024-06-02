#include <stdio.h>
#include <stdlib.h>
#include <CanInterface.h>

CanInterface::CanInterface (const std::string& interface) 
    : interface_(interface),
      socket_(-1) {}

bool CanInterface::initialize (void)
{
    struct ifreq ifr;
    struct sockaddr_can addr;
    bool retVal = true;

    socket_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (socket_ < 0)
    {
        std::cout << "error opening can socket" << std::endl;
        retVal = false;
    }

    strcpy(ifr.ifr_name, interface_.c_str());
    ioctl(socket_, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if ( bind (
         socket_,
         (struct sockaddr *)&addr,
         sizeof(addr) ) < 0 )
    {
        std::cout << "error in can socket bind" << std::endl;
        retVal = false;
    }

    return retVal;
}

bool CanInterface::readMessage (struct can_frame& can_socket_frame)
{
    int no_bytes = read(socket_, &can_socket_frame, sizeof(struct can_frame));
    bool retVal = true;

    if (no_bytes < 0)
    {
        std::cout << "CAN reading error" << std::endl;
        retVal = false;
    }

    return retVal;
}