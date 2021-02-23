#include "tcpconnection.h"

namespace net{
    void TcpConnection::handleRead(){
        std::shared_ptr<base::CircleReadBuffer> inputBuffer = _channel->inputBuffer();
        if(inputBuffer->readFromFd(_channel->fd()) > 0){
            _channel->removeFromPoller();
            if(_messageProcessor) {
                base::ThreadPoolSingleton::get().execute([this](){_messageProcessor(this, _channel->inputBuffer());});
            }
        }
        else {
            handleClose();
        }
    }
    void TcpConnection::handleWrite(){
        std::shared_ptr<base::CircleWriteBuffer> outputBuffer = _channel->outputBuffer();
        std::cerr<<"start handle write!"<<std::endl;
        outputBuffer->writeToFd(_channel->fd());
        if(outputBuffer->byteToWrite() == 0){
            _channel->removeFromPoller();
            _channel->addToPoller(EPOLLIN);
        }
        
    }
    void TcpConnection::handleClose(){
        _channel->destroy();
        if(_closeCallBack) _closeCallBack(this);
    }
    void TcpConnection::handleError(){

    }
    void TcpConnection::send(std::string s){
        std::shared_ptr<base::CircleWriteBuffer> outputBuffer = _channel->outputBuffer();
        outputBuffer->append(s);
    }
    void TcpConnection::finishSend(){
        _channel->addToPoller(EPOLLOUT);
    }
    void TcpConnection::setConnectionCallBack(ConnectionCallBack cb){
        _connectionCallBack = std::move(cb);
    }
    void TcpConnection::setMessageProcessor(MessageProcessor cb){
        _messageProcessor = std::move(cb);
    }
    void TcpConnection::setCloseCallBack(CloseCallBack cb){
        _closeCallBack = std::move(cb);
    }
    void TcpConnection::enable(){
        _channel->addToPoller(EPOLLIN);
    }
    const int TcpConnection::fd()const{
        return _fd;
    }
    const std::string TcpConnection::name()const{
        return _name;
    }
    const std::thread::id TcpConnection::getOwnerThreadID()const{
        return _ownerThreadID;
    }
}