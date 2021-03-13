#include "buffer.h"

namespace base{
    const int Buffer::capacity() const{
        return _capacity;
    }
    const int Buffer::size() const{
        return _size;
    }
    const int Buffer::freeSize() const{
        return capacity() - size();
    }
    const int Buffer::byteToWrite() const{
        return size() - _writeidx;
    }
    
    const int Buffer::byteToRead() const{
        return size() - _readidx;
    }
    int Buffer::readFromFd(int fd){
        if(freeSize() <= 0) return 0;
        //std::cout<<"tic"<<std::endl;
        int n = read(fd, &_data[size()], freeSize());
        //std::cout<<"toc"<<std::endl;
        if(n <= 0) return 0;

        // std::cout<<"start read. fd = "<<fd<<std::endl;
        // std::cout<<"current size: "<<size();
        // std::cout<<" Byte : "<<n<<std::endl;
        // std::cout<<"data: "<<std::endl;
        int curSize = size();
        _size += n;
        for(int i = 0; i < n; i++){
            std::cout<<_data[curSize + i];
        }
        std::cout<<std::endl<<"read complete!"<<std::endl;
        return n;
    }
    int Buffer::writeToFd(int fd){
        if(byteToWrite() <= 0) {
            std::cout<<"write failed. fd = "<<fd<<" Byte : " <<byteToWrite()<<std::endl;
            return 0;
        }
        
        int n = write(fd, &_data[0] + _writeidx, byteToWrite());
        // std::cout<<"start write. fd = "<<fd<<" Byte : " <<byteToWrite()<<"write index :"<<_writeidx<<std::endl;
        // std::cout<<"data: "<<std::endl;
        for(int i = 0; i < n; i++){
            std::cout<<_data[_writeidx + i];
        }
        std::cout<<std::endl;
        _writeidx += n;
        return n;
    }

    void Buffer::swap(Buffer& buffer){
        std::swap(buffer._data, _data);
        std::swap(buffer._size, _size);
        std::swap(buffer._capacity, _capacity);
    }
    bool Buffer::writable(int len){
        return freeSize() >= len;
    }
    void Buffer::append(const char* buffer, int len){
        assert(writable(len));
        std::copy(buffer, buffer + len, end());
        _size += len;
    }
    void Buffer::append(const std::string& s){
        if(!writable(s.size())){
            std::cerr<<"free size: "<<freeSize()<<std::endl;
            assert(writable(s.size()));
        }
        
        std::copy(s.begin(), s.end(), end());
        _size += s.size();
    }
    void Buffer::append(const Buffer& buffer){
        assert(writable(buffer.size()));
        std::copy(buffer.begin(), buffer.end(), end());
        _size += buffer.size();
    }
    void Buffer::append(std::string&& s){
        if(!writable(s.size())){
            std::cerr<<"free size: "<<freeSize()<<std::endl;
            assert(writable(s.size()));
        }
        std::copy(s.begin(), s.end(), end());
        _size += s.size();
    }
    char& Buffer::operator[](int idx){
        return const_cast<char&>(static_cast<const Buffer&>(*this)[idx]);
    }
    const char& Buffer::operator[](int idx) const{
        return _data[idx];
    }
    void Buffer::clear(){
        _size = 0;
        _readidx = 0;
        _writeidx = 0;
    }
    Buffer::DataType::iterator Buffer::begin(){
        return _data.begin();
    }
    Buffer::DataType::iterator Buffer::end(){
        return _data.begin() + size();
    }
    Buffer::DataType::const_iterator Buffer::begin() const{
        return _data.begin();
    }
    Buffer::DataType::const_iterator Buffer::end() const{
        return _data.begin() + size();
    }
    void Buffer::push_back(char c){
        *end() = c;
        _size++;
    }
    std::string Buffer::getNAsString(int n){
        assert(n <= (byteToRead()));
        std::string result(_data.begin() + _readidx, _data.begin() + _readidx + n);
        _readidx += n;
        return result;
    }
    std::string Buffer::getAllAsString(){
        std::string result(_data.begin() + _readidx, end());
        _readidx = size();
        return result;
    }
    int Buffer::find(const std::string& s) const{
        auto iter = std::find_first_of(begin() + _readidx, end(), s.begin(), s.end());
        if(iter == end()) return -1;
        return iter - begin();
    }
    
}