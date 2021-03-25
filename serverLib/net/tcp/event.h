#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "channel.h"
#include "epollpoller.h"
#include <thread>
#include <mutex>
#include <boost/noncopyable.hpp>
namespace net{
    class Event : private boost::noncopyable{
    public:
        using ChannelList = std::vector<Channel*>;
        Event(int eventNum = 1024): _channels(eventNum),
                                    _activeChannels(eventNum),
                                    _poller(std::make_shared<EpollPoller>(eventNum)),
                                    _tid(std::this_thread::get_id()){
            for(int i = 0; i < eventNum; i++){
                std::lock_guard<std::mutex> lg(_mutex);
                _channels[i] = new Channel(_poller);
            }
        };
        
        void loop();
        Channel* getFreeChannel();
        ~Event(){
            for(int i = 0; i < _channels.size(); i++){
                delete _channels[i];
                _channels[i] = nullptr;
            }
        }
        const std::thread::id getID()const;
    private: 
        std::mutex _mutex;
        ChannelList _channels;
        ChannelList _activeChannels;
        std::shared_ptr<Poller> _poller;
        const std::thread::id _tid;
    };
};
