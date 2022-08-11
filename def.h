#ifndef DEF_H
#define DEF_H
//定义数据格式
struct CpuData
{
//    int time;
    double usage;
};
struct MemoryData
{
//    int time;
    double usage;
};
struct DiskData
{
//    int time;
    int diskNum;
    double readUsage;
    double wirteUsage;
};
struct NetData
{
//    int time;
    double readUsage;
    double wirteUsage;
};
struct NodeTime
{
    int node;
    int time;
};

const int DATACAPACITY = 200000;//一次能缓存的数据条数
const int TIMECONVERT = 1000000;
#endif // DEF_H
