#include "../httpserver.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <strstream>
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

int main(){
    HttpServer server(9527);
    server.setHttpCallBack([](const HttpRequest* a, HttpResponse& b){
        b.addVersion(a->getVersion());
        b.addStatusCode("200");
        b.addStatusMessage("OK");
        b.addBody("<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + getTime() + "</body></html>");
    });
    server.start();
    return 0;
}