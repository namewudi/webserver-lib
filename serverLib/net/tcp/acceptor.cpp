#include "acceptor.h"

namespace net{
    void Acceptor::listen(){
        ::listen(fd(), 100);
        _listenEventManager.loop();
    }
    const int Acceptor::fd()const{
        return _listenerFd;
    }
    void Acceptor::setConnectionCallBack(ConnectionCallBack cb){
        _connectionCallBack = std::move(cb);
    }
}