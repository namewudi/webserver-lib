#include "circleBuffer.h"

namespace base{
    const int CircleBuffer::freeSize() const {
        int curSize = this->size();
        if(_index > curSize){
            return _index - curSize - 1;
        }
        else {
            return capacity() - curSize + _index - 1;
        }
    }
    const int CircleBuffer::byteToUse()const{
        int curSize = this->size();
        if(curSize < _index){
            return capacity() - _index + curSize;
        }
        else {
            return curSize - _index;
        }
    }
    void CircleBuffer::clear(){
        this->_size = 0;
        this->_index = 0;
    }
    int CircleBuffer::modifyIndex(int index){
        if(index >= this->capacity()){
            return modifyIndex(index - capacity());
        }
        return index;
    }
    int CircleBuffer::find(int start, int end, const std::string pattern)const{
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

    std::string CircleBuffer::getString(int begin, int end){
        if(begin < end){
            return std::string(_data.begin() + begin, _data.begin() + end);
        }
        else if(begin > end){
            return std::string(_data.begin() + begin, _data.end()) + std::string(_data.begin(), _data.begin() + end);
        }
        return "";
    }
}