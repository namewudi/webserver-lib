#include "tcpconnection.h"

namespace net{
    void TcpConnection::handleRead(){
        std::shared_ptr<base::CircleReadBuffer> inputBuffer = _channel->inputBuffer();
        if(inputBuffer->readFromFd(_channel->fd()) != 0){
            //_channel->removeFromPoller();
            if(_messageProcessor != nullptr) {//同时读取多个请求将引发并发问题 fix me
                if(messageProcssing()) return;
                this->startMessageProcss();
                base::ThreadPoolSingleton::get().execute([this](){_messageProcessor(this, _channel->inputBuffer());});
            }
        }
        else {
            std::cerr<<"读不到内容!"<<std::endl;
            handleClose();
        }
    }
    void TcpConnection::handleWrite(){
        std::shared_ptr<base::CircleWriteBuffer> outputBuffer = _channel->outputBuffer();
        std::cerr<<"start handle write!"<<std::endl;
        outputBuffer->writeToFd(_channel->fd());
        // if(outputBuffer->byteToWrite() == 0){
        //     _channel->removeFromPoller();
        //     _channel->addToPoller(EPOLLIN);
        // }
        if(outputBuffer->remainTaskLength() == 0){
            if(_sendCompleteCallBack != nullptr){
                _sendCompleteCallBack(this);
            }
        }
    }
    void TcpConnection::handleClose(){
        std::cout<<"tcp connection handle close"<<std::endl;
        _channel->destroy();
        if(_closeCallBack) _closeCallBack(this);
    }
    void TcpConnection::handleError(){

    }
    void TcpConnection::send(std::string s){
        std::shared_ptr<base::CircleWriteBuffer> outputBuffer = _channel->outputBuffer();
        outputBuffer->append(s);
    }
    void TcpConnection::sendInfinite(std::string s){
        std::shared_ptr<base::CircleWriteBuffer> outputBuffer = _channel->outputBuffer();
        outputBuffer->addTaskLength(s.length());
        _channel->addToPoller(EPOLLOUT | EPOLLIN); //fix me
        outputBuffer->appendInfinite(&s.front(), s.size());
    }
    void TcpConnection::finishSend(){
        _channel->addToPoller(EPOLLOUT | EPOLLIN); //fix me
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