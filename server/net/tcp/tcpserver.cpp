#include "tcpserver.h"

namespace net{
    void TcpServer::start(){
        _acceptor.listen();
    }
    void TcpServer::newConnection(SocketInfor clientInfor){
        std::shared_ptr<Event> eventManager = _eventManagers.get();
        Channel* channel = eventManager->getFreeChannel();
        assert(channel != nullptr);

        std::string name(clientInfor.getAddrHost());
        name.push_back(':');
        name.append(std::to_string(clientInfor.getPortHost()));

        std::shared_ptr<TcpConnection> conn = std::make_shared<TcpConnection>(name, clientInfor.fd(), channel, eventManager->getID());
        conn->setConnectionCallBack(_connectionCallBack);
        conn->setMessageProcessor(_messageProcessor);
        conn->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
        conn->enable();
        if(_connectionCallBack) {
            _connectionCallBack(conn.get());
        }
        _connectionMap[name] = conn;
        std::cout<<"here is thread: "<<conn->getOwnerThreadID()<<std::endl;
        std::cout<<name<<" comes. "<<"total connection: "<<_connectionMap.size()<<std::endl;
    }
    void TcpServer::setConnectionCallBack(TcpConnection::ConnectionCallBack cb){
        _connectionCallBack = std::move(cb);
    }
    void TcpServer::setMessageProcessor(TcpConnection::MessageProcessor cb){
        _messageProcessor = std::move(cb);
    }
    void TcpServer::removeConnection(TcpConnection* conn){
        std::string name(conn->name());
        _connectionMap.erase(conn->name());
        std::cout<<"here is thread: "<<conn->getOwnerThreadID()<<std::endl;
        std::cout<<name<<" leaves. "<<"total connection: "<<_connectionMap.size()<<std::endl;
    }
}