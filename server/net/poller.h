#pragma once
#include <boost/noncopyable.hpp>
#include <vector>
#include <sys/epoll.h>
namespace net{
    class Channel;
    class Poller{
    public:
        using ChannelList = std::vector<Channel*>;
        virtual void poll(ChannelList&) = 0;
        virtual bool isInEvent(int) = 0;
        virtual bool isOutEvent(int) = 0;
        virtual bool isCloseEvent(int) = 0;
        virtual bool isErrorEvent(int) = 0;
        virtual bool removeChannel(int) = 0;
        virtual bool addChannel(Channel*, int, int) = 0;
        virtual bool modChannel(Channel*, int, int) = 0;
        virtual ~Poller(){};
    };
}
