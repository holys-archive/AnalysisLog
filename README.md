### 1.针对nginx日志进行分析, 此时分析的日志格式如下###
<pre>
log_format  access  '$remote_addr - $remote_user [$time_local] "$request"'
                    '$status $body_bytes_sent "$http_referer"'
                    '"$http_user_agent" "http_x_forwarded_for" $request_time';
</pre>
### 2.AnalysisLog分析日志功能 ###
<pre>
(1)$request_time的统计分布
(2)访问最多的url及其$request_time分布
</pre>
### 3.使用 ###

<pre>
(1)编译
<code>make</code>
(2)运行
<code>analysislog access.log.part > report.html</code>
(3)linux上crontab定时运行,这里设置每５分钟运行一次
<code>*/5 * * * * /path/to/AnalysisLog/analysislog  /path/to/AnalysisLog/access.log  2>&1 > /path/to/AnalysisLog/report.html </code>
或<code>*/5 * * * * /bin/cat path/to/AnalysisLog/access.log | /path/to/AnalysisLog/analysislog > /path/to/AnalysisLog/report.html </code>
</pre>

### 4.access.log 日志 ###

<pre>
115.192.167.65 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "https://www.example.com/" "example" "-" 60.034
113.104.208.4 - - [27/Jun/2013:00:01:01 +0800] "GET /static/images/cloud.gif HTTP/1.1" 200 3476 "http://www.example.com/?isclient=true&version=2.2&utype=select" "example" "-" 0.000
182.121.203.146 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/comment/receive HTTP/1.1" 200 200 "https://www.example.com/" "example" "-" 0.069
222.185.248.170 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "https://www.example.com/" "example" "-" 0.079
114.246.128.170 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 136 "https://www.example.com/" "example" "-" 0.057
218.75.89.89 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "https://www.example.com/" "example" "-" 0.084
113.205.192.30 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "http://www.example.com/" "example" "-" 0.006
117.208.165.91 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "https://www.example.com/" "example" "-" 0.006
219.145.108.170 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "http://www.example.com/" "example" "-" 0.090
222.213.216.139 - - [27/Jun/2013:00:01:01 +0800] "POST /weibo/topic/message HTTP/1.1" 200 110 "http://www.example.com/" "example" "-" 0.094
</pre>

### 5.运行结果 ###

![result](http://f.cl.ly/items/0Q1u0L1G302w083f0H2R/Screenshot.png)
