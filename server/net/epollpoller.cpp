#include "epollpoller.h"
#include <thread>
namespace net{
    void EpollPoller::poll(ChannelList& activeChannels) {
        /*监听红黑树g_efd, 将满足的事件的文件描述符加至events数组中, 10秒没有事件满足, 返回 0*/
        int nfd = epoll_wait(_epollFd, &_activeEvents.front(), _maxEvents, _timeout);
        
        activeChannels.resize(nfd);
        if(nfd > 0){
            for(int i = 0; i < nfd; i++){
                activeChannels[i] = static_cast<Channel*>(_activeEvents[i].data.ptr);
            }
        }

    }
    bool EpollPoller::isInEvent(int event) {
        return event & EPOLLIN; //修改
    }
    bool EpollPoller::isOutEvent(int event) {
        return event & EPOLLOUT; //修改
    }
    bool EpollPoller::isCloseEvent(int event) {
        return (event & EPOLLRDHUP); //修改
    }
    bool EpollPoller::isErrorEvent(int event) {
        return  event & EPOLLERR; //修改
    }
    bool EpollPoller::removeChannel(int fd){
        epoll_event temp = {0, {0}};
        temp.data.ptr = nullptr;
        if(epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, &temp) < 0) {
            //printf("event delete fail\n");
            return false;
        }
        else {
            //printf("event delete OK\n");
            return true;
        }
        
    }
    bool EpollPoller::addChannel(Channel* channel, int fd, int eventType){
        epoll_event temp = {0, {0}};
        temp.data.ptr = static_cast<void*>(channel);
        temp.events = eventType;
        if(epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &temp)<0){
            //printf("event add failed [fd=%d], events[%d]\n", fd, eventType);
            return false;
        }
        else{
            //if(eventType & EPOLLIN) printf("event add OK [fd=%d], op=%d, events=epollin\n", fd, EPOLL_CTL_ADD);
            //else printf("event add OK [fd=%d], op=%d, events=epollout\n", fd, EPOLL_CTL_ADD);
            return true;
        }
    }
    bool EpollPoller::modChannel(Channel* channel, int fd, int eventType) {
        epoll_event temp = {0, {0}};
        temp.data.ptr = static_cast<void*>(channel);
        temp.events = eventType;
        if(epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &temp)<0){
            //printf("event mod failed [fd=%d], events[%d]\n", fd, eventType);
            return false;
        }
        else{
            //if(eventType & EPOLLIN) printf("event add OK [fd=%d], op=%d, events=epollin\n", fd, EPOLL_CTL_ADD);
            //else printf("event mod OK [fd=%d], op=%d, events=epollout\n", fd, EPOLL_CTL_ADD);
            return true;
        }
    }
}