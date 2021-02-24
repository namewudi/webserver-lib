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
        if(getMethod() == HttpMethod::GET){
            return httpRequestLine->getParametersAsString();
        }
        else{
            return httpRequestBody->getParametersAsString();
        }
        
    }

    const std::string HttpRequest::getParameter(const std::string& key){
        if(getMethod() == HttpMethod::GET){
            return httpRequestLine->getParameter(key);
        }
        else{
            return httpRequestBody->getParameter(key);
        }
    }
}