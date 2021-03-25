#pragma once
#include "httpException.h"
#include <list>
#include "./multiPart.h"
namespace net{
    enum class ContentType{
        application_x_www_form_urlencoded,
        multipart_form_data
    };
    class HttpRequestBody{
    public:
        using ParameterMap = std::unordered_map<std::string, std::string>;
        const std::string getParameter(const std::string key){
            if(parameterMap.count(key) > 0){
                return parameterMap[key];
            }
            return "";
        }
        bool hasParameter(const std::string key){
            return parameterMap.count(key) > 0;
        }
        void setContentType(const std::string& type){
            if(type == "application/x-www-form-urlencoded"){
                this->contentType = ContentType::application_x_www_form_urlencoded;
            }
            else if(type.find("multipart/form-data") != type.length()){
                int index = type.find("boundary");
                if(index != type.size()){
                    _boundary = type.substr(index + 9);
                }
                this->contentType = ContentType::multipart_form_data;
            }
            
        }
        const std::string getBoundary()const{
            return _boundary;
        }
        const ContentType getContentType()const{
            return this->contentType;
        }
        void setParameter(std::string key, std::string value){
            //std::cerr<<"set parameter int body: "<<key <<" : "<< value<<std::endl;
            this->parameterMap[key] = value;
        }
        const std::string getParametersAsString()const{
            std::string result;
            for(auto iter = this->parameterMap.begin(); iter != this->parameterMap.end();){
                result += iter->first;
                result += ':';
                result += iter->second;
                iter++;
                if(iter != this->parameterMap.end()) result += ";<br/>";
            }
            return result;
        }
        void append(std::string&& data){
            _data.append(std::move(data));
        }
        void append(const std::string& data){
            _data.append(data);
        }
        const std::string getAsString()const{
            return _data;
        }
        void addMultiPart(const MultiPart& multiPart){
            _multiparts.push_back(multiPart);
        }
        void addMultiPart(MultiPart&& multiPart){
            _multiparts.push_back(std::move(multiPart));
        }
        const std::list<MultiPart>& getMultiParts()const{
            return _multiparts;
        }
        ParameterMap parameterMap;
        std::string _data;
    private:
        std::list<MultiPart> _multiparts;
        std::string _boundary;
        ContentType contentType;

    };
}