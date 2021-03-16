#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<hiredis/hiredis.h>
namespace net{
    class RedisContextRAII{
    public:
        ~RedisContextRAII(){
            redisFree(_context);
            std::cout<<"连接释放"<<std::endl;
        }
        redisContext* const getContext(){
            return _context;
        }
        void setContext(redisContext* context){
            this->_context = context;
        }
    private:
        redisContext* _context;
    };
    class RedisUtil{
    public:
        RedisUtil(){}
        RedisUtil(const std::string& host, int port){
            connect(host, port);
        }
        static bool connect(const std::string& host, int port);
        static void setExpire(const std::string& key, int seconds);
        static bool hasStringKey(const std::string& key);
        static const std::string getString(const std::string& key);
        static void setString(const std::string& key, const std::string& value);
        static const std::vector<std::string> getList(const std::string& key);
        static const int addToList(const std::string& key, const std::string& value);
        static void addToMap(const std::string& key, const std::string& field, const std::string& value);
        static const std::string getInMap(const std::string& key, const std::string& field);
        static void free(){redisFree(_context.getContext());}
    private:
        static RedisContextRAII _context;
        static redisReply* _reply;
    };
}