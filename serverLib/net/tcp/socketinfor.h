#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
namespace net{
    enum class IpType{
        IPV4, IPV6
    };
    class SocketInfor{
    public:
        SocketInfor(const sockaddr_in& sa, const int& fd): _socketFd(fd),
                                                           _addr4(sa.sin_addr),
                                                           _port(sa.sin_port),
                                                           _ipType(IpType::IPV4) {}
        SocketInfor(const sockaddr_in6& sa, const int& fd): _socketFd(fd),
                                                           _addr6(sa.sin6_addr),
                                                           _port(sa.sin6_port),
                                                           _ipType(IpType::IPV6){}
        const std::string getAddrHost() const;
        //const in_addr getAddrNet() const;
        const uint16_t getPortHost() const;
        const uint16_t getPortNet() const;
        const int fd() const;
    private:
        const int _socketFd;
        in_addr _addr4;
        in6_addr _addr6;
        const uint16_t _port;
        IpType _ipType;
    };
}