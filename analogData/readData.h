/*************************
2019-07-13
读取数据，结构简单

2019-07-18
仔细思考一下，这部分指令可以和findCircle部分合并在一起完成
合并之后可以节省大量时间和内存，不需要再重新手动控制内存
*************************/

#pragma once 

#include <cmath>
#include <algorithm>
//#include "../createData/createFile.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

#define NUMOFNODE 100
#define NUMOFARC 300
struct edge {
    //记录节点信息
    std::string arcId;
	std::string frontNodeId;
	std::string nextNodeId;
    
    //记录支付通道信息
    double frontToNext;
    double nextToFront;
    double offset;      //记录偏移量
    double sum;         //记录总量
    double offsetRatio; //记录偏移比例

    //Arc();
	edge(std::string  tempFront, std::string  tempNext);
	edge(std::string  tempFront, std::string  tempNext, double tempFrontToNext, double tempNextToFront);
    void reverse();
    bool operator==(edge & tempArc);
};

//从文件中读取数据
std::vector<edge> readDataFromFile();

//将Arcs格式数据写入文件中
//写入之后用于检查文件读取是否有问题
//对实际功能没有任何影响，如果不需要可以直接不调用
void writeArcsToFile(std::vector<edge> & myArcs);