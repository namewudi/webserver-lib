#pragma once
#include "./circleBuffer.h"
#include <assert.h>
#include <unistd.h>
#include "buffer.h"
#include <mutex>
#include <atomic>
#include <condition_variable>
namespace base{
    class CircleWriteBuffer: public CircleBuffer{
    public:
        CircleWriteBuffer(){
            _taskLength = 0;
        }
        const int byteToWrite() const;
        int writeToFd(int fd);
        bool writable(int len){return freeSize() >= len;}
        void appendInfinite(const char* buffer, int len);
        void append(const char* buffer, int len);
        void append(const std::string& s);
        void append(const Buffer& buffer){append(&(*buffer.begin()), buffer.size());}
        void addTaskLength(int len){
            _taskLength += len;
        }
        int remainTaskLength(){
            return (int)_taskLength;
        }
    private: 
        std::mutex _mutex;
        std::atomic<int> _taskLength;
        std::condition_variable _condVari;
    };
}