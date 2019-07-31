/*************************
2019-07-12
生成数据文件，生成过程自动记录文件路径，不需要手动修改
需要修改的地方包括createFile.h中#define部分
NUMOFNODE用于表示节点数量
NUMOFARC用于表示边的数量

注释掉的部分为手写set，功能上存在一些问题
*************************/

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "time.h"
#include <sys/stat.h> 　
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
//    //返回头指针
//    nodePair * begin(); 
//    // 返回末尾指针
//    nodePair * end();
//    //返回长度 
//    int length(); 
//    //对结构体初始化，全部置空
//    void initMySet();
//    //判断结构体是否为空
//    bool isEmpty();
//    //判断结构体中是否有需要的内容
//    bool ifExist(nodePair tempNodePair);
//    //想结构体中插入数据
//    bool insert(nodePair tempNodePair);
//
//    //bool push(nodePair tempNodePair);
//    
//    //从结构体中删除特定数据
//    bool pop(nodePair tempNodePair);
//};
//*/
