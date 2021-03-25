#pragma once
#include <memory>
#include <iostream>
namespace net{
    class HttpRequest;
    class HttpResponse;
    class FilterChain;
    class Filter{
    public:
        virtual void doFilter(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>, std::shared_ptr<FilterChain>){}
        void setPath(const std::string& path){this->_path = path;}
        const std::string& getPath(){return _path;}
    private:
        std::string _path;
    };
}