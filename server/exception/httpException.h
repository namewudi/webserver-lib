#pragma once
#include <exception>
namespace exception{
    class HttpException: public std::exception{
    public:
        HttpException(){};
        HttpException(const std::string& msg){
            this->msg = msg;
        }
        HttpException(std::string&& msg){
            this->msg = std::move(msg);
        }
        const char* what() const noexcept override{
            return msg.c_str();
        }
        std::string msg;
    };
    class HttpVersionException: public HttpException{
    public:
        HttpVersionException(){};
        HttpVersionException(const std::string& msg):HttpException(msg){}
        HttpVersionException(std::string&& msg):HttpException(std::move(msg)){}
    };
    class HttpMethodException: public HttpException{
    public:
        HttpMethodException(){};
        HttpMethodException(const std::string& msg):HttpException(msg){}
        HttpMethodException(std::string&& msg):HttpException(std::move(msg)){}
    };
}