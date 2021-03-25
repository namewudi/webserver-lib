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
        virtual void init(int) = 0;
        void listen();
        virtual void handleRead() = 0;
        void setConnectionCallBack(ConnectionCallBack);
        const int fd()const;
    protected:    
        ConnectionCallBack _connectionCallBack;
        Event _listenEventManager;
        int _listenerFd;
    };
}