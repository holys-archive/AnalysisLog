#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

#define URL_MAX_LEN 2096

long totalRecordNum = 0;    //总的请求数
double totalRecordTime = 0; //总的请求时间
long totalURL = 0;

struct ReCord {
    string ip;
    string date;
    string requestURL;
    string status;
    int size;
    string referer;
    string userAgent;
    string httpForward;
    float requestTime;
};

//url计数结构
struct Count_Node {
    string node;
    long count;
    Count_Node(string n="", long c=0){
        node = n; count = c;
    }
};

//排序
class Count_Node_Cmp {
public:
    bool operator()(Count_Node& u1, Count_Node& u2) {
        return u1.count > u2.count;
    }
};

struct Part_Request_Time {
    float time;
    int count;
    Part_Request_Time(float t=0.0, int c=0) {
        time = t; count = c;
    }
};

class Part_Request_Time_Cmp {
public:
    bool operator()(Part_Request_Time& p1, Part_Request_Time& p2) {
        return p1.time < p2.time;
    }
};

//记录url的访问时间的平均值、最小值、最大值
struct Url_Time {
    float average;
    float minimum;
    float maximum;
    Url_Time(float v=0.0, float mi=0.0, float ma=0.0) {
        average = v; minimum = mi; maximum = ma;
    }
};

map<float, int> request_time_map;
map<string, int> request_url_map;               //统计url的数量
map<string, vector<float> > request_url_time_map; //记录访问url的时间
map<string, Url_Time> url_time_map;   //记录一个url的请求时间的最小值、最大值、平均值

vector<struct Count_Node> request_url_count;

bool isDigit(string token) {
    int len = token.length();
    for (int i = 0; i < len; ++i) {
        if (token[i] < '0' || token[i] > '9')
            return false;
    }
    return true;
}

//将每一行日志进行分割
ReCord parse_line(char *buffer, int &ok) {

    ReCord r;
    char* keyWord = NULL;
    char* sep = " ";
    int cnt = 0;
    bool flag = false;

    keyWord = strtok(buffer, sep);
    while(keyWord) {

        if(cnt == 0) {
            if (keyWord[0] > '0' && keyWord[0] < '9') {
                r.ip = keyWord;         //ip
                cnt++;
            }
            else
                break;    //如果第一行不是ip开始，则这行不属于日志
        }
        else if (cnt == 1) {
            if (keyWord[0] == '[') {
                char tmp[256];
                strncpy(tmp, keyWord+1, 11);
                tmp[11] = '\0';
                r.date = tmp;           //date
                cnt++;
            }
        }
        else if (cnt == 2) {            //request

            if (strcmp(keyWord, "\"-\"") == 0) {
                //cout << "matches" << endl;
                r.requestURL = keyWord;
                cnt++;
            }
            else if (keyWord[0] == '\"') {
                flag = true;
            }
            else if (flag && keyWord[0] == '/') {
                r.requestURL = keyWord;
                cnt++;
            }
        }
        else if (cnt == 3) {            //status
            if (isDigit(keyWord)) {
                r.status = keyWord;
                cnt++;
            }
        }
        else if (cnt == 4) {            //byte
            if (isDigit(keyWord)) {
                r.size = atoi(keyWord);
                cnt++;
            }
        }
        else if (cnt == 5) {            //http referer
            r.referer = keyWord;
            cnt++;
        }
        else if (cnt == 6) {            //user agent
            int len = strlen(keyWord);
            if (keyWord[0] == '\"' && keyWord[len-1] == '\"') {
                r.userAgent = keyWord;
                cnt++;
            }
            else {
                r.userAgent += keyWord;
                if (keyWord[len-1] == '\"') {
                    cnt++;
                } else {
                    r.userAgent += " ";
                }
            }
        }
        else if (cnt == 7) {            //http x forward for


            if(keyWord[0] == '\"') {
                r.httpForward = keyWord;
                cnt++;
            }
            else {
                r.httpForward = "";         //处理python代码情况, 没有r.httpforward

                int len = strlen(keyWord);
                keyWord[len-1] = '\0';
                r.requestTime = atof(keyWord)/1000000;  //记微秒改为秒
                cnt++;
            }

        }
        else if (cnt == 8) {            // request time
            int len = strlen(keyWord);
            keyWord[len-1] = '\0';
            r.requestTime = atof(keyWord);
        }

        keyWord = strtok(NULL, sep);
    }

    if (cnt == 0)
        ok = 0;
    else
        ok = 1;

    return r;
}

//　请求时间分布
void visitTime()
{
    int i = 0;

    priority_queue<Count_Node, vector<Count_Node>, Count_Node_Cmp> UrlCountQueue;
    map<float, int>::iterator it = request_time_map.begin();

    cout << "request time" << "\t\t" << "number" << endl;
    for(; it != request_time_map.end(); ++it, ++i) {
        cout << it->first << "\t\t" << it->second << endl;
        if (i==1000) break;
    }
    cout << i << endl;


}


