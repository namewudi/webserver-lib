#include "../buffer.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main(){
    using base::Buffer;
    Buffer b("lalala");
    Buffer c(b);
    int fd = open("/home/wudi/develop/serverLib/server/base/test/testBuffer.txt", O_RDONLY);
    Buffer d;
    d.readFromFd(fd);
    d.push_back('\n');
    d.append(c);
    for(int i = 0; i < d.size(); i++){
        std::cout<<d[i];
    }
    std::cout<<std::endl;
    close(fd);
    return 0;
}