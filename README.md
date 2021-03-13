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
***
## 压力测试
测试工具:ab   
测试参数：-n 10000 -c 10000
```
This is ApacheBench, Version 2.3 <$Revision: 1706008 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 114.213.207.246 (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        
Server Hostname:        114.213.207.246
Server Port:            9527

Document Path:          /login.html
Document Length:        1646 bytes

Concurrency Level:      10000
Time taken for tests:   0.711 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      17880000 bytes
HTML transferred:       16460000 bytes
Requests per second:    14067.11 [#/sec] (mean)
Time per request:       710.878 [ms] (mean)
Time per request:       0.071 [ms] (mean, across all concurrent requests)
Transfer rate:          24562.50 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   17  26.6     11     169
Processing:    21   65  11.8     66      89
Waiting:       18   63  12.9     63      85
Total:         66   82  19.5     77     194

Percentage of the requests served within a certain time (ms)
  50%     77
  66%     79
  75%     83
  80%     85
  90%    107
  95%    127
  98%    138
  99%    190
 100%    194 (longest request)
 ```
