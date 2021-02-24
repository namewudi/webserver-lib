#pragma once
#include "./circleBuffer.h"
#include <assert.h>
#include <unistd.h>
#include "buffer.h"
namespace base{
    class CircleWriteBuffer: public CircleBuffer{
    public:
        const int byteToWrite() const;

        int writeToFd(int fd);
        bool writable(int len){return freeSize() >= len;}
        void append(const char* buffer, int len);
        void append(const std::string& s);
        void append(const Buffer& buffer){append(&(*buffer.begin()), buffer.size());}
    };
}