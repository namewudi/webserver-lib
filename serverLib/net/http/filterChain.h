#pragma once
#include <iostream>
#include <memory>
#include "filter.h"
namespace net{
    class HttpRequest;
    class HttpResponse;
    class FilterChain{
    public:
        void doFilter(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp){
            auto trigeredUrl = req->getUrl();
            auto path = _curFilter->getPath();
            //std::cout<<"here is chain: "<<path<<std::endl;
            if(path == trigeredUrl.substr(0, std::min(path.length(), trigeredUrl.length()))){
                _curFilter->doFilter(req, resp, _nextChain);
            }
            else _nextChain->doFilter(req, resp); 
        }
        void setFilter(std::shared_ptr<Filter> filter){this->_curFilter = filter;}
        void setNextChain(std::shared_ptr<FilterChain> chain){this->_nextChain = chain;}
    private:
        std::shared_ptr<Filter> _curFilter;
        std::shared_ptr<FilterChain> _nextChain;
    };
}