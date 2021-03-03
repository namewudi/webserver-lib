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
            _data.append(": ");
            _data.append(b);
            addBlankLine();
        }
        void addBody(const std::string& s){
            _content.append(s);
        }
        void addBlankLine(){
            _data.append("\r\n");
        }
        void sendRedirect(const std::string& path){
            addBody(R"(<!DOCTYPE html ><html><head>
            <meta http-equiv="Content-Type" content="text/html;" charset="UTF-8" />)");             
            addBody("<script   language= 'javascript'>  ");
            addBody("     window.location.replace(\"" + path + "\")");
            addBody("</script>");                
            addBody("</head>");                  
            addBody("<body>");                     
            addBody("</body>");
            addBody("</html>");
        }
        const std::string& getAsString() {
            _data.append("\r\n");
            return _data.append(std::move(_content));
        }
        const int getContentLength() const{
            return _content.length();
        }
        bool addHtml(const std::string& path){
            std::ifstream ifs(path, std::ios::binary);
            if(!ifs.is_open()) return false;
            std::string str(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(0));           
            ifs.close();
            addVersion(HttpVersion::HTTP1_1);
            
            addStatusCode("200");
            addStatusMessage("OK");
            std::cerr<<"html file size: "<<str.size()<<std::endl;
            this->addHeader("Content-Length", std::to_string(str.size()));
            this->addHeader("Content-Type", "text/html; charset=utf-8");
            _content.append(str);
            return true;
        }
        bool addStaticResource(const std::string& path){
            std::string type;
            int m = path.length();
            type = std::string(path.begin() + m - 3, path.end());
            if(type == "css"){
                return addCSS(path);
            }
            return addHtml(path); 
        }
        bool addCSS(const std::string& path){
            std::ifstream ifs(path, std::ios::binary);
            if(!ifs.is_open()) return false;
            std::string str(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(0));           
            ifs.close();
            addVersion(HttpVersion::HTTP1_1);
            
            addStatusCode("200");
            addStatusMessage("OK");
            std::cerr<<"html file size: "<<str.size()<<std::endl;
            this->addHeader("Content-Length", std::to_string(str.size()));
            this->addHeader("Content-Type", "text/css");
            this->addBlankLine();
            _content.append(str);
            return true;
        }
    private:
        std::string _data;
        std::string _content;
    };
}