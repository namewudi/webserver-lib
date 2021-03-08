#pragma once
#include "../../base/timeStamp.h"
#include <map>
#include <unordered_map>
namespace net{

    class HttpSession{
    public:
        void setSessionID(const std::string& sessionID){this->_sessionID = sessionID;}
        const std::string getAttribute(const std::string&);
        void setAttribute(const std::string&, const std::string&);
        void updateExpiredTime(const base::TimeStamp& timeStamp){_data[_sessionID].second = timeStamp;}
    private:
        static std::map<std::string, std::pair<std::unordered_map<std::string, std::string>, base::TimeStamp>> _data;
        std::string _sessionID;
    };
}