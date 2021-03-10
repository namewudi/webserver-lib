#pragma once
#include "../../base/buffer.h"
#include "../utils/httpUtils.h"
#include <fstream>
#include <string>
#include <iostream>
#include "cookie.h"
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
        void addCookie(const Cookie& cookie){
            _cookies.push_back(cookie);
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
            
            for(auto cookie : _cookies){
                std::cout<<"add cookie: "<<cookie.toString()<<std::endl;
                addHeader("Set-Cookie", cookie.toString());
            }
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
            this->addHeader("Content-Type", "text/css");
            _content.append(str);
            return true;
        }
    private:
        std::string _data;
        std::string _content;
        std::vector<Cookie> _cookies;
    };
}