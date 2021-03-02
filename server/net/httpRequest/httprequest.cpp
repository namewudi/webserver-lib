#include "httprequest.h"

namespace net{
    const std::string HttpRequest::getHeader(const std::string& key) const{
        return httpRequestHeader->getHeader(key);
    }
    const HttpMethod HttpRequest::getMethod()const{
        return httpRequestLine->getHttpMethod();
    }
    const std::string HttpRequest::getUrl()const{
        return httpRequestLine->getUrl();
    }
    const HttpVersion HttpRequest::getVersion()const{
        return httpRequestLine->getHttpVersion();
    }

    const std::string HttpRequest::getParameters()const{
        std::string result = httpRequestLine->getParametersAsString();
        if(result.empty()){
            if(getMethod() == HttpMethod::POST){
                result = httpRequestBody->getParametersAsString();
            }
        }
        return result;   
    }

    const std::string HttpRequest::getParameter(const std::string& key){
        std::string result = httpRequestLine->getParameter(key);
        if(result.empty()){
            if(getMethod() == HttpMethod::POST){
                result = httpRequestBody->getParameter(key);
            }
        }
        return result; 
    }
    bool HttpRequest::hasParameter(const std::string& key){
        return httpRequestLine->hasParameter(key) || httpRequestBody->hasParameter(key);
    }
}