#include "redisUtils.h"

namespace net{
    RedisContextRAII RedisUtil::_context;
    redisReply* RedisUtil::_reply = nullptr;
    bool RedisUtil::connect(const std::string& host, int port){
        _context.setContext(redisConnect(host.c_str(), port));
        if(_context.getContext() != NULL && _context.getContext()->err)
        {
            printf("connect error: %s\n", _context.getContext()->errstr);
            return 0;
        }
        return 1;
    }
    void RedisUtil::setExpire(const std::string& key, int seconds){
        _reply = (redisReply*)redisCommand(_context.getContext(), "EXPIRE %s %s", key.c_str() , std::to_string(seconds).c_str());
        freeReplyObject(_reply);
    }
    bool RedisUtil::hasStringKey(const std::string& key){
        _reply = (redisReply*)redisCommand(_context.getContext(), "GET %s", key.c_str());
        if(_reply->str == nullptr) return false;
        return true;
    }
    const std::string RedisUtil::getString(const std::string& key){
        _reply = (redisReply*)redisCommand(_context.getContext(), "GET %s", key.c_str());
        if(_reply->str == nullptr) return "";
        std::string str = _reply->str;
        freeReplyObject(_reply);
        return str;
    }
    void RedisUtil::setString(const std::string& key, const std::string& value){
        redisCommand(_context.getContext(), "SET %s %s", key.c_str(), value.c_str());
    }

    const std::vector<std::string> RedisUtil::getList(const std::string& key){
        _reply = (redisReply*)redisCommand(_context.getContext(), "LLEN %s", key.c_str());
        auto len = _reply->integer;
        freeReplyObject(_reply);
        if(len == 0) {
            return {};
        }
        _reply = (redisReply*)redisCommand(_context.getContext(), "LRANGE %s 0 %s", key.c_str(), std::to_string(len).c_str());
        std::vector<std::string> result;
        len = _reply->elements;
        for(int i = 0; i < len; i++){
            if(_reply->element[i]->len == 0) continue;
            result.push_back(_reply->element[i]->str);
        }
        freeReplyObject(_reply);
        return result;
    }
    const int RedisUtil::addToList(const std::string& key, const std::string& value){
        _reply = (redisReply*)redisCommand(_context.getContext(), "LRANGE %s 0 %s %s", key.c_str(), value.c_str());

        int len = _reply->integer;
        freeReplyObject(_reply);
        return len;
    }

    void RedisUtil::addToMap(const std::string& key, const std::string& field, const std::string& value){
        _reply = (redisReply*)redisCommand(_context.getContext(), "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
        freeReplyObject(_reply);
    }
    const std::string RedisUtil::getInMap(const std::string& key, const std::string& field){
        _reply = (redisReply*)redisCommand(_context.getContext(), "HGET %s %s", key.c_str(), field.c_str());
        if(_reply->len == 0){
            freeReplyObject(_reply);
            return "";
        }
        std::string result = _reply->str;
        freeReplyObject(_reply);
        return result;
    }

}