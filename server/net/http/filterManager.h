#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "filterChain.h"
namespace net{
    class HttpRequest;
    class HttpResponse;
    class FilterManager{
    public:
        void regist(std::shared_ptr<Filter> filter){
            std::shared_ptr<FilterChain> chain = std::make_shared<FilterChain>();
            chain->setFilter(filter);
            chain->setNextChain(_end);
            if(!_filterChains.empty()){
                _begin->setNextChain(_filterChains.front());
                _filterChains.back()->setNextChain(chain);
            }
            else{
                _begin->setNextChain(chain);
            }
            _filterChains.push_back(chain);
        }
        void registBegin(std::shared_ptr<Filter> filter){
            _begin = std::make_shared<FilterChain>();
            _begin->setFilter(filter);
            _begin->setNextChain(_end);
        }
        void registEnd(std::shared_ptr<Filter> filter){
            _end = std::make_shared<FilterChain>();
            _end->setFilter(filter);
        }
        void startChains(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp){_begin->doFilter(req, resp);}
    private:
        std::vector<std::shared_ptr<FilterChain>> _filterChains;
        std::shared_ptr<FilterChain> _begin;
        std::shared_ptr<FilterChain> _end;
    };
}