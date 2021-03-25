#pragma once
#include "event.h"
#include "socketinfor.h"
#include <functional>
#include <assert.h>
#include <string.h> //bzero
#include "acceptor.h"
namespace net{
    
    class AcceptorV6: public Acceptor{
    public:
        using ConnectionCallBack = std::function<void(SocketInfor)>;
        void init(int port)override{
            sockaddr_in6 sin;
            int lfd = socket(AF_INET6, SOCK_STREAM, 0);
            _listenerFd = lfd;
            bzero(&sin, sizeof(sin));
            sin.sin6_family = AF_INET6;
            sin.sin6_addr = in6addr_any;
            sin.sin6_port = htons(port);
            int opt = 1;
            setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
            bind(lfd, (sockaddr*)&sin, sizeof(sin));
            Channel* channel = _listenEventManager.getFreeChannel();
            assert(channel != nullptr);
            channel->bind(lfd);
            channel->addToPoller(EPOLLIN);
            channel->setReadCallBack(std::bind(&Acceptor::handleRead, this));
        }
        void handleRead(){
            sockaddr_in6 cin;
            socklen_t len = sizeof(cin);
            int cfd = ::accept(fd(), (sockaddr*)&cin, &len);
            SocketInfor newClient(cin, cfd);
            if(_connectionCallBack){
                _connectionCallBack(newClient);
            }
        }
    };
}