// 访问URL最多的时间分布
void topVisitURL(int k)
{
    priority_queue<Count_Node, vector<Count_Node>, Count_Node_Cmp> UrlCountQueue;
    map<string, int>::iterator it = request_url_map.begin();

    int cnt = 0;
    Count_Node cn;
    for(; it != request_url_map.end(); ++it) {
        totalURL += it->second;

        if (cnt < k) {
            UrlCountQueue.push(Count_Node(it->first, it->second));
            cnt++;
        } else {
            cn = UrlCountQueue.top();
            if (it->second > cn.count) {
                UrlCountQueue.pop();
                UrlCountQueue.push(Count_Node(it->first, it->second));
            }
        }
    }

    //  cout << "访问url最多的时间分布" << endl;     //Node指url
    int i = 0;

    while (!UrlCountQueue.empty()) {
        cn= UrlCountQueue.top();

        //cout << cn.count <<"\t\t" <<  cn.node << "\n";
        request_url_count.push_back(cn);

        map<string, vector<float> >::iterator it = request_url_time_map.find(cn.node);
        vector<float> &v = it->second;

        //将每个url的平均时间、最小值、最大值记录下来
        float maximum = -1;
        float minimum = 100000;
        float sum = 0;

        Url_Time ut;

        for(int i = 0; i < v.size(); i++) {
            if (maximum < v[i])
                maximum = v[i];

            if (minimum > v[i])
                minimum = v[i];
            sum += v[i];
        }
        ut.maximum = maximum;
        ut.minimum = minimum;
        ut.average = sum / v.size();

        url_time_map[cn.node] = ut;
        UrlCountQueue.pop();
    }
}


void initAnalysisLog(FILE* fp)
{
    //char filePath[256] = "access.log";
    char buffer[URL_MAX_LEN] = "\0";
    ReCord record, r;
    int ok;

    while (fgets(buffer, URL_MAX_LEN, fp) != NULL) {

        if(buffer[0] < '0' || buffer[0] > '9') continue;    //过滤不属于同一格式的记录
        record = parse_line(buffer, ok);

        //r = record;
        //cout << r.ip << "\t" << r.date << "\t" << r.requestURL << "\t" << r.status
        //     << "\t" << r.size << "\t" << r.referer<< "\t" << r.userAgent << "\t" << r.httpForward  << "\t" << r.requestTime <<  endl;

        totalRecordTime += record.requestTime;

        int part1 = int(record.requestTime * 1000)%10;
        int part2 = int(record.requestTime * 1000)/10;

        if (part1 >= 5) {
            record.requestTime = (part2+1)*1.0/100;
        } else {
            record.requestTime = part2*1.0/100;
        }
        ++totalRecordNum;           //记录总的条数
        request_time_map[record.requestTime] += 1;
        request_url_map[record.requestURL] += 1;    //统计请求最多url

        //cout << record.requestTime << " " << part1 << " " << part2 << endl;
        request_url_time_map[record.requestURL].push_back(record.requestTime); //url相对的请求时间
    }
}

