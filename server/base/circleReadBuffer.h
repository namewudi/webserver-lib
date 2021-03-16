#pragma once
#include "./circleBuffer.h"
#include <unistd.h>
namespace base{
    class CircleReadBuffer: public CircleBuffer{
    public:
        CircleReadBuffer(){}
        CircleReadBuffer(int bufferSize):CircleBuffer(bufferSize){}
        const int byteToRead() const;
        int readFromFd(int fd);
    };
}