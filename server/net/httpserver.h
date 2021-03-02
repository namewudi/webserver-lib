#pragma once

#include "tcpserver.h"
#include "./utils/httpUtils.h"
#include "../base/buffer.h"
#include "httpresponse.h"
#include "servlet.h"
#include "servletManager.h"
#include <assert.h>
#include <boost/noncopyable.hpp>
#include <memory>
namespace net{
    class HttpServer: private boost::noncopyable{
    public:
        HttpServer(int port, int threadNum = 3, int num = 100): _tcpServer(port, threadNum, num){
            _servletManager = std::make_shared<ServletManager>();
            _tcpServer.setMessageProcessor([this](TcpConnection* a, std::shared_ptr<base::CircleReadBuffer> b){
                std::cerr<<"message processing : start!"<<std::endl;
                std::shared_ptr<HttpRequest> req = std::make_shared<HttpRequest>();
                HttpUtils::parseRequest(b, req);
                std::shared_ptr<HttpResponse> resp = std::make_shared<HttpResponse>();
                this->_servletManager->handle(req, resp);
                //a->send(resp->getAsString());
                //a->finishSend();
                a->sendInfinite(resp->getAsString());//大文件传输
                if(req->getHeader("Connection") != "keep-alive"){
                    a->handleClose();
                }
                a->finishMessageProcss();
                std::cerr<<"message processing : end!"<<std::endl;
            });
        }

        void start(){
            _tcpServer.start();
        }
        void registServlet(std::shared_ptr<Servlet> servlet){
            _servletManager->regist(servlet);
        }
        
    private:
        std::shared_ptr<ServletManager> _servletManager;
        TcpServer _tcpServer;
    };
}