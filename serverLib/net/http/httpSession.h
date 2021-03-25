#pragma once
#include "../../base/timeStamp.h"
#include <map>
#include <unordered_map>
namespace net{

    class HttpSession{
    public:
        void setSessionID(const std::string& sessionID){this->_sessionID = sessionID;}
        const std::string getSessionID(){return this->_sessionID;}
        virtual const std::string getAttribute(const std::string&) = 0;
        virtual void setAttribute(const std::string&, const std::string&, int seconds = 1000) = 0;
    private:
        std::string _sessionID;
    };
}