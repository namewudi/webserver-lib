#pragma once
#include "poller.h"
#include "channel.h"
#include <stdio.h>
#include <iostream>
#include <memory>
namespace net{
    class Channel;
    class EpollPoller: public Poller{
    public:
        using ChannelList = std::vector<Channel*>;
        EpollPoller(int num): _epollFd(epoll_create(num)),
                                      _maxEvents(num),
                                      _activeEvents(num),
                                      _timeout(10000){}
        void poll(ChannelList&) override;
        bool isInEvent(int) override;
        bool isOutEvent(int) override;
        bool isCloseEvent(int) override;
        bool isErrorEvent(int) override;
        bool removeChannel(int) override;
        bool addChannel(Channel*, int, int) override;
        bool modChannel(Channel*, int, int) override;
    private:
        using EventList = std::vector<epoll_event>;
        int _epollFd;
        int _maxEvents;
        EventList _activeEvents;
        int _timeout;//修改
    };
}
