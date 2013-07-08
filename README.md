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

<html><head><title></title><script type='text/javascript'>function t(c){for(var b=c.parentNode.parentNode.parentNode.getElementsByTagName('tr'),a=0;a<b.length;a++)'hide'==b[a].className?(b[a].className='show',c.innerHTML='[-] Collapse'):'show'==b[a].className&&(b[a].className='hide',c.innerHTML='[+] Expand')};function a(c){var b=c.parentNode.parentNode.nextElementSibling;'a-hide'==b.className?(b.className='a-show',c.innerHTML='[-]'):'a-show'==b.className&&(b.className='a-hide',c.innerHTML='[+]')};function y(c){for(var b=c.parentNode.parentNode.parentNode.getElementsByTagName('tr'),a=0;a<b.length;a++){ var name = b[a].className.split(' ');'hide'==name[0]?(b[a].className='show '+name[1],c.innerHTML='[-] Collapse'):'show'==name[0]&&(b[a].className='hide '+name[1],c.innerHTML='[+] Expand')}};function x(c){for(var b=c.parentNode.parentNode.parentNode.parentNode.getElementsByClassName(c.id),a=0;a<b.length;a++){var name=b[a].className.split(' ');'hide'==name[0]?(b[a].className='show '+name[1],c.innerHTML='[-]collapse'):'show'==name[0]&&(b[a].className='hide '+name[1],c.innerHTML='[+]expand')}};</script><style type='text/css'>body {   font-family: Verdana;   font-size: 11px;}table.a1,table.a2 {   border-spacing: 0;   font-size: 11px;   margin: 5px;   table-layout: fixed;   white-space: nowrap;}table.a1 { width: 600px }table.a2 {   background-color: #EEE;   padding: 0 5px;   width: 590px;}.head {   background-color: #222;   color: #FFF;   padding: 5px;}.head span,.s {   cursor: pointer;   font-size: 9px;}.r { text-align: right }.red {   color: #D20B2C;   font-weight: 700;}.lnk {   font-weight:bold;   font-size:10px;}a { color: #222 }.desc {   background-color: #EEE;   color: #222;   padding: 5px;}.d1,.d2 {   overflow: hidden;   text-overflow: ellipsis;}.d1 { border-bottom: 1px dotted #eee }.d2 { border-bottom: 1px dotted #ccc }.bar {   background-color: #777;   border-right: 1px #FFF solid;   height: 10px;}.a-hide,.hide { display: none }</style>
</head>
<body>
<table class='a1'><col style='width:60px'><col style='width:80px'><col style='width:100px'><col style='width:360px'>
<tr><td class='head' colspan='3'> Request Time</td><td class='head r'><span onclick='t(this)'>Expand [+]</span></td></tr><tr><td class='desc' colspan='4'> Different request time by number - time - percent - bandwidth</td></tr><tr><td class='d1'>1</td><td class='d1'>0.00</td><td class='d1'>10.000000%</td><td class='d1'><div class='bar' style='width:10.000000%'></div></td></tr><tr><td class='d1'>2</td><td class='d1'>0.01</td><td class='d1'>20.000000%</td><td class='d1'><div class='bar' style='width:20.000000%'></div></td></tr><tr><td class='d1'>1</td><td class='d1'>0.06</td><td class='d1'>10.000000%</td><td class='d1'><div class='bar' style='width:10.000000%'></div></td></tr><tr><td class='d1'>1</td><td class='d1'>0.07</td><td class='d1'>10.000000%</td><td class='d1'><div class='bar' style='width:10.000000%'></div></td></tr><tr><td class='d1'>2</td><td class='d1'>0.08</td><td class='d1'>20.000000%</td><td class='d1'><div class='bar' style='width:20.000000%'></div></td></tr><tr><td class='d1'>2</td><td class='d1'>0.09</td><td class='d1'>20.000000%</td><td class='d1'><div class='bar' style='width:20.000000%'></div></td></tr><tr><td class='d1'>1</td><td class='d1'>60.03</td><td class='d1'>10.000000%</td><td class='d1'><div class='bar' style='width:10.000000%'></div></td></tr></table><table class='a1'><col style='width:60px'><col style='width:60px'><col style='width:30px'><col style='width:40px'><col style='width:30px'><col style='width:60px'><col style='width:320px'>
<tr><td class='head' colspan='3'>Request URL</td><td colspan='4' class='head r'><span onclick='y(this)'>[+] Expand</span></td</tr><tr><td class='desc' colspan='7'>Top Different request url by number - percent - min time - max time - average time - request url</td></tr><tr><td class='d1'>8</td><td class='d1'>80.0000d><td class='d1'><span style='cursor:pointer' id='x2' onclick='x(this)'>[+]expand</span></td><td class='d1'>/weibo/topic/message</td></tr><tr class='hide x2'><td class='d1'>`-   2</td><td class='d1'>0.01</td><td colspan='3' class='d1'>25.000000%</td><td colspan='2' class='d1'><div class='bar' style='width:25.000000%'></div></td></tr><tr class='hide x2'><td class='d1'>`-   1</td><td class='d1'>0.06</td><td colspan='3' class='d1'>12.500000%</td><td colspan='2' class='d1'><div class='bar' style='width:12.500000%'></div></td></tr><tr class='hide x2'><td class='d1'>`-   2</td><td class='d1'>0.08</td><td colspan='3' class='d1'>25.000000%</td><td colspan='2' class='d1'><div class='bar' style='width:25.000000%'></div></td></tr><tr class='hide x2'><td class='d1'>`-   2</td><td class='d1'>0.09</td><td colspan='3' class='d1'>25.000000%</td><td colspan='2' class='d1'><div class='bar' style='width:25.000000%'></div></td></tr><tr class='hide x2'><td class='d1'>`-   1</td><td class='d1'>60.03</td><td colspan='3' class='d1'>12.500000%</td><td colspan='2' class='d1'><div class='bar' style='width:12.500000%'></div></td></tr><tr><td class='d1'>1</td><td class='d1'>10.000000%</td><td class='d1'>0.07</td><td class='d1'>0.07</td><td class='d1'>0.07</td><td class='d1'><span style='cursor:pointer' id='x1' onclick='x(this)'>[+]expand</span></td><td class='d1'>/weibo/comment/receive</td></tr><tr class='hide x1'><td class='d1'>`-   1</td><td class='d1'>0.07</td><td colspan='3' class='d1'>100.000000%</td><td colspan='2' class='d1'><div class='bar' style='width:100.000000%'></div></td></tr><tr><td class='d1'>1</td><td class='d1'>10.000000%</td><td class='d1'>0.00</td><td class='d1'>0.00</td><td class='d1'>0.00</td><td class='d1'><span style='cursor:pointer' id='x0' onclick='x(this)'>[+]expand</span></td><td class='d1'>/static/images/cloud.gif</td></tr><tr class='hide x0'><td class='d1'>`-   1</td><td class='d1'>0.00</td><td colspan='3' class='d1'>100.000000%</td><td colspan='2' class='d1'><div class='bar' style='width:100.000000%'></div></td></tr></table>
</body>
</html>
