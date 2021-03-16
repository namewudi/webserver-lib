#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "cookie.h"
namespace net{
    class HttpRequestHeader{
    public:
        using HeaderMap = std::unordered_map<std::string, std::string>;
        using CookieMap = std::unordered_map<std::string, std::string>;
        void setHeader(std::string key, std::string value){
            if(key == "Cookie"){
                std::string ck, cv;
                int target = -2, index;
                while(true){
                    index = value.find('=', std::max(target, 0));
                    if(index == std::string::npos) break;
                    ck = value.substr(target + 2, index - target - 2);
                    target = value.find(';', index);
                    if(target == std::string::npos) {
                        target = value.length();
                    }
                    cv = value.substr(index + 1, target - index - 1);
                    _cookies[ck] = cv;
                    //std::cout<<"cookie key: "<<ck<<"cookie value: "<<cv<<std::endl;
                }
            }
            else{
                this->headerMap[key] = value;
            }
        }
        const std::string getCookie(const std::string& key)const{
            std::string result;
            try{
                result = _cookies.at(key);
            }
            catch(std::out_of_range){
                std::cout<<"get Cookie失败"<<std::endl;
            }
            return result;
        }
        bool hasCookie(const std::string& key)const{
            return _cookies.count(key) > 0;
        }
        const std::vector<std::pair<std::string, std::string>> getAllCookie()const{
            std::vector<std::pair<std::string, std::string>> result;
            for(const auto& elem : _cookies){
                result.push_back(elem);
            }
            return result;
        }
        const std::string getHeader(const std::string& key)const{
            std::string result;
            try{
                result = headerMap.at(key);
            }
            catch(std::out_of_range){
                std::cout<<"get Header失败"<<std::endl;
            }
            return result;
        }
        bool hasHeader(const std::string& key)const{
            return headerMap.count(key) > 0;
        }
        const HeaderMap getHeaderMap()const{
            return this->headerMap;
        }
    private:
        HeaderMap headerMap;
        CookieMap _cookies;
    };
}