#pragma once
#include <vector>
#include <thread>
#include <assert.h>
#include <functional>
#include <mutex>
#include "event.h"
namespace net{
    class ManagerPool{
    public:
        explicit ManagerPool(int threadNum = 5, int eventNum = 10):_idx(0){
            assert(threadNum > 0);
            _size = threadNum;
            auto threadFunc = [this, eventNum](){
                auto id = std::this_thread::get_id();
                auto event = std::make_shared<Event>(eventNum);

                std::unique_lock<std::mutex> ul(_mutex);
                _events.push_back(event);
                ul.unlock();

                event->loop();
            };
            for(int i = 0; i < threadNum; i++){
                _pool.push_back(std::thread(threadFunc));
            }
        }
        std::shared_ptr<Event> get(){
            return _events[(_idx++) % _size];
        }
    private:
        std::vector<std::thread> _pool;
        std::vector<std::shared_ptr<Event>> _events;
        std::mutex _mutex;
        int _size;
        int _idx;
    };
}