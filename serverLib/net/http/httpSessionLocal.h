#pragma once
#include "../../base/timeStamp.h"
#include "httpSession.h"
#include <map>
#include <unordered_map>
namespace net{

    class HttpSessionLocal: public HttpSession{
    public:
        const std::string getAttribute(const std::string&)override;
        void setAttribute(const std::string&, const std::string&, int second = 1000)override;
    private:
        void updateExpiredTime(const base::TimeStamp& timeStamp){_data[this->getSessionID()].second = timeStamp;}
        static std::map<std::string, std::pair<std::unordered_map<std::string, std::string>, base::TimeStamp>> _data;
    };
}