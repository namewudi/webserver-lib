#pragma once
#include "httpresponse.h"
#include <iostream>
#include <vector>
#include "servlet.h"
#include <map>
namespace net{
    class ServletManager{
    public:
        void regist(std::shared_ptr<Servlet> servlet){
            for(const auto & path : servlet->getPaths()){
                servlets[path] = servlet;
                std::cerr<<"regist: "<< path<<std::endl;
            }
        }
        void handle(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
            std::cerr<<"here is servlet manager. url call: "<<req->getUrl()<<std::endl;
            resp->addVersion(req->getVersion());
            if(servlets.count(req->getUrl()) == 0) { 
                resp->addStatusCode("404");
                resp->addStatusMessage("Not Found");
                resp->addBody("<html><head><title>404 Not Found</title></head>"
                            "<body><h1>404 Not Found</h1></body></html>");
                return;
            }
            resp->addStatusCode("200");
            resp->addStatusMessage("OK");
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
        }
    private:
        std::map<std::string, std::shared_ptr<Servlet>> servlets;
    };
}