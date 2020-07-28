# tiny http server
[format-in-goole](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)

a tiny http server based on lockfree threadpool

* using pthread instead of std::thread
* lockfree threadpool(CAS)
* reactor model

## 增加文件描述符上限、半连接队列大小(tcp_max_syn_backlog)、全连接队列大小(somaxconn)后压测结果不变

<!-- [testing page](http://www.howard97.top) -->

# bench(without modify kernel params)

| cpu       |   ram    |
|----------|------------|
| i3-7100 CPU @ 3. 90GHz × 4 | 16G|

using apache ab :

``` shell
ab -n 1000000 -c 5000 -k  http://localhost:10000/
```

 **TinyHttpServer(长连接、４线程)**
 

``` 
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        108 bytes

Concurrency Level:      5000
Time taken for tests:   9.618 seconds
Complete requests:      1000000
Failed requests:        0
Keep-Alive requests:    1000000
Total transferred:      197000000 bytes
HTML transferred:       108000000 bytes
Requests per second:    103971.42 [#/sec] (mean)
Time per request:       48.090 [ms] (mean)
Time per request:       0.010 [ms] (mean, across all concurrent requests)
Transfer rate:          20002.31 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1  28.1      0    1084
Processing:    10   47  13.4     44     513
Waiting:       10   47  13.4     44     513
Total:         10   48  33.1     44    1590

Percentage of the requests served within a certain time (ms)
  50%     44
  66%     47
  75%     49
  80%     49
  90%     56
  95%     79
  98%     96
  99%    106
 100%   1590 (longest request)
 ```

 **Muduo(长连接、４线程)**

``` 
Server Software:        Muduo
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        112 bytes

Concurrency Level:      1000
Time taken for tests:   16.665 seconds
Complete requests:      1000000
Failed requests:        0
Keep-Alive requests:    1000000
Total transferred:      216000000 bytes
HTML transferred:       112000000 bytes
Requests per second:    60004.75 [#/sec] (mean)
Time per request:       16.665 [ms] (mean)
Time per request:       0.017 [ms] (mean, across all concurrent requests)
Transfer rate:          12657.25 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1  27.1      0    1038
Processing:     0   16  12.4     14     470
Waiting:        0   16  12.4     14     470
Total:          0   17  33.9     14    1500

Percentage of the requests served within a certain time (ms)
  50%     14
  66%     19
  75%     22
  80%     24
  90%     30
  95%     35
  98%     42
  99%     47
 100%   1500 (longest request))

 ```

**TinyHttpServer(单线程、长连接)**

``` 
Server Software:   	        
Server Hostname:        localhost   	
Server Port:            10000	

Document Path:          /	 
Document Length:        108 bytes	
Concurrency Level:      1000	 
Time taken for tests:   7. 379 seconds	
Complete requests:      1000000	
Failed requests:        0	
Keep-Alive requests:    1000000	
Total transferred:      197000000 bytes	
HTML transferred:       108000000 bytes	
Requests per second:    135518. 07 [#/sec] (mean)	
Time per request:       7. 379 [ms] (mean)	
Time per request:       0. 007 [ms] (mean, across all concurrent requests)	
Transfer rate:          26071. 35 [Kbytes/sec] received	

Connection Times (ms)	

              min  mean[+/-sd] median   max	

Connect:        0    0   0. 7      0      41	
Processing:     0    7   1. 2      7      20	
Waiting:        0    7   1. 2      7      20	
Total:          0    7   1. 5      7      54	

Percentage of the requests served within a certain time (ms)	
  50%      7	
  66%      7	
  75%      8	
  80%      8	
  90%      9	
  95%     10	
  98%     11	
  99%     11	
 100%     54 (longest request)	
 ```

 **muduo(单线程、长连接)**

``` 
Server Software:        Muduo
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        112 bytes

Concurrency Level:      1000
Time taken for tests:   15.708 seconds
Complete requests:      1000000
Failed requests:        0
Keep-Alive requests:    1000000
Total transferred:      216000000 bytes
HTML transferred:       112000000 bytes
Requests per second:    63659.95 [#/sec] (mean)
Time per request:       15.708 [ms] (mean)
Time per request:       0.016 [ms] (mean, across all concurrent requests)
Transfer rate:          13428.27 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1  26.6      0    1023
Processing:     1   15   8.1     15     466
Waiting:        0   15   8.1     15     466
Total:          1   16  31.9     15    1486

Percentage of the requests served within a certain time (ms)
  50%     15
  66%     16
  75%     17
  80%     18
  90%     21
  95%     25
  98%     29
  99%     32
 100%   1486 (longest request)

 ```
