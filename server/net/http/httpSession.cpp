#include "httpSession.h"

namespace net{
    std::map<std::string, std::pair<std::unordered_map<std::string, std::string>, base::TimeStamp>> HttpSession::_data;
    const std::string HttpSession::getAttribute(const std::string& key){
        std::cout<<"get attribute :"<<key<<"at session: "<<_sessionID<<std::endl;
        if(_data.count(_sessionID) == 0) return "";
        auto now = base::TimeStamp().setNow();
        if(_data[_sessionID].second < now) {
            std::cout<<"session 超时"<<_data[_sessionID].second.toString()<<" now is: "<<now.toString()<<std::endl;;
            _data.erase(_sessionID);
            return "";
        }
        std::cout<<"value: "<<_data[_sessionID].first[key]<<std::endl;
        return _data[_sessionID].first[key];
    }
    void HttpSession::setAttribute(const std::string&key, const std::string& value){
        std::cout<<"set attribute :"<<key<<"value: "<<value<<"at session: "<<_sessionID<<std::endl;
        _data[_sessionID].first[key] = value;
        _data[_sessionID].second.setNow();
        _data[_sessionID].second += 1000;
    }
}