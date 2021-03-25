#include "tcpserver.h"

namespace net{
    void TcpServer::start(){
        _acceptor->listen();
    }
    void TcpServer::newConnection(SocketInfor clientInfor){
        std::shared_ptr<Event> eventManager = _eventManagers.get();
        Channel* channel = eventManager->getFreeChannel();
        if(channel == nullptr){
            return;//channel用尽
        }

        std::string name(clientInfor.getAddrHost());
        name.push_back(':');
        name.append(std::to_string(clientInfor.getPortHost()));
        name.push_back(':');
        name.append(std::to_string(clientInfor.fd()));

        std::shared_ptr<TcpConnection> conn = std::make_shared<TcpConnection>(name, clientInfor.fd(), channel, eventManager->getID());
        conn->setConnectionCallBack(_connectionCallBack);
        conn->setMessageProcessor(_messageProcessor);
        conn->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
        {
            std::lock_guard<std::mutex> lg(_mutex);
            _connectionMap[name] = conn;
        }
        conn->enable();
        if(_connectionCallBack) {
            _connectionCallBack(conn);
        }
        
        //std::cout<<"here is thread: "<<conn->getOwnerThreadID()<<std::endl;
        std::cout<<base::TimeStamp::getNowAsString()<<" : "<<name<<" comes. "<<"total connection: "<<_connectionMap.size()<<std::endl;
    }
    void TcpServer::setConnectionCallBack(TcpConnection::ConnectionCallBack cb){
        _connectionCallBack = std::move(cb);
    }
    void TcpServer::setMessageProcessor(TcpConnection::MessageProcessor cb){
        _messageProcessor = std::move(cb);
    }
    void TcpServer::removeConnection(std::shared_ptr<TcpConnection> conn){
        std::string name(conn->name());
        auto id = conn->getOwnerThreadID();
        {
            std::lock_guard<std::mutex> lg(_mutex);
            _connectionMap.erase(name);
        }   
        //std::cout<<"here is thread: "<<id<<std::endl;
        std::cout<<base::TimeStamp::getNowAsString()<<" : "<<name<<" leaves. "<<"total connection: "<<_connectionMap.size()<<std::endl;
    }
}