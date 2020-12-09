#include "channel.h"

namespace net{
    bool Channel::Monitored()const{ 
        return _beingMonitored;
    }
    bool Channel::isFree(){
        return _unused;
    }
    void Channel::updateEventType(int eventType){
        _event = eventType;
    }
    void Channel::destroy(){
        removeFromPoller();
        close(_fd);
        clear();
        _unused = 1;
    }
    void Channel::bind(int eventFd){
        _unused = 0;
        _fd = eventFd;
    }
    void Channel::clear(){
        _inputBuffer.clear();
        _outputBuffer.clear();
        _beingMonitored = 0;
        _fd = -1;
        _readCallBack = nullptr;
        _writeCallBack = nullptr;
        _closeCallBack = nullptr;
        _errorCallBack = nullptr;
    }
    void Channel::setReadCallBack(EventCallBack cb){
        _readCallBack = std::move(cb);
    }
    void Channel::setWriteCallBack(EventCallBack cb){
        _writeCallBack = std::move(cb);
    }
    void Channel::setCloseCallBack(EventCallBack cb){
        _closeCallBack = std::move(cb);
    }
    void Channel::setErrorCallBack(EventCallBack cb){
        _errorCallBack = std::move(cb);
    }
    const int Channel::fd() const{
        return _fd;
    }
    const int Channel::eventType(){
        std::lock_guard<std::mutex> lg(_mutex);
        return _event;
    }
    void Channel::addToPoller(int eventType){
        std::lock_guard<std::mutex> lg(_mutex);
        if(!Monitored()){
            assert(_owner->addChannel(this, _fd, eventType));
            _beingMonitored = 1;
        }
        else{
            assert(_owner->modChannel(this, _fd, eventType));
        }
        updateEventType(eventType);
    }
    void Channel::removeFromPoller(){
        std::lock_guard<std::mutex> lg(_mutex);
        if(Monitored()){
            assert(_owner->removeChannel(_fd));
            updateEventType(0);
            _beingMonitored = 0;
        }
    }
    void Channel::doTask(){
        int event = eventType();
        if(_owner->isCloseEvent(event)){
            if(_closeCallBack) _closeCallBack();
        }
        else if(_owner->isErrorEvent(event)){
            if(_errorCallBack) _errorCallBack();
        }
        else if(_owner->isInEvent(event)){
            if(_readCallBack) _readCallBack();
        }
        else if(_owner->isOutEvent(event)){
            if(_writeCallBack) _writeCallBack();
        }
    }
    base::Buffer* Channel::inputBuffer(){
        return &_inputBuffer;
    }
    base::Buffer* Channel::outputBuffer(){
        return &_outputBuffer;
    }
}