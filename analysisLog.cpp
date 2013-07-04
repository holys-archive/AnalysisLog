#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <queue>
#include <set>

using namespace std;


#define URL_MAX_LEN 2096


// 记录host的结构
struct HostsRecord {
    string ip;
    long count;
    HostsRecord(string p="", long c=0) {
        ip = p; count = c;
    }
};

// 自定义host比较类
class CompareHosts {
public:
    bool operator()(HostsRecord& h1, HostsRecord& h2) {
        return h1.count > h2.count;
    }
};

// 记录状态码结构
struct StatusCode {
    string status;
    long count;
    StatusCode(string s="", long c=0) {
        status = s; count = c;
    }
};

//自定义statuscode比较类
class CompareStatus {
public:
    bool operator()(StatusCode& s1, StatusCode& s2) {
        return s1.count > s2.count;
    }
};

//url请求最大时间统计，时间可以是平均时间或其他
struct UrlMaxTime {
    string url;
    float time;     //请求时间
    float size;     //大小
    UrlMaxTime(string u="", float t=0.0, float s=0.0) {
        url = u; time = t; size = s;
    }
};

class CompareUrlTime {
public:
    bool operator()(UrlMaxTime& u1, UrlMaxTime& u2) {
        return u1.time > u2.time;
    }
};

//统计请求时间分布　
struct RequestTime {
    string time;
    long count;
    RequestTime(string t="", long c=0) {
        time = t; count = c;
    }
};

class CompareRequestTime {
public:
    bool operator()(RequestTime& r1, RequestTime& r2) {
        return r1.count > r2.count;
    }
};

struct Http404 {
    string url;
    long count;
    Http404(string u="", long c=0) {
        url = u; count = c;
    }
};
class CompareHttp404 {
public:
    bool operator()(Http404& h1, Http404& h2) {
        return h1.count > h2.count;
    }
};

//参数:k表示前k数
void topKHosts(int k)
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";                    //分隔符
    FILE* fp = NULL;
    int i = 1, num = 0;


    map<string, int> hosts;
    for(;;++i) {
        sprintf(filePath, "file%d", i);
        fp = fopen(filePath, "r");

        if (NULL == fp)
            break;

        while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

            keyWord = strtok(buffer, sep);
            hosts[keyWord] += 1;

        }
        fclose(fp);
    }
    priority_queue<HostsRecord, vector<HostsRecord>, CompareHosts> hostQueue;

    int cnt = 0;
    HostsRecord hr;

    //用优先列队来统计
    for (map<string, int>::iterator it = hosts.begin(); it != hosts.end(); ++it) {
        num += it->second;
        if (cnt < k) {
            hostQueue.push(HostsRecord(it->first, it->second));
            ++ cnt;
        } else {
            hr = hostQueue.top();
            if(it->second > hr.count) {
                hostQueue.pop();
                hostQueue.push(HostsRecord(it->first, it->second));
            }
        }
    }
    cout << "访问主机分布:" << endl;
    while(!hostQueue.empty()) {
        hr = hostQueue.top();
        cout <<hr.count  << "\t\t" << hr.count*1.0/num << "\t\t" << hr.ip <<  endl;
        hostQueue.pop();
    }
}

bool isDigit(string str)
{
    int len = str.length();
    for (int i = 0; i < len; ++i) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

void topKStatusCodes(int k)
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";                    //分隔符
    FILE* fp = NULL;
    int i = 1, num = 0;


    map<string, int> codes;
    for(;;++i) {
        sprintf(filePath, "file%d", i);
        fp = fopen(filePath, "r");
        if (NULL == fp)
            break;

        while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

            keyWord = strtok(buffer, sep);
            int j = 1;

            while(keyWord) {

                if (3 == j && isDigit(keyWord) || 4 == j && isDigit(keyWord) || 5 == j && isDigit(keyWord)) { // 状态码在第三到五个位置
                    codes[keyWord] += 1;        // 统计
                    break;
                }
                keyWord = strtok(NULL, sep);
                ++j;
            }
        }
        fclose(fp);
    }

    priority_queue<StatusCode, vector<StatusCode>, CompareStatus> statusQueue;

    int cnt = 0;
    StatusCode sc;
    for(map<string, int>::iterator it = codes.begin(); it != codes.end(); ++it) {
        num += it->second;
        if (cnt < k) {
            statusQueue.push(StatusCode(it->first, it->second));
            ++ cnt;
        } else {
            sc =  statusQueue.top();
            if (it->second > sc.count) {
                statusQueue.pop();
                statusQueue.push(StatusCode(it->first, it->second));
            }
        }
    }

    cout << "HTTP状态码分布:" << endl;
    while(!statusQueue.empty()) {
        sc = statusQueue.top();
        cout <<sc.count  << "\t\t" << sc.count*1.0/num << "\t\t" << sc.status <<  endl;
        statusQueue.pop();
    }
}


