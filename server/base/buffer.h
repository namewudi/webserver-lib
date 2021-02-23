#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <unistd.h>

namespace base{
    class Buffer{
        using DataType = std::vector<char>;
        public:
            Buffer(int capacity = 3072): _data(capacity),
                                     _capacity(capacity),
                                     _size(0),
                                     _writeidx(0),
                                     _readidx(0){}
            Buffer(const Buffer& buffer): _data(buffer._data),
                                          _capacity(buffer._capacity),
                                          _size(buffer._size),
                                          _writeidx(0),
                                          _readidx(0){}
            Buffer(Buffer&& buffer): _data(std::forward<DataType>(buffer._data)),
                                     _capacity(buffer._capacity),
                                     _size(buffer._size),
                                     _writeidx(0),
                                     _readidx(0){}
            Buffer(const std::string& s, int capacity = 1024){
                assert(s.size() <= capacity);
                _data.assign(s.begin(), s.end());
                _size = s.size();
                _capacity = capacity;
                _writeidx = 0;
                _readidx = 0;
            }
            int find(const std::string&) const;
            virtual int readFromFd(int);
            virtual int writeToFd(int);
            const int size() const;
            void swap(Buffer&);
            const int capacity() const;
            virtual const int freeSize() const;
            virtual const int byteToWrite() const;
            virtual const int byteToRead() const;
            const char& operator[](int) const;
            char& operator[](int);
            virtual void append(const char*, int);
            virtual void append(const std::string&);
            virtual void append(std::string&&);
            virtual void append(const Buffer&);
            void push_back(char);
            void clear();
            std::string getNAsString(int);
            std::string getAllAsString();
            
            
            bool writable(int);
            
            int _size; 
            DataType::const_iterator begin() const;
            DataType::const_iterator end() const;  
            DataType::iterator begin();   
            DataType::iterator end();
            virtual int find_first_of(const std::string pattern, int start)const{
                return -1;
            }
            int _capacity;
            int _writeidx;
            int _readidx;
            DataType _data;
    };
}
