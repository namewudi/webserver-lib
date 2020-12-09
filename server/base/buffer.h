#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
namespace base{
    class Buffer{
        using DataType = std::vector<char>;
        public:
            Buffer(int capacity = 1024): _data(capacity),
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
            int readFromFd(int);
            int writeToFd(int);
            const int size() const;
            void swap(Buffer&);
            const int capacity() const;
            const int freeSize() const;
            const int byteToWrite() const;
            const int byteToRead() const;
            const char& operator[](int) const;
            char& operator[](int);
            void append(char*, int);
            void append(const std::string&);
            void append(std::string&&);
            void append(const Buffer&);
            void push_back(char);
            void clear();
            std::string getNAsString(int);
            std::string getAllAsString();
        private:
            bool writable(int);
            DataType::iterator begin();
            DataType::iterator end();
            DataType::const_iterator begin() const;
            DataType::const_iterator end() const;
            DataType _data;
            int _capacity;
            int _size;
            int _writeidx;
            int _readidx;
    };
}
