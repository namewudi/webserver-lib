#pragma once
#include "threadpool.h"

namespace base{
    class ThreadPoolSingleton{
    public:
        ThreadPoolSingleton(const ThreadPoolSingleton&) = delete;
        ThreadPoolSingleton& operator=(const ThreadPoolSingleton&) = delete;
        static ThreadPool& get(int threadNum = 3){
            static ThreadPool basePool(threadNum);
            return basePool;
        }
    private:
        ThreadPoolSingleton(){}
    };
}