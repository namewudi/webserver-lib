#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
namespace net{
    
    class SocketInfor{
    public:
        SocketInfor(const sockaddr_in& sa, const int& fd): _socketFd(fd),
                                                           _addr(sa.sin_addr),
                                                           _port(sa.sin_port) {}
        const std::string getAddrHost() const;
        const in_addr getAddrNet() const;
        const uint16_t getPortHost() const;
        const uint16_t getPortNet() const;
        const int fd() const;
    private:
        const int _socketFd;
        const in_addr _addr;
        const uint16_t _port;
    };
}