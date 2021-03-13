#pragma once
#include "event.h"
#include "tcpconnection.h"
#include "acceptor.h"
#include <map>
#include <functional>
#include "socketinfor.h"
#include <thread>
#include <mutex>
#include "managerPool.h"
namespace net{
    class TcpServer{
    public:
        TcpServer(int port, int threadNum = 3, int num = 100): _acceptor(port),
                                                               _eventManagers(threadNum, num){
            _acceptor.setConnectionCallBack(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
        }
        void start();
        void setConnectionCallBack(TcpConnection::ConnectionCallBack);
        void setMessageProcessor(TcpConnection::MessageProcessor);
    private:
        void newConnection(SocketInfor);
        void removeConnection(std::shared_ptr<TcpConnection>);
        TcpConnection::ConnectionCallBack _connectionCallBack;
        TcpConnection::MessageProcessor _messageProcessor;
        ManagerPool _eventManagers;
        Acceptor _acceptor;
        std::mutex _mutex;
        std::map<const std::string, std::shared_ptr<TcpConnection>> _connectionMap;
    };
}