针对nginx日志进行分析, 此时分析的日志格式如下:
log_format  access  '$remote_addr - $remote_user [$time_local] "$request"'
                    '$status $body_bytes_sent "$http_referer"'
                    '"$http_user_agent" "http_x_forwarded_for" $request_time';

AnalysisLog现只分析日志功能“
(1)$request_time的统计分布
(2)访问最多的url及其$request_time分布

使用:
(1)make
(2)analysislog access.log.part > report.html

