#pragma once
#include <boost/noncopyable.hpp>
#include "threadpool.h"

namespace base{
    class ThreadPoolSingleton: private boost::noncopyable{
    public:
        static ThreadPool& get(int threadNum = 3){
            static ThreadPool basePool(threadNum);
            return basePool;
        }
    private:
        ThreadPoolSingleton(){}
    };
}