# webserver-lib

### 受muduo启发设计的web服务器，可实现http服务器和tcp服务器。

## 结构
### 服务器使用one loop per thread模式，包含两个线程池，其中一个线程池实现IO，另一个线程池实现信息的解码，逻辑运算及编码操作。客户连接会通过Acceptor分发给各个IO线程，IO线程会领用闲置Channel进行数据传输。

## 特点
