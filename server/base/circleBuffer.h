#pragma once
#include <iostream>
#include <vector>
namespace base{
    class CircleBuffer{
    public:
        CircleBuffer(int capacity = 3072): _data(capacity),
                                     _capacity(capacity),
                                     _size(0),
                                     _index(0){}
        using DataType = std::vector<char>;
        DataType _data;
        int _index;
        int getIndex()const{
            return _index;
        }
        void setIndex(int x){
            //std::cerr<<"index set to "<<x<<std::endl;
            this->_index = x;
        }
        const int size()const{
            return this->_size;
        }
        const int capacity()const{
            return this->_capacity;
        }
        virtual const int freeSize()const = 0;
        void clear(){
            this->_size = 0;
            this->_index = 0;
        }
        DataType::iterator begin(){
            return _data.begin();
        }
        DataType::iterator end(){
            return _data.end();
        }
        int modifyIndex(int index){
            if(index >= this->capacity()){
                return modifyIndex(index - capacity());
            }
            return index;
        }
        const int _capacity;
        int _size;
    };
}