void topKUrlRequestTime(int k)
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";                    //分隔符
    FILE* fp = NULL;
    int i = 1, num = 0;
    char tmp[256];

    map<string, vector<float> > urlTime;
    map<string, vector<float> > urlSize;

    for(;;++i) {
        sprintf(filePath, "file%d", i);
        fp = fopen(filePath, "r");
        if (NULL == fp)
            break;


        while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

            string url = "";
            float time = -1;
            float size = -1;

            keyWord = strtok(buffer, sep);
            int j = 1;

            while (keyWord) {
                if (3 == j && '/' == keyWord[0]) {
                    url = keyWord;
                    int pos = url.find('?');
                    if(pos >= 0) {
                        strncpy(tmp, keyWord, pos);
                        tmp[pos] = '\0';
                        url = tmp;
                    }
                }

                if (6 == j && url != "") {
                    try {
                        size = atof(keyWord);
                    } catch(double) {

                    }
                }

                if (8 == j && url != "") {
                    try {
                        time = atof(keyWord);
                    } catch(double) {
                        time = -2;      //表示未能得到所要求的时间
                    }
                    break;
                }

                keyWord = strtok(NULL, sep);
                ++j;
            }

            if (url != "" && size > -1 && time > -1) {
                urlTime[url].push_back(time);
                urlSize[url].push_back(size);

            }
        }
        cout << filePath << endl;
    }

    priority_queue<UrlMaxTime, vector<UrlMaxTime>, CompareUrlTime> urlTimeQueue;
    int cnt = 0;
    UrlMaxTime umt;

//此处只求出平均请求时间最大的前k个链接
//要求出最大时间，最小时间
//2013-07-04
//
    for(map<string, vector<float> >::iterator it = urlTime.begin(); it != urlTime.end(); ++it) {
        vector<float> &v = it->second;
        if(v.size() < 100) continue;

        float sumTime = 0, sumSize = 0, averageTime = 0, averageSize = 0;
        map<string, vector<float> >::iterator sit = urlSize.find(it->first);
        vector<float> &sv = sit->second;
        for(int n = 0; n < v.size(); ++n) {
            sumTime += v[n];
            sumSize += sv[n];
        }

        averageTime = sumTime/v.size();
        averageSize = sumSize/v.size();

        if (cnt < k) {
            urlTimeQueue.push(UrlMaxTime(it->first, averageTime, averageSize));
            ++cnt;
        } else {
            umt = urlTimeQueue.top();
            if (averageTime > umt.time) {
                urlTimeQueue.pop();
                urlTimeQueue.push(UrlMaxTime(it->first, averageTime, averageSize));
            }
        }
    }
    cout << "url的访问平均时间:";
    while(!urlTimeQueue.empty()) {
        umt = urlTimeQueue.top();
        cout << umt.time<< "\t\t\t" << umt.size << "\t\t" << umt.url << endl;
        urlTimeQueue.pop();
    }
}

