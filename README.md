# tiny http server

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
ab -n 100000 -c 1000 -k -r http://localhost:10000/
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

 **TinyHttpServer(长连接、４线程)**
 

``` 
Server Software:        
Server Hostname:        localhost
Server Port:            10000

Document Path:          /
Document Length:        108 bytes

Concurrency Level:      1000
Time taken for tests:   7.831 seconds
Complete requests:      1000000
Failed requests:        0
Keep-Alive requests:    1000000
Total transferred:      197000000 bytes
HTML transferred:       108000000 bytes
Requests per second:    127699.85 [#/sec] (mean)
Time per request:       7.831 [ms] (mean)
Time per request:       0.008 [ms] (mean, across all concurrent requests)
Transfer rate:          24567.26 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1  27.1      0    1042
Processing:     0    7   5.0      8     259
Waiting:        0    7   5.0      8     259
Total:          0    8  31.0      8    1298

Percentage of the requests served within a certain time (ms)
  50%      8
  66%      8
  75%      8
  80%      8
  90%      8
  95%      9
  98%      9
  99%      9
 100%   1298 (longest request)
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
