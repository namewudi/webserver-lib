#pragma once
#include <iostream>
#include <functional>
#include <memory>
#include <sys/epoll.h>
#include <mutex>
#include "poller.h"
#include <boost/noncopyable.hpp>
#include "../base/buffer.h"
#include "../base/circleReadBuffer.h"
#include "../base/circleWriteBuffer.h"
namespace net{
    class Poller;
    class Channel: private boost::noncopyable{
        using EventCallBack = std::function<void()>;
        //using EventNotifier = std::function<void()>;
    public:
        Channel(std::shared_ptr<Poller> poller) : _owner(poller),
                                                  _unused(1),
                                                  _beingMonitored(0),
                                                  _inputBuffer(std::make_shared<base::CircleReadBuffer>()),
                                                  _outputBuffer(std::make_shared<base::CircleWriteBuffer>()){};
        void setReadCallBack(EventCallBack);
        void setWriteCallBack(EventCallBack);
        void setCloseCallBack(EventCallBack);
        void setErrorCallBack(EventCallBack);
        void addToPoller(int);
        void removeFromPoller();
        void destroy();
        void bind(int);
        void doTask();
        const int fd() const;
        bool Monitored()const;
        void updateEventType(int);
        const int eventType();
        const int getTrigeredEvent()const{
            return _trigeredEvent;
        }
        void setTrigeredEvent(int event){
            _trigeredEvent = event;;
        }
        bool isFree();
        ~Channel(){
            close(_fd);
        }
        std::shared_ptr<base::CircleReadBuffer> inputBuffer();
        std::shared_ptr<base::CircleWriteBuffer> outputBuffer();
    private:
        void clear();
        std::shared_ptr<Poller> _owner;
        bool _unused;
        int _event;
        int _trigeredEvent;
        int _fd;
        bool _beingMonitored;
        //EventNotifier _InEvent;
        //EventNotifier _OutEvent;
        //EventNotifier _CloseEvent;
        //EventNotifier _ErrorEvent;
        EventCallBack _readCallBack;
        EventCallBack _writeCallBack;
        EventCallBack _closeCallBack;
        EventCallBack _errorCallBack;
        std::shared_ptr<base::CircleReadBuffer> _inputBuffer;
        std::shared_ptr<base::CircleWriteBuffer> _outputBuffer;
        std::mutex _mutex;
    };
}