void print()
{
    printf("<html><head><title></title>");
    printf("<script type='text/javascript'>"
           "function t(c){for(var b=c.parentNode.parentNode.parentNode.getElementsByTagName('tr'),a=0;a<b.length;a++)'hide'==b[a].className?(b[a].className='show',c.innerHTML='[-] Collapse'):'show'==b[a].className&&(b[a].className='hide',c.innerHTML='[+] Expand')};function a(c){var b=c.parentNode.parentNode.nextElementSibling;'a-hide'==b.className?(b.className='a-show',c.innerHTML='[-]'):'a-show'==b.className&&(b.className='a-hide',c.innerHTML='[+]')};"
           "function y(c){for(var b=c.parentNode.parentNode.parentNode.getElementsByTagName('tr'),a=0;a<b.length;a++){ var name = b[a].className.split(' ');'hide'==name[0]?(b[a].className='show '+name[1],c.innerHTML='[-] Collapse'):'show'==name[0]&&(b[a].className='hide '+name[1],c.innerHTML='[+] Expand')}};"
           "function x(c){for(var b=c.parentNode.parentNode.parentNode.parentNode.getElementsByClassName(c.id),a=0;a<b.length;a++){var name=b[a].className.split(' ');'hide'==name[0]?(b[a].className='show '+name[1],c.innerHTML='[-]collapse'):'show'==name[0]&&(b[a].className='hide '+name[1],c.innerHTML='[+]expand')}};"
           "</script>"
           "<style type='text/css'>"
           "body {   font-family: Verdana;   font-size: 11px;}table.a1,table.a2 {   border-spacing: 0;   font-size: 11px;   margin: 5px;   table-layout: fixed;   white-space: nowrap;}table.a1 { width: 1000px }table.a2 {   background-color: #EEE;   padding: 0 5px;   width: 590px;}.head {   background-color: #222;   color: #FFF;   padding: 5px;}.head span,.s {   cursor: pointer;   font-size: 9px;}.r { text-align: right }.red {   color: #D20B2C;   font-weight: 700;}.lnk {   font-weight:bold;   font-size:10px;}a { color: #222 }.desc {   background-color: #EEE;   color: #222;   padding: 5px;}.d1,.d2 {   overflow: hidden;   text-overflow: ellipsis;}.d1 { border-bottom: 1px dotted #eee }.d2 { border-bottom: 1px dotted #ccc }.bar {   background-color: #777;   border-right: 1px #FFF solid;   height: 10px;}.a-hide,.hide { display: none }"
           "</style>\n"
           "</head>\n"
           "<body>\n");
    //总的访问情况
    printf("<table class='a1'>"
           "<col style='width:200px'>"
           "<col style='width:200px'>"
           "<col style='width:200px'>"
           "<tr><td class='head' colspan='3'> General Visit</td></tr>"
           "<tr><td class='desc d1' >total number</td> <td class='desc d1'>total request time</td><td class='desc d1'>average time </td></tr>"
           "<tr><td class='d1'>%d</td><td class='d1'>%.3f</td><td class='d1'>%.3f</td></tr></table>", totalRecordNum, totalRecordTime, totalRecordTime/totalRecordNum);

    printf("<table class='a1'>"
           "<col style='width:60px'>"
           "<col style='width:80px'>"
           "<col style='width:100px'>"
           "<col style='width:360px'>\n"
           "<tr><td class='head' colspan='3'> Request Time</td><td class='head r'><span onclick='t(this)'>Expand [+]</span></td></tr>"
           "<tr><td class='desc d1'> number</td><td class='desc d1'>time</td> <td class='desc d1'>percent</td><td class='desc d1'>bandwidth</td></tr>");

    //　访问时间分布

    map<float, int>::iterator it = request_time_map.begin();

    for(int i = 0; it != request_time_map.end(); ++it, ++i) {
        //cout << it->first << "\t\t" << it->second << endl;
        float percent = (it->second*1.0/totalRecordNum)*100;
        if (i > 10)
            printf("<tr class='hide'><td class='d1'>%d</td><td class='d1'>%.2f</td><td class='d1'>%f%%</td><td class='d1'><div class='bar' style='width:%f%%'></div></td></tr>",it->second, it->first, percent, percent);
        else
            printf("<tr><td class='d1'>%d</td><td class='d1'>%.2f</td><td class='d1'>%f%%</td><td class='d1'><div class='bar' style='width:%f%%'></div></td></tr>", it->second, it->first, percent, percent);
    }
    printf("</table>");


    //　访问Url最多分布
    printf("<table class='a1'>"
           "<col style='width:60px'>"
           "<col style='width:60px'>"
           "<col style='width:30px'>"
           "<col style='width:40px'>"
           "<col style='width:30px'>"
           "<col style='width:60px'>"
           "<col style='width:320px'>\n"
           "<tr><td class='head' colspan='3'>Request URL</td><td colspan='4' class='head r'><span onclick='y(this)'>[+] Expand</span></td</tr>"
           "<tr><td class='desc d1'>number</td><td class='desc d1'>percent</td><td class='desc d1'>min</td><td class='desc d1'>time</td><td class='desc d1' title='max time'>max time</td><td class='desc d1'>average time</td><td class='desc d1'>request url</td></tr>");


    for (int i = request_url_count.size()-1; i >= 0; --i) {
        Count_Node cn = request_url_count[i];
        float percent = cn.count*1.0/totalURL*100;

        Url_Time ut = url_time_map[cn.node];

        printf("<tr><td class='d1'>%d</td><td class='d1'>%f%%</td><td class='d1'>%.2f</td><td class='d1'>%.2f</td><td class='d1'>%.2f</td><td class='d1'><span style='cursor:pointer' id='x%d' onclick='x(this)'>[+]expand</span></td><td class='d1'>%s</td></tr>",  cn.count, percent, ut.minimum, ut.maximum, ut.average, i, cn.node.c_str());

        map<string, vector<float> >::iterator it = request_url_time_map.find(cn.node);
        map<float, long> part_time_map;
        vector<float> &v = it->second;
        for(int j = 0; j < v.size(); j++) {
            part_time_map[v[j]]++;
        }

        map<float, long>::iterator pit = part_time_map.begin();
        int k = 0;
        for (;pit != part_time_map.end(); ++pit, ++k) {
            float percent2 = pit->second*1.0/v.size() * 100;

            printf("<tr class='hide x%d'><td class='d1'>`-   %d</td><td class='d1'>%.2f</td><td colspan='3' class='d1'>%f%%</td><td colspan='2' class='d1'><div class='bar' style='width:%f%%'></div></td></tr>",i, pit->second, pit->first, percent2, percent2);
            if (k  > 130) break;
        }

    }
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");
}

int main(int argc, char *argv[])
{
    FILE *fp = NULL;

    if (argc < 2)
        fp = stdin;
    else {
        char *filePath = argv[1];
        fp = fopen(filePath, "r");
    }

 //   char *filePath = "access.log";
    initAnalysisLog(fp);
 //   visitTime();
    topVisitURL(500);
    print();
    return 0;
}

