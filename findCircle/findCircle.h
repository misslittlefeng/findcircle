/*************************
2019-07-22
完成数据体结构相关指令，完成部分结构体成员函数

2019-07-29
终于快要特么完成了，写吐了
*************************/

#include "../analogData/readData.h"
#include <windows.h>
#include <set>
#include <map>

#define MAXCIRCLELENGTH 30

class nodeForCircle {
public : 
    int circleNodeId;

    //记录节点的前序节点及后续节点信息
    std::vector<nodeForCircle*> front;
    std::vector<nodeForCircle*> next;

    //三个用于找环的临时变量
    int count;
    int mark;
    int position;

    nodeForCircle(int tempId);
};

std::vector<nodeForCircle> getNodeDataFromArc (std::vector<edge> & myArcs);
void writeNodesToFile(std::vector<nodeForCircle> & myNodes);

class arcForCircle {
public : 
    int ciecleArcId;

    //记录一条边的前序节点及后续节点信息
    nodeForCircle * frontNode;
    nodeForCircle * nextNode;

    //确认一条路径是否是其他路径合并得到的
    //如果一条路径是由其他路径合并得到的，需要记录合并之前的路径信息
    int mergeTag; 
    //0为原本就存在的路径
    //1为原本存在并且有其他合并情况存在的路径
    //2为原本不存在的路径，纯新生成路径
    //3为原本存在并且有其他合并情况存在的路径
    std::vector<arcForCircle *> mergeArcs;

    //记录一条边的信息，目前有用的信息为偏移量及总金额
    double value;
    double sum;

    arcForCircle(int tempId);
};

void writeCircleArcsToFile(std::vector<arcForCircle> & myArcs);

static std::map<int, arcForCircle*> intForArc;

arcForCircle * findArc(int front, int next);
arcForCircle * findArc(nodeForCircle & front, nodeForCircle & next);

std::vector<arcForCircle> getCircleArcDataFromArc (std::vector<edge> & myArcs, std::vector<nodeForCircle> & myNodes);

#if 0
class arcForMerge {
public :
    int mergeArcId;
    int frontArcId;
    int nextArcId;

    double value;
    double sum;

    arcForMerge(arcForCircle & arc1, arcForCircle & arc2);

    arcForMerge(arcForMerge & arc1, arcForMerge & arc2);
};
#endif

bool checkFirstOrLast(std::vector<nodeForCircle> & myNodes);

bool check(std::vector<nodeForCircle> & myNodes);

void preTreatment(std::vector<nodeForCircle> & myNodes, std::vector<arcForCircle> & myArcs);

class circle{
public : 
    int circleId;

    std::vector<arcForCircle *> circleArcs;

    double minOffset;
    double secondMinOffset;
    double circleSum;

    arcForCircle * minPosition;

    std::string circleString;

    void completeData();
    void init();
    void show();
};

std::string transNodesToString(std::vector<nodeForCircle>::iterator & nnbegin,std::vector<nodeForCircle>::iterator & nnend);

std::string transArcsToString(std::vector<arcForCircle *> & myArcs);

std::vector<int> adjustOrder(std::vector<int> & temp);

std::vector<circle> findCircles(std::vector<nodeForCircle> & myNodes);

void writeCirclesTofile(std::vector<circle> & myCircles);

/*************************
2019-07-22
核心功能之一，环路发现
1. 读取数据为readData中的读取数据，发现过程使用指定格式
2. 数据预处理，包括删除无用节点，合并部分非关键路径
3. 环路发现
    3.1 遍历整个有效路径数据
    3.2 去重
    3.3 写入文件
4. 返回有效的信息
*************************/