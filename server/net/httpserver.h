#pragma once

#include "tcpserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include <assert.h>
#include <boost/noncopyable.hpp>
namespace net{
    class HttpServer: private boost::noncopyable{
    public:
        using HttpCallBack = std::function<void(const HttpRequest*, HttpResponse&)>;
        HttpServer(int port, int threadNum = 3, int num = 100): _tcpServer(port, threadNum, num){
            _tcpServer.setMessageProcessor([this](TcpConnection* a, base::Buffer* b){
                HttpRequest hreq(b);
                assert(hreq.phase());
                HttpResponse hresp;
                _httpCallBack(&hreq, hresp);
                a->send(hresp.getAsString());
                a->finishSend();
            });
        }
        void setHttpCallBack(HttpCallBack cb){
            _httpCallBack = std::move(cb);
        }
        void start(){
            _tcpServer.start();
        }
    private:
        HttpCallBack _httpCallBack;
        TcpServer _tcpServer;
    };
}