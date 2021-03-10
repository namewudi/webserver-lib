#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "httprequest.h"
#include "httpresponse.h"
namespace net{
    class Servlet{
        public:
            void setPath(std::string path){
                this->paths.push_back(std::move(path));
            }
            const std::vector<std::string>& getPaths()const{
                return this->paths;
            }
            virtual void doGet(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>) = 0;
            virtual void doPost(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>) = 0;
            
        private:
            std::vector<std::string> paths;
    };
}