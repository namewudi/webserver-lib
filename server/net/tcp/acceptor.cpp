#include "acceptor.h"

namespace net{
    void Acceptor::listen(){
        ::listen(fd(), 20);
        _listenEventManager.loop();
    }
    const int Acceptor::fd()const{
        return _listenerFd;
    }
    void Acceptor::setConnectionCallBack(ConnectionCallBack cb){
        _connectionCallBack = std::move(cb);
    }
    void Acceptor::handleRead(){
        sockaddr_in cin;
        socklen_t len = sizeof(cin);
        int cfd = ::accept(fd(), (sockaddr*)&cin, &len);
        SocketInfor newClient(cin, cfd);
        if(_connectionCallBack){
            _connectionCallBack(newClient);
        }
    }
}