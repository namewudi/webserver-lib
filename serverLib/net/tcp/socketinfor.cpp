#include "socketinfor.h"

namespace net{
    const std::string SocketInfor::getAddrHost() const{
        switch (_ipType)
        {
        case IpType::IPV4 :
            return std::string(inet_ntoa(_addr4));
        case IpType::IPV6 :
            char buffer[1024];
            return std::string(inet_ntop(AF_INET6, &_addr6, buffer, sizeof(buffer)));
        default:
            break;
        }
        
    }
    const uint16_t SocketInfor::getPortHost() const{
        return ntohs(_port);
    }
    // const in_addr SocketInfor::getAddrNet() const{
    //     return _addr;
    // }
    const uint16_t SocketInfor::getPortNet() const{
        return _port;
    }
    const int SocketInfor::fd() const{
        return _socketFd;
    }
}