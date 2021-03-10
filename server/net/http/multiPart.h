#pragma once
#include <unordered_map>
#include <iostream>
namespace net{
    class MultiPart
    {
    private:
        std::unordered_map<std::string, std::string> _header;
        std::string _data;
    public:
        void setHeader(std::string key, std::string value){
            _header[key] = value;
        }
        const std::string& getHeader(const std::string& key)const{
            return _header.at(key);
        }
        bool hasHeader(const std::string& key)const{
            return _header.count(key) > 0;
        }
        void setData(std::string&& data){
            _data = std::move(data);
        }
        const std::string& getData()const{
            return _data;
        }
    };   
}