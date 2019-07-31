/*************************
2019-07-22
���������ṹ���ָ���ɲ��ֽṹ���Ա����

2019-07-29
���ڿ�Ҫ��ô����ˣ�д����
*************************/

#include "../analogData/readData.h"
#include <windows.h>
#include <set>
#include <map>

#define MAXCIRCLELENGTH 30

class nodeForCircle {
public : 
    int circleNodeId;

    //��¼�ڵ��ǰ��ڵ㼰�����ڵ���Ϣ
    std::vector<nodeForCircle*> front;
    std::vector<nodeForCircle*> next;

    //���������һ�����ʱ����
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

    //��¼һ���ߵ�ǰ��ڵ㼰�����ڵ���Ϣ
    nodeForCircle * frontNode;
    nodeForCircle * nextNode;

    //ȷ��һ��·���Ƿ�������·���ϲ��õ���
    //���һ��·����������·���ϲ��õ��ģ���Ҫ��¼�ϲ�֮ǰ��·����Ϣ
    int mergeTag; 
    //0Ϊԭ���ʹ��ڵ�·��
    //1Ϊԭ�����ڲ����������ϲ�������ڵ�·��
    //2Ϊԭ�������ڵ�·������������·��
    //3Ϊԭ�����ڲ����������ϲ�������ڵ�·��
    std::vector<arcForCircle *> mergeArcs;

    //��¼һ���ߵ���Ϣ��Ŀǰ���õ���ϢΪƫ�������ܽ��
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
���Ĺ���֮һ����·����
1. ��ȡ����ΪreadData�еĶ�ȡ���ݣ����ֹ���ʹ��ָ����ʽ
2. ����Ԥ��������ɾ�����ýڵ㣬�ϲ����ַǹؼ�·��
3. ��·����
    3.1 ����������Ч·������
    3.2 ȥ��
    3.3 д���ļ�
4. ������Ч����Ϣ
*************************/