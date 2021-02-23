#pragma once
#include "./circleBuffer.h"
#include <unistd.h>
namespace base{
    class CircleWriteBuffer: public CircleBuffer{
    public:
        virtual const int freeSize() const override{
            if(size() < _index){
                return _index - size() - 1;
            }
            else {
                return capacity() - size() + _index - 1;
            }
        }
        const int byteToWrite() const{
            if(size() < _index){
                return capacity() - _index + size();
            }
            else {
                return size() - _index;
            }
        }

        int writeToFd(int fd){
            if(byteToWrite() <= 0) {
                std::cout<<"write failed. fd = "<<fd<<" Byte : " <<byteToWrite()<<std::endl;
                return 0;
            }
            int n = 0;
            std::cout<<"start write. fd = "<<fd<<" Byte = " <<byteToWrite()<<" start index = "<<_index<<std::endl;
            if(byteToWrite() > capacity() - _index){
                n = write(fd, &_data[_index], capacity() - _index);   
            }
            else{
                n = write(fd, &_data[_index], byteToWrite());
            }
            /*
            std::cout<<"data: ***********************************************"<<std::endl;
            for(int i = 0; i < n; i++){
                std::cerr<<_data[_index + i];
            }
            std::cout<<std::endl<<"*****************************************************"<<std::endl;
            */
            _index += n;
            if(_index >= capacity()) {
                _index -= capacity();
            }
            std::cerr<<"write complete, remain byte to write : "<<byteToWrite()<<std::endl;
            return n;
        }
        bool writable(int len){
        return freeSize() >= len;
        }
        void append(const char* buffer, int len){
            assert(writable(len));
            if((size() + len) > capacity()){
                std::copy(buffer, buffer + capacity() - size(), _data.begin() + _size);
                std::copy(buffer + capacity() - size(), buffer + len, _data.begin());
            }
            else{
                std::copy(buffer, buffer + len, _data.begin() + _size);
            }
            _size += len;
            if(_size >= capacity()){
                _size -= capacity();
            }
        }
        void append(const std::string& s){
            if(!writable(s.size())){
                std::cerr<<"free size: "<<freeSize()<<std::endl;
                assert(writable(s.size()));
            }
            append(&s.front(), s.size());
        }
        void append(const Buffer& buffer){
            append(&(*buffer.begin()), buffer.size());

        }
        void append(std::string&& s){
            append(s);
        }
    };
}