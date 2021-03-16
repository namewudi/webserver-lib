#include "httpSessionLocal.h"

namespace net{
    std::map<std::string, std::pair<std::unordered_map<std::string, std::string>, base::TimeStamp>> HttpSessionLocal::_data;
    const std::string HttpSessionLocal::getAttribute(const std::string& key){
        //std::cout<<"get attribute :"<<key<<"at session: "<<_sessionID<<std::endl;
        if(_data.count(this->getSessionID()) == 0) return "";
        auto now = base::TimeStamp().setNow();
        if(_data[this->getSessionID()].second < now) {
            //std::cout<<"session 超时"<<_data[_sessionID].second.toString()<<" now is: "<<now.toString()<<std::endl;;
            _data.erase(this->getSessionID());
            return "";
        }
        //std::cout<<"value: "<<_data[_sessionID].first[key]<<std::endl;
        return _data[this->getSessionID()].first[key];
    }
    void HttpSessionLocal::setAttribute(const std::string&key, const std::string& value, int seconds){
        //std::cout<<"set attribute :"<<key<<"value: "<<value<<"at session: "<<_sessionID<<std::endl;
        _data[this->getSessionID()].first[key] = value;
        updateExpiredTime(base::TimeStamp().setNow() + seconds);
        // _data[this->getSessionID()].second.setNow();
        // _data[this->getSessionID()].second += seconds;
    }
}