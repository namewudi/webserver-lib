#pragma once
#include "../base/buffer.h"
namespace net{
    class HttpResponse{
    public:
        void addVersion(const std::string& s){
            _data.append(s);
            _data.push_back(' ');
        }
        void addStatusCode(const std::string& s){
            _data.append(s);
            _data.push_back(' ');
        }
        void addStatusMessage(const std::string& s){
            _data.append(s);
            _data.append("\r\n");
        }
        void addHeader(const std::string& a, const std::string& b){
            _data.append(a);
            _data.push_back(':');
            _data.append(b);
            _data.append("\r\n");
        }
        void addBody(const std::string& s){
            _data.append("\r\n");
            _data.append(s);
        }
        const std::string& getAsString() const{
            return _data;
        }
    private:
        std::string _data;
    };
}