# men

[format-in-google](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)

linux-command online search tool

men-server

* 基于自己实现的一个轻量网络库 [tiny-http-server](https://github.com/Howard0o0/TinyHttpServer)
* 使用redis作为cache,收到查找request时先到cache里查找，如cache miss则到man-online爬取结果,并存入cache

men-client

* web端
[web端](http://101.132.152.51:10000/)

* 命令行客户端
```bash
# search command "less"
men-client less
```

## 带业务测压(访问主页)

[tcp测压结果传送门](https://github.com/Howard0o0/TinyHttpServer)

```
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        688 bytes

Concurrency Level:      10000
Time taken for tests:   13.091 seconds
Complete requests:      1000000
Failed requests:        0
Keep-Alive requests:    1000000
Total transferred:      774000000 bytes
HTML transferred:       688000000 bytes
Requests per second:    76388.50 [#/sec] (mean)
Time per request:       130.910 [ms] (mean)
Time per request:       0.013 [ms] (mean, across all concurrent requests)
Transfer rate:          57738.96 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   14 246.1      0    7191
Processing:    23   97  32.3     99    1962
Waiting:        0   73  39.5     77    1962
Total:         24  111 260.3     99    9144

Percentage of the requests served within a certain time (ms)
  50%     99
  66%    105
  75%    109
  80%    111
  90%    121
  95%    131
  98%    145
  99%    166
 100%   9144 (longest request)

```





