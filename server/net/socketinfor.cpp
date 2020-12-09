#include "socketinfor.h"

namespace net{
    const std::string SocketInfor::getAddrHost() const{
        return std::string(inet_ntoa(_addr));
    }
    const uint16_t SocketInfor::getPortHost() const{
        return ntohs(_port);
    }
    const in_addr SocketInfor::getAddrNet() const{
        return _addr;
    }
    const uint16_t SocketInfor::getPortNet() const{
        return _port;
    }
    const int SocketInfor::fd() const{
        return _socketFd;
    }
}