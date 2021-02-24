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
    private:
        std::string msg;
    };
    class HttpVersionException: public HttpException{
        const char* what() const noexcept override{
            return "can't parse Http version!";
        }
    };
    class HttpMethodException: public HttpException{
        const char* what() const noexcept override{
            return "can't parse Http method!";
        }
    };
}