#pragma once
#include <iostream>
#include <vector>
namespace base{
    class CircleBuffer{
    public:
        using DataType = std::vector<char>;

        CircleBuffer(int capacity = 30720): _data(capacity),
                                     _capacity(capacity),
                                     _size(0),
                                     _index(0){}
        
        int getIndex()const{return _index;}
        void setIndex(int x){
            //std::cerr<<"index set to "<<x<<std::endl;
            this->_index = x;
        }
        const int size()const{return this->_size;}
        const int capacity()const{return this->_capacity;}
        const int freeSize()const;
        const int byteToUse()const;
        void clear();
        DataType::iterator begin(){return _data.begin();}
        DataType::iterator end(){return _data.end();}
        int modifyIndex(int index);
        int find_first_of(int start, int end, const std::string pattern)const;
        std::string getString(int begin, int end);

        DataType _data;
        int _index;
        const int _capacity;
        int _size;
    };
}