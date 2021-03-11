#pragma once

#include "../tcp/tcpserver.h"
#include "../utils/httpUtils.h"
#include "../../base/buffer.h"
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
            _servletManager->init();
            _tcpServer.setMessageProcessor([this](TcpConnection* a, std::shared_ptr<base::CircleReadBuffer> b){
                std::cerr<<"message processing : start!"<<std::endl;
                std::shared_ptr<HttpRequest> req = std::make_shared<HttpRequest>();
                HttpUtils::parseRequest(b, req);
                if(req->hasHeader("Connection") && req->getHeader("Connection") == "keep-alive"){
                    a->setSendCompleteCallBack([](TcpConnection* conn){
                        conn->getChannel()->removeFromPoller();
                        conn->getChannel()->addToPoller(EPOLLIN);
                    });
                }
                else{
                    a->setSendCompleteCallBack([](TcpConnection* conn){
                        conn->handleClose();
                    });
                }
                std::shared_ptr<HttpResponse> resp = std::make_shared<HttpResponse>();
                this->_servletManager->handle(req, resp);
                a->sendInfinite(resp->getAsString());//大文件传输
                a->finishMessageProcss();
                std::cerr<<"message processing : end!"<<std::endl;
            });
        }

        void start(){
            _tcpServer.start();
        }
        void setStaticResourcePath(const std::string& path){_servletManager->setStaticResourcePath(path);}
        void registServlet(std::shared_ptr<Servlet> servlet){
            _servletManager->regist(servlet);
        }
        void registFilter(std::shared_ptr<Filter> filter){
            _servletManager->registFilter(filter);
        }
    private:
        std::shared_ptr<ServletManager> _servletManager;
        TcpServer _tcpServer;
    };
}