void topKRequestTime(int k)
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";                    //分隔符
    FILE* fp = NULL;
    int i = 1, num = 0;
    char tmp[256];

    map<string, vector<float> > urlTime;
    map<string, vector<float> > urlSize;
    map<string, long> requestTimeMap;

   for(;;++i) {
        sprintf(filePath, "file%d", i);
        fp = fopen(filePath, "r");
        if (NULL == fp)
            break;

        while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

            string url = "";
            float time = -1;
            float size = -1;

            keyWord = strtok(buffer, sep);
            int j = 1;

            while (keyWord) {

                if (8 == j && keyWord[0] >= '0' && keyWord[0] <= '9') {
                    requestTimeMap[keyWord] += 1;           //　统计请求时间分布
                    break;
                }

                keyWord = strtok(NULL, sep);
                ++j;
            }
        }
    }



    priority_queue<RequestTime, vector<RequestTime>, CompareRequestTime> requestTimeQueue;

    int cnt = 0;
    RequestTime rt;
    for(map<string, long>::iterator it = requestTimeMap.begin(); it != requestTimeMap.end(); it++) {
       // cout << it->first << "\t\t" << it->second << endl;

        num += it->second;
        if (cnt < k) {
            requestTimeQueue.push(RequestTime(it->first, it->second));
            ++cnt;
        } else {
            rt = requestTimeQueue.top();
            if (it->second > rt.count) {
                requestTimeQueue.pop();
                requestTimeQueue.push(RequestTime(it->first, it->second));
            }
        }
    }
    cout << "请求时间分布:"<<endl;
    while(!requestTimeQueue.empty()) {
        rt = requestTimeQueue.top();
        cout << rt.count << "\t\t" << rt.count*1.0/num <<  "\t\t" <<rt.time << endl;
        requestTimeQueue.pop();
    }
}

void topKHttp404(int k)
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";
    FILE* fp = NULL;
    int i = 1, num = 0;

    map<string, long> http404Map;
    for(;;++i) {
        sprintf(filePath, "file%d", i);
        fp = fopen(filePath, "r");

        if (NULL == fp)
            break;

        while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

            int j = 1;
            string url = "";
            keyWord = strtok(buffer, sep);
            while(keyWord) {
                if (3 == j)
                    url = keyWord;

                if (5 == j && strcmp(keyWord, "404") == 0) {
                    http404Map[url] += 1;
                    break;
                }
                keyWord = strtok(NULL, sep);
                ++j;
            }
        }
        cout << filePath << endl;
        fclose(fp);
    }

    priority_queue<Http404, vector<Http404>, CompareHttp404> Http404Queue;
    int cnt = 0;
    Http404 h4;
    for(map<string, long>::iterator it = http404Map.begin(); it != http404Map.end(); ++it) {
        num += it->second;
        if (cnt < k) {
            Http404Queue.push(Http404(it->first, it->second));
            ++cnt;
        } else {
            h4 = Http404Queue.top();
            if (h4.count < it->second) {
                Http404Queue.pop();
                Http404Queue.push(Http404(it->first, it->second));
            }
        }
    }
    cout << "Http 404 错误统计:" << endl;
    while(!Http404Queue.empty()) {
        h4 = Http404Queue.top();
        cout << h4.count << "\t\t" << h4.count*1.0/num << "\t\t" << h4.url << endl;
        Http404Queue.pop();
    }
}

void UniqueVisitor()
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";
    FILE* fp = NULL;
    int i = 1, num = 0;

    map<string, long> totalBit;
    map<string, set<string> > markIP;
    for (;; ++i) {
       sprintf(filePath, "file%d", i);
        fp = fopen(filePath, "r");

        if (NULL == fp)
            break;

        while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

            int j = 1;
            string ip = "";
            string date = "";
            long size = -1;

            keyWord = strtok(buffer, sep);
            while(keyWord) {
                if (1 == j)
                    ip = keyWord;
                else if (2 == j) {
                    char tmp[256];
                    strncpy(tmp, keyWord+1, 11);
                    tmp[11] = '\0';
                    date = tmp;
                }
                else if(6 == j && isDigit(keyWord)) {  //这里还需要调整，有较大误差.
                    size = atoi(keyWord);
                }

                keyWord = strtok(NULL, sep);
                ++j;
            }
            if (ip != "" && date != "" && size > -1) {
                totalBit[date] += size;
                markIP[date].insert(ip);
            }
        }
        cout << filePath << endl;
        fclose(fp);
    }

    ////还需要调整，有较大误差.
    cout << "每天独立访客:" << endl;
    for(map<string, long>::iterator it = totalBit.begin(); it != totalBit.end(); ++it) {
        cout<< markIP[it->first].size() << "\t\t" << it->first << "\t\t" << it->second << endl;
    }
}

void topKRefererURLs(int k)
{
    char filePath[256] = "\0";
    char buffer[URL_MAX_LEN] = "\0";
    char* keyWord = NULL;
    char* sep = "|";
    FILE* fp = NULL;
    int i = 1, num = 0;


}

int main(int argc, char *argv[]) {
    topKHosts(6);
    topKStatusCodes(6);
    topKUrlRequestTime(6);
    topKRequestTime(6);
    topKHttp404(6);
    UniqueVisitor();
}
