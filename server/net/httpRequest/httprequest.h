#pragma once
#include <string>
#include <unordered_map>
#include "httpRequestLine.h"
#include "httpRequestHeader.h"
#include "httpRequestBody.h"
#include <memory>
#include "../../exception/httpException.h"
#include "../utils/processStatus.h"
namespace net{
    
    class HttpRequest{ 
    public:
        //HttpRequest(base::Buffer* buffer): data(std::move(buffer->getAllAsString())){}
        HttpRequest(){
            this->status = Status::init;
            this->httpRequestBody = std::make_shared<HttpRequestBody>();
            this->httpRequestHeader = std::make_shared<HttpRequestHeader>();
            this->httpRequestLine = std::make_shared<HttpRequestLine>();
        }
        const Status getStatus()const{
            return this->status;
        }
        void setStatus(const Status& status){
            this->status = status;
        }
        const HttpMethod getMethod()const;
        const std::string getUrl()const;
        const void setUrl(const std::string url){
            this->httpRequestLine->setUrl(url);
        }
        const HttpVersion getVersion()const;

        void setVersion(HttpVersion version){
            this->httpRequestLine->setHttpVersion(version);
        }
        bool hasHeader(const std::string& key)const{
            return httpRequestHeader->hasHeader(key);
        }
        const std::string getHeader(const std::string&) const;
        void setHeader(const std::string& key, const std::string& value){
            this->httpRequestHeader->setHeader(key, value);
        }
        const std::string getParameters()const;
        void setParameter(const std::string& key, const std::string& value){
            this->httpRequestLine->setParameter(key, value);
        }
        void setParameters(const HttpRequestLine::ParameterMap& parameters){
            for(const auto& elem : parameters){
                this->httpRequestLine->setParameter(elem.first, elem.second);
            }
        }
        const std::string getParameter(const std::string&);
        std::shared_ptr<HttpRequestLine> getRequestLine(){
            return this->httpRequestLine;
        }
        std::shared_ptr<HttpRequestHeader> getRequestHeader(){
            return this->httpRequestHeader;
        }
        std::shared_ptr<HttpRequestBody> getRequestBody(){
            return this->httpRequestBody;
        }
    private:
        std::shared_ptr<HttpRequestBody> httpRequestBody;
        std::shared_ptr<HttpRequestLine> httpRequestLine;
        std::shared_ptr<HttpRequestHeader> httpRequestHeader;
        Status status;
    };
}