#pragma once
#include <iostream>
#include <unordered_map>
namespace net{
    enum class HttpMethod{
            GET, POST, PUT, DELETE
    };
    enum class HttpVersion{
        HTTP1_0 , HTTP1_1
    };
    class HttpRequestLine{
    public:
        using ParameterMap = std::unordered_map<std::string, std::string>;
        const std::string getUrl() const{
            return this->url;
        }
        void setUrl(const std::string& url){
            this->url = url;
        }
        const ParameterMap getParameterMap() const{
            return this->parameterMap;
        }
        const std::string getParametersAsString()const{
            std::string result("{<br/>");
            for(auto iter = this->parameterMap.begin(); iter != this->parameterMap.end();){
                result += iter->first;
                result += ':';
                result += iter->second;
                iter++;
                if(iter != this->parameterMap.end()) result += ";<br/>";
            }
            result += "<br/>}<br/>";
            return result;
        }
        const std::string getParameter(const std::string key){
            if(parameterMap.count(key) > 0){
                return parameterMap[key];
            }
            return "";
        }
        bool hasParameter(const std::string key){
            return parameterMap.count(key) > 0;
        }
        void setMethod(HttpMethod method){
            this->httpmethod = method;
        }
        void setParameter(std::string key, std::string value){
            this->parameterMap[key] = value;
        }
        const HttpVersion getHttpVersion() const{
            return this->httpversion;
        }
        void setHttpVersion(HttpVersion httpversion){
            this->httpversion = httpversion;
        }
        const HttpMethod getHttpMethod() const{
            return this->httpmethod;
        }
        void setHttpMethod(HttpMethod httpmethod){
            this->httpmethod = httpmethod;
        }
    private:
        std::string url;
        ParameterMap parameterMap;
        HttpVersion httpversion;
        HttpMethod httpmethod;
    };
}