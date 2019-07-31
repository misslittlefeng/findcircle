/*************************
2019-07-12
���������ļ������ɹ����Զ���¼�ļ�·��������Ҫ�ֶ��޸�
��Ҫ�޸ĵĵط�����createFile.h��#define����
NUMOFNODE���ڱ�ʾ�ڵ�����
NUMOFARC���ڱ�ʾ�ߵ�����

ע�͵��Ĳ���Ϊ��дset�������ϴ���һЩ����
*************************/

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "time.h"
#include <sys/stat.h> ��
#include <sys/types.h>



class createFile{
public: 
    createFile();
    void createDataFile();
    
private: 
    int numOfNode;
    int numOfArc;
    std::string dataFilePath;
    std::string folderPath;
};

struct nodePair {
    int nodePairId;
    int frontNodeId;
    int nextNodeId;
    int value;

    //nodePair * next;

    bool operator==(nodePair & tempNodePair);

    nodePair(int tempFront, int tempNext);
    void getValue(int tempValue);
};

int computeNodePairId(int tempA, int tempB);

bool ifNodePairExist(std::vector<nodePair> &judgeNodePair, nodePair & tempNodePair);

///*
//class mySet {
//private:
//    nodePair * front;
//    int mySetLength;
//    nodePair * last;
//
//    //mySet();
//public:
//    //����ͷָ��
//    nodePair * begin(); 
//    // ����ĩβָ��
//    nodePair * end();
//    //���س��� 
//    int length(); 
//    //�Խṹ���ʼ����ȫ���ÿ�
//    void initMySet();
//    //�жϽṹ���Ƿ�Ϊ��
//    bool isEmpty();
//    //�жϽṹ�����Ƿ�����Ҫ������
//    bool ifExist(nodePair tempNodePair);
//    //��ṹ���в�������
//    bool insert(nodePair tempNodePair);
//
//    //bool push(nodePair tempNodePair);
//    
//    //�ӽṹ����ɾ���ض�����
//    bool pop(nodePair tempNodePair);
//};
//*/
