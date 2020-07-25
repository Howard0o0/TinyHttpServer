# men

linux-command online search tool

men-server

* 基于自己实现的一个轻量网络库 [tiny-http-server](https://github.com/Howard0o0/TinyHttpServer)
* 使用redis作为cache,收到查找request时先到cache里查找，如cache miss则到man-online爬取结果,并存入cache

men-client(命令行客户端)
```bash
# search command "less"
men-client less
```




