#pragma once
#include "../../base/timeStamp.h"
#include "httpSession.h"
#include "../utils/redisUtils.h"
#include <map>
#include <unordered_map>
namespace net{

    class HttpSessionRedis: public HttpSession{
    public:
        const std::string getAttribute(const std::string&)override;
        void setAttribute(const std::string&, const std::string&, int second = 1000)override;
    private:
        void updateExpiredTime(int seconds){RedisUtil::setExpire(this->getSessionID(), seconds);}
    };
}