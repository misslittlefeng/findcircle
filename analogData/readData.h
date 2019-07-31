/*************************
2019-07-13
��ȡ���ݣ��ṹ��

2019-07-18
��ϸ˼��һ�£��ⲿ��ָ����Ժ�findCircle���ֺϲ���һ�����
�ϲ�֮����Խ�ʡ����ʱ����ڴ棬����Ҫ�������ֶ������ڴ�
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
    //��¼�ڵ���Ϣ
    std::string arcId;
	std::string frontNodeId;
	std::string nextNodeId;
    
    //��¼֧��ͨ����Ϣ
    double frontToNext;
    double nextToFront;
    double offset;      //��¼ƫ����
    double sum;         //��¼����
    double offsetRatio; //��¼ƫ�Ʊ���

    //Arc();
	edge(std::string  tempFront, std::string  tempNext);
	edge(std::string  tempFront, std::string  tempNext, double tempFrontToNext, double tempNextToFront);
    void reverse();
    bool operator==(edge & tempArc);
};

//���ļ��ж�ȡ����
std::vector<edge> readDataFromFile();

//��Arcs��ʽ����д���ļ���
//д��֮�����ڼ���ļ���ȡ�Ƿ�������
//��ʵ�ʹ���û���κ�Ӱ�죬�������Ҫ����ֱ�Ӳ�����
void writeArcsToFile(std::vector<edge> & myArcs);