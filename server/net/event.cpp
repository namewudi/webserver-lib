#include "event.h"

namespace net{
    void Event::loop(){
        assert(std::this_thread::get_id() == getID());
        while(true){
            _poller->poll(_activeChannels);
            for(Channel* elem : _activeChannels){
                elem->doTask();
            }
        }
    }
    Channel* Event::getFreeChannel()const{
        int idx = std::find_if(_channels.begin(), _channels.end(), [](Channel* channel){
            return channel->isFree();
        }) - _channels.begin();
        if(idx == _channels.size()) return nullptr;
        return _channels[idx];
    }
    const std::thread::id Event::getID()const{
        return _tid;
    }
}