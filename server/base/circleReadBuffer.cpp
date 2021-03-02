#include "circleReadBuffer.h"

namespace base{
    const int CircleReadBuffer::byteToRead() const{
        return this->byteToUse();
    }

    int CircleReadBuffer::readFromFd(int fd){
        if(freeSize() <= 0) {
            std::cerr<<"读缓冲区空闲空间为0！"<<std::endl;
            return -1;
        }
        std::cout<<"start read. fd = "<<fd<<std::endl;
        std::cout<<"current size: "<<size()<<std::endl;
        int n = read(fd, &_data[size()], std::min(freeSize(), capacity() - size()));// fix me

        if(n <= 0) return 0;
        /*
        std::cout<<"data: *************************************"<<std::endl;
        for(int i = 0; i < n; i++){
            std::cout<<_data[_size + i];
        }
        std::cout<<std::endl<<" Byte : "<<n<<std::endl;
        std::cout<<"*******************************************"<<std::endl;
        std::cout<<"all data: *************************************"<<std::endl;
        for(int i = 0; i < _size + n; i++){
            std::cout<<_data[i];
        }
        std::cout<<std::endl<<"*******************************************"<<std::endl;
        */
        _size = modifyIndex(_size + n);

        std::cout<<"read complete!"<<"read byte "<< n <<"  free size "<<freeSize()<<std::endl;
        return n;
    }
}