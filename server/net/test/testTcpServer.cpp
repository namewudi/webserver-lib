#include <iostream>
#include "../tcpserver.h"

using namespace std;
using namespace net;
using namespace base;
int main(){
    TcpServer tcpserver(9527);
    tcpserver.setConnectionCallBack([](const TcpConnection* conn){
        cout<<"hello! "<<conn->name()<<endl;
    });
    tcpserver.setMessageProcessor([](TcpConnection* a, Buffer* b){
        string s(b->getAllAsString());
        a->send(s);
        a->send(s);
        a->send(s);
        a->finishSend();
    });
    tcpserver.start();
    return 0;
}
