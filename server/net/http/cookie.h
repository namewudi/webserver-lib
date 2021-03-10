#pragma once
#include <iostream>
#include "../../base/timeStamp.h"
namespace net{
    class Cookie{
    public:
        Cookie(const std::string& key, const std::string& value){
            this->_key = key;
            this->_value = value;
            this->_hasExipredTime = false;
        }
        const std::string getKey()const {
            return _key;
        }
        const std::string getValue()const{
            return _value;
        }
        Cookie& setMaxAge(int time){
            _expiredTime.setNow();
            _expiredTime += time;
            _hasExipredTime = true;
            return *this;
        }
        Cookie& setPath(const std::string& path){
            _path = path;
            return *this;
        }
        const std::string toString(){
            std::string result = _key + "=" + _value;
            if(!_path.empty()){
                result.append("; Path=" + _path);
            }
            if(_hasExipredTime){
                result.append("; Expires=" + _expiredTime.toCookieString());
            }
            return result;
        }
    private:
        bool _hasExipredTime;
        std::string _path;
        std::string _key, _value;
        base::TimeStamp _expiredTime;
    };
}