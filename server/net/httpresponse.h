#pragma once
#include "../base/buffer.h"
#include "utils/httpUtils.h"
#include <fstream>
#include <string>
#include <iostream>
namespace net{
    class HttpResponse{
    public:
        void addVersion(const HttpVersion& version){
            _data.append(HttpUtils::parseToString(version));
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
            addBlankLine();
            _data.append(s);
        }
        void addBlankLine(){
            _data.append("\r\n");
        }
        const std::string& getAsString() const{
            return _data;
        }
        bool addHtml(const std::string& path){
            std::ifstream ifs(path, std::ios::binary);
            std::string str(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(0));           
            ifs.close();
            std::cerr<<"html file size: "<<str.size()<<std::endl;
            this->addHeader("Content-Length", std::to_string(str.size()));
            this->addBlankLine();
            _data.append(str);
            return true;
        }
    private:
        std::string _data;
    };
}