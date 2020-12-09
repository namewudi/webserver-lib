#pragma once
#include "../base/buffer.h"
#include <string>
#include <unordered_map>
namespace net{
    class HttpRequest{
    public:
        HttpRequest(base::Buffer* buffer): _data(std::move(buffer->getAllAsString())){}
        bool phaseRequestLine(int, int);
        bool phaseRequestHeader(int, int);
        bool phase();
        const std::string getMethod()const;
        const std::string getUrl()const;
        const std::string getVersion()const;
        const std::string getHeader(const std::string&) const;
    private:
        std::unordered_map<std::string, std::string> _headerMap;
        std::string _data;
        std::string _method;
        std::string _url;
        std::string _version;
    };
}