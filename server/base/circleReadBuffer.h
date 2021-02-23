#pragma once
#include "./circleBuffer.h"
#include <unistd.h>
namespace base{
    class CircleReadBuffer: public CircleBuffer{
    public:
        virtual const int freeSize() const override{
            int curSize = size();
            if(_index > curSize){
                return _index - curSize - 1;
            }
            else {
                return capacity() - curSize + _index - 1;
            }
        }
        const int byteToRead() const{
            int curSize = size();
            if(curSize < _index){
                return capacity() - _index + size();
            }
            else {
                return size() - _index;
            }
        }
        int readFromFd(int fd){
            if(freeSize() <= 0) return 0;
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
            _size += n;
            if(_size >= capacity()){
                _size -= capacity();
            }

            std::cout<<"read complete!"<<"  free size "<<freeSize()<<std::endl;
            return n;
        }
        int find_first_of(int start, int end, const std::string pattern)const{
            int m = pattern.size();
            if(start > end){
                int j = 0;
                for(int i = start; i < 2 * capacity() - start + end - m; i++){
                    for(j = 0; j < m; j++){
                        if(pattern[j] != _data[(i + j) >= capacity() ? (i + j - capacity()) : (i + j)]) {
                            break;
                        }
                    }
                    if(j == m) {
                        //std::cerr<<"find pattern "<<pattern<<" success! at index: "<< i<<std::endl;
                        return i;
                    }
                }
            }
            else if(start < end){
                int j = 0;
                for(int i = start; i < end + 1 - m; i++){
                    for(j = 0; j < m; j++){
                        if(pattern[j] != _data[i + j]) {
                            break;
                        }
                    }
                    if(j == m) {
                        //std::cerr<<"find pattern "<<pattern<<" success! at index: "<< i<<std::endl;
                        return i;
                    }
                }
            }
            return -1;
        }
        std::string getString(int begin, int end){
            if(begin < end){
                return std::string(_data.begin() + begin, _data.begin() + end);
            }
            else if(begin > end){
                return std::string(_data.begin() + begin, _data.end()) + std::string(_data.begin(), _data.begin() + begin);
            }
        }
    };
}