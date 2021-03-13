# webserver-lib

__一个受muduo库启发设计的web服务器，可实现http服务器和tcp服务器。__
***
## 结构
服务器使用one loop per thread模式，包含两个线程池，其中一个线程池实现IO，另一个线程池实现信息的解码，逻辑运算及编码操作。客户连接会通过Acceptor分发给各个IO线程，IO线程会领用闲置Channel进行数据传输。
***
## 特点
1. 仿servlet接口
    >用户只需专注doGet和doPost的实现
2. 支持session会话
    >session会话基于cookie实现。cookie记录SESSION_ID，通过SESSION_ID可索引用户的状态信息
3. 支持filter
    >仿照servlet的filter设计，通过调用chain.doFilter(req, resp)将请求传递至下一级
4. 更高的内存复用
   >读写缓冲区均采用环形队列实现
***
## 注意事项
1. filter根据注册的顺序进行调用
2. 目前SESSION_ID根据时间进行编码，有较大安全隐患，后续会优化编码方式
## 已知bug
1. 同一个用户短时间内发送多次请求可能会造成请求丢失