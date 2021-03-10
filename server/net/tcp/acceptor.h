#pragma once
#include "event.h"
#include "socketinfor.h"
#include <functional>
#include <assert.h>
#include <string.h> //bzero
namespace net{
    
    class Acceptor{
    public:
        using ConnectionCallBack = std::function<void(SocketInfor)>;
        Acceptor(int port): _listenEventManager(1){
            sockaddr_in sin;
            int lfd = socket(AF_INET, SOCK_STREAM, 0);
            _listenerFd = lfd;
            bzero(&sin, sizeof(sin));
            sin.sin_family = AF_INET;
            sin.sin_addr.s_addr = INADDR_ANY;
            sin.sin_port = htons(port);
            int opt = 1;
            setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
            bind(lfd, (sockaddr*)&sin, sizeof(sin));
            Channel* channel = _listenEventManager.getFreeChannel();
            assert(channel != nullptr);
            channel->bind(lfd);
            channel->addToPoller(EPOLLIN);
            channel->setReadCallBack(std::bind(&Acceptor::handleRead, this));
        }
        void listen();
        void handleRead();
        void setConnectionCallBack(ConnectionCallBack);
        const int fd()const;
        
    private:
        ConnectionCallBack _connectionCallBack;
        Event _listenEventManager;
        int _listenerFd;
    };
}