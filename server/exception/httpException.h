#pragma once
#include <exception>
namespace exception{
    class HttpException: public std::exception{
        const char* what() const noexcept override{
            return "Http error!";
        }
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