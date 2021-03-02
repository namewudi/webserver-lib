#pragma once
#include "channel.h"
#include <assert.h>
#include <functional>
#include <sys/epoll.h>
#include <mutex>
#include <thread>
#include "../base/threadpoolSingleton.h"
namespace net{
    class TcpConnection{
    public:
        using MessageProcessor = std::function<void(TcpConnection*, std::shared_ptr<base::CircleReadBuffer>)>;
        using ConnectionCallBack = std::function<void(const TcpConnection*)>;
        using CloseCallBack = std::function<void(TcpConnection*)>;
        TcpConnection(const std::string& name, int fd, Channel* channel, std::thread::id id): _fd(fd),
                                                                          _channel(channel),
                                                                          _name(name),
                                                                          _ownerThreadID(id),
                                                                          _messageProcssing(false){                                                                    
            assert(channel->isFree());
            channel->bind(fd);
            channel->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
            channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
            channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
            channel->setErrorCallBack(std::bind(&TcpConnection::handleError, this));
        }
        ~TcpConnection(){
            _channel->destroy();
            std::cout<<"close connection. fd = "<<_fd<<std::endl;
        }
        void enable();
        void send(std::string);
        void sendInfinite(std::string);
        void setConnectionCallBack(ConnectionCallBack);
        void setMessageProcessor(MessageProcessor);
        void setCloseCallBack(CloseCallBack);
        const std::string name()const;
        void startMessageProcss(){
            _messageProcssing = true;
        }
        void finishMessageProcss(){
            _messageProcssing = false;
        }
        bool messageProcssing(){
            return _messageProcssing;
        }
        void handleRead();
        void handleWrite();
        void handleClose();
        void handleError();
        void finishSend();
        const std::thread::id getOwnerThreadID()const;
        const int fd() const;
    private:
        int _fd;
        const std::thread::id _ownerThreadID;
        ConnectionCallBack _connectionCallBack;
        MessageProcessor _messageProcessor;
        bool _messageProcssing;
        CloseCallBack _closeCallBack;
        Channel* _channel;
        const std::string _name;
    };
}