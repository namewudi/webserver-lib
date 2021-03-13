#include "../../../server/myserver.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
using namespace net;
using namespace base;

const std::string basePath = "/home/amax/wudi/develop/serverLib/application/resources/static/";

string getTime() {
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	time_t a = std::chrono::system_clock::to_time_t(tp);
	tm temp;
	localtime_r(&a, &temp);
    stringstream ss;
    ss << put_time(&temp, "%X");
    string result;
    ss >> result;
	return result;
}

class Home:public Servlet{
public:
    Home(){
        Servlet::setPath("/");
        Servlet::setPath("/hello");
    }
    void doGet(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        resp->addHtml(basePath + "/index.html");
    }
    void doPost(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        this->doGet(req, resp);
    }
};

class LoginSuccess:public Servlet{
public:
    LoginSuccess(){
        Servlet::setPath("/hello/world");
    }
    void doGet(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        
        resp->addBody(R"(<!DOCTYPE html ><html><head>
    <meta http-equiv="Content-Type" content="text/html;" charset="UTF-8" /></head>)");
        resp->addBody("<body>Now is : " + getTime() + "<br>parameter: " + req->getParameters() + "</body></html>");
    }
    void doPost(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        this->doGet(req, resp);
    }
};


int main(){
    HttpServer server(9527);
    server.setStaticResourcePath(basePath);
    server.registServlet(std::shared_ptr<Servlet>(new Home));
    server.registServlet(std::shared_ptr<Servlet>(new LoginSuccess));
    server.start();
    return 0;
}