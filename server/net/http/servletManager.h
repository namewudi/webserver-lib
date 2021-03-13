#pragma once
#include "httpresponse.h"
#include <iostream>
#include <vector>
#include "servlet.h"
#include "filterManager.h"
#include "../../base/timeStamp.h"
#include <map>
namespace net{
    class ServletManager{
    public:
        static void init(){_filterManager.registEnd(_endFilter);_filterManager.registBegin(_beginFilter);}
        static void setStaticResourcePath(const std::string& path){staticResourcePath = path;}
        static void regist(std::shared_ptr<Servlet> servlet);
        static bool handleStaticResource(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp);
        static void handle404(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp);
        static bool handleDynamicResource(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp);
        static void handleRequest(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
            //std::cout<<"start handle request!"<<std::endl;
            if(handleDynamicResource(req, resp)) return;
            if(handleStaticResource(req, resp)) return;
            handle404(req, resp);
        }
        static void handle(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp);
        static void registFilter(std::shared_ptr<Filter> filter){_filterManager.regist(filter);}
    private:
        static std::map<std::string, std::shared_ptr<Servlet>> servlets;
        static std::string staticResourcePath;
        static FilterManager _filterManager;
        static std::shared_ptr<Filter> _beginFilter;
        static std::shared_ptr<Filter> _endFilter;
    };
    class BeginFilter: public Filter{
    public:
        BeginFilter(){
            this->setPath("/");
        }
        void doFilter(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp, std::shared_ptr<FilterChain> chain)override{
            chain->doFilter(req, resp);
            //std::cout<<"Content-Length: "<<std::to_string(resp->getContentLength())<<std::endl;
            resp->addHeader("Content-Length", std::to_string(resp->getContentLength()));
        }
    };
    class EndFilter: public Filter{
    public:
        EndFilter(){
            this->setPath("/");
        }
        void doFilter(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp, std::shared_ptr<FilterChain> chain)override{
            ServletManager::handleRequest(req, resp);
        }
    };
}