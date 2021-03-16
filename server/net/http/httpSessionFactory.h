#pragma once
#include "httpSessionLocal.h"
#include "httpSessionRedis.h"
#include <memory>
namespace net{
    class HttpSessionFactory{
    public:
        virtual std::shared_ptr<HttpSession> getInstance() = 0;
    };

    class HttpSessionLocalFactory: public HttpSessionFactory{
    public:
        std::shared_ptr<HttpSession> getInstance(){
            return std::make_shared<HttpSessionLocal>();
        }
    };
    class HttpSessionRedisFactory: public HttpSessionFactory{
    public:
        HttpSessionRedisFactory(const std::string& host, int port){
            RedisUtil::connect(host, port);
        }
        std::shared_ptr<HttpSession> getInstance(){
            return std::make_shared<HttpSessionRedis>();
        }
    private:
    };
}