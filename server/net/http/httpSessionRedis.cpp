#include "httpSessionRedis.h"

namespace net{
    const std::string HttpSessionRedis::getAttribute(const std::string& key){
        return RedisUtil::getInMap(this->getSessionID(), key);
    }
    void HttpSessionRedis::setAttribute(const std::string&key, const std::string& value, int second){
        RedisUtil::addToMap(this->getSessionID(), key, value);
        updateExpiredTime(second);
    }
}