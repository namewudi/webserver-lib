#include "../httpserver.h"
#include <chrono>
#include <iostream>
#include <iomanip>
using namespace std;
using namespace net;
using namespace base;

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

class Myservlet1:public Servlet{
public:
    Myservlet1(){
        Servlet::setPath("/hello");
    }
    void doGet(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        std::string parameter = req->getParameters();
        resp->addBody("<html><head><title>This is title </title></head>"
        "<body><h1>URL: " + req->getUrl() + "</h1>Now is " + getTime() + "<br>" +
        "parameter :<br/>" + parameter+ 
        "<br/> method :" + HttpUtils::parseToString(req->getMethod()) + "<br/>version :" + 
        HttpUtils::parseToString(req->getVersion()) + "</body></html>");
    }
    void doPost(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        this->doGet(req, resp);
    }
};

class Myservlet2:public Servlet{
public:
    Myservlet2(){
        Servlet::setPath("/hello/world");
    }
    void doGet(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        resp->addBody("<html><head><title>This is title </title></head>"
        "<body><h1>URL: " + req->getUrl() + "</h1>Message:hello world</body></html>");
    }
    void doPost(std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp)override{
        this->doGet(req, resp);
    }
};


int main(){
    HttpServer server(9527);
    server.registServlet(std::shared_ptr<Servlet>(new Myservlet1));
    server.registServlet(std::shared_ptr<Servlet>(new Myservlet2));
    server.start();
    return 0;
}