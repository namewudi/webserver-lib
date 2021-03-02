#pragma once
#include "httpresponse.h"
#include <iostream>
#include <vector>
#include "servlet.h"
#include <map>
namespace net{
    class ServletManager{
    public:
        ServletManager():staticResourcePath("/home/wudi/develop/myBlog/application/resources/static"){
            
        }
        void regist(std::shared_ptr<Servlet> servlet){
            for(const auto & path : servlet->getPaths()){
                servlets[path] = servlet;
                std::cerr<<"regist: "<< path<<std::endl;
            }
        }
        bool handleStaticResource(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
            std::string path(staticResourcePath + req->getUrl());

            return resp->addStaticResource(path);
        }
        void handle404(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
            resp->addVersion(req->getVersion());
            resp->addStatusCode("404");
            resp->addStatusMessage("Not Found");
            resp->addBody("<html><head><title>404 Not Found</title></head>"
                        "<body><h1>404 Not Found</h1></body></html>");
        }
        bool handleDynamicResource(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
            if(servlets.count(req->getUrl()) == 0) { 
                return false;
            }
            resp->addVersion(req->getVersion());
            
            resp->addStatusCode("200");
            resp->addStatusMessage("OK");
            if(req->getHeader("Connection") == "keep-alive"){
                resp->addHeader("Connection", "keep-alive");
            }
            switch (req->getMethod())
            {
            case HttpMethod::GET:
                servlets[req->getUrl()]->doGet(req, resp);
                break;
            case HttpMethod::POST:
                servlets[req->getUrl()]->doPost(req, resp);
                break;
            default:
                break;
            }
            return true;
        }
        void handle(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
            std::cerr<<"here is servlet manager. url call: "<<req->getUrl()<<std::endl;
            if(handleDynamicResource(req, resp)) return;
            if(handleStaticResource(req, resp)) return;
            
            handle404(req, resp);
        }
    private:
        std::map<std::string, std::shared_ptr<Servlet>> servlets;
        std::string staticResourcePath;
    };
}