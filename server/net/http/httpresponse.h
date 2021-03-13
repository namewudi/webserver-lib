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
        HttpResponse(): _line(3){}
        void addVersion(const HttpVersion& version){
            _line[0] = HttpUtils::parseToString(version);
        }
        void addStatusCode(const std::string& s){
            _line[1] = s;
        }
        void addStatusMessage(const std::string& s){
            _line[2] = s;
        }
        void addHeader(const std::string& a, const std::string& b){
            _header.append(a);
            _header.append(": ");
            _header.append(b);
            addBlankLine(_header);
        }
        void addBody(const std::string& s){
            _content.append(s);
        }
        void addBlankLine(std::string& data){
            data.append("\r\n");
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
        const std::string getAsString() {
            std::string result;
            result.append(_line[0]);
            result.push_back(' ');
            result.append(_line[1]);
            result.push_back(' ');
            result.append(_line[2]);
            addBlankLine(result);
            for(auto cookie : _cookies){
                //std::cout<<"add cookie: "<<cookie.toString()<<std::endl;
                addHeader("Set-Cookie", cookie.toString());
            }
            return result + _header +  "\r\n" + _content;
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
        std::vector<std::string> _line;
        std::string _header;
        std::string _content;
        std::vector<Cookie> _cookies;
    };
}