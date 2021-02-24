#pragma once
#include "./circleBuffer.h"
#include <unistd.h>
namespace base{
    class CircleReadBuffer: public CircleBuffer{
    public:
        const int byteToRead() const;
        int readFromFd(int fd);
    };
}