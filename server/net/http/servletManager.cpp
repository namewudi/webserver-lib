#include "servletManager.h"

namespace net{
    std::map<std::string, std::shared_ptr<Servlet>> ServletManager::servlets;
    std::string ServletManager::staticResourcePath;
    FilterManager ServletManager::_filterManager;
    std::shared_ptr<Filter> ServletManager::_beginFilter(new BeginFilter);
    std::shared_ptr<Filter> ServletManager::_endFilter(new EndFilter);
    void ServletManager::regist(std::shared_ptr<Servlet> servlet){
        for(const auto & path : servlet->getPaths()){
            servlets[path] = servlet;
            std::cerr<<"regist: "<< path<<std::endl;
        }
    }
    bool ServletManager::handleStaticResource(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
        std::string path(staticResourcePath + req->getUrl());
        return resp->addStaticResource(path);
    }
    void ServletManager::handle404(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
        resp->addVersion(req->getVersion());
        resp->addStatusCode("404");
        resp->addStatusMessage("Not Found");
        resp->addBody("<html><head><title>404 Not Found</title></head>"
                    "<body><h1>404 Not Found</h1></body></html>");
    }
    bool ServletManager::handleDynamicResource(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
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
    void ServletManager::handle(std::shared_ptr<HttpRequest>& req, std::shared_ptr<HttpResponse>& resp){
        std::cerr<<"here is servlet manager. url call: "<<req->getUrl()<<std::endl;
        if(req->getRequestHeader()->hasCookie("SESSION_ID")){
            req->getSession()->setSessionID(req->getRequestHeader()->getCookie("SESSION_ID"));
        }
        else{
            std::string sessionID = base::TimeStamp().setNow().toCookieString();
            std::cout<<"set SESSIONID: "<<sessionID<<std::endl;
            resp->addCookie(Cookie("SESSION_ID", sessionID).setMaxAge(1000).setPath("/"));
            req->getSession()->setSessionID(sessionID);
        }
        _filterManager.startChains(req, resp);
    }
}