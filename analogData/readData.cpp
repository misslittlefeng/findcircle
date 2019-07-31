#include "readData.h"

//���캯����ֻ�����ڵ�������
edge::edge(std::string  tempFront, std::string  tempNext) {
    this->frontNodeId = tempFront;
    this->nextNodeId = tempNext;
    this->arcId = computeNodePairId(this->frontNodeId, this->nextNodeId);

    this->frontToNext = 0;
    this->nextToFront = 0;
    this->offset = 0;
    this->sum = 0;
    this->offsetRatio = 0;
}

//���캯�����������п������õ���Ϣ����һЩ�ڱ�������û��ʹ�ù����Ժ���չ���ܵĹ����п�������
edge::edge(std::string  tempFront, std::string  tempNext, double tempFrontToNext, double tempNextToFront){
    //ȷ��offset�ܹ�������
    if ((tempFrontToNext - tempNextToFront) > 0) {
        this->frontNodeId = tempFront;
        this->nextNodeId = tempNext;
        this->arcId = computeNodePairId(this->frontNodeId, this->nextNodeId);
        
        this->frontToNext = (double)tempFrontToNext;
        this->nextToFront = (double)tempNextToFront;
        this->offset = this->frontToNext - this->nextToFront;
    }
    else {
        this->frontNodeId = tempNext;
        this->nextNodeId = tempFront;
        this->arcId = computeNodePairId(this->frontNodeId, this->nextNodeId);

        this->frontToNext = (double)tempNextToFront;
        this->nextToFront = (double)tempFrontToNext;
        this->offset = this->frontToNext - this->nextToFront;
    }

    this->sum = this->frontToNext + this->nextToFront;
    this->offsetRatio = this->offset / this->sum;
}

//���ص��ڲ�������ͬ�������ݹ�����һ�������Ψһ
bool edge::operator==(edge & tempArc) {
    return this->arcId == tempArc.arcId;
}

std::vector<edge> readDataFromFile() {
    std::vector<edge> result;

    char * myFileCharPath = new char[100];
	GetCurrentDirectory(sizeof(char) * 100, myFileCharPath);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //��ָ���ļ����µ��ƶ�·����ȡ����
    std::fstream myDataFile;
    myDataFile.open(myFilePathString + "/data/myData", std::ios::in);

    //�������ȶ�ȡΪvector��ʽ�е����ݣ������ķ���ʵ�ּ򵥣��ٶȽ�Ϊ����
    //�������ַ����ڴ�ռ�ö࣬��������ݵ�ʱ�������½�����
    std::vector<std::vector<int> > data;
    std::string tempLine;
    while (getline(myDataFile, tempLine)){
        std::stringstream tempStringStream(tempLine);
        std::vector<int> tempData;
        //δ�����С���޷�ʹ�õ�����
        for (int i = 0; i < NUMOFNODE; ++i) {
            int tempNum;
            tempStringStream >> tempNum;
            tempData.push_back(tempNum);
        }
        data.push_back(tempData);
        std::vector<int>().swap(tempData);
    }

    //д��Arc�ṹ����
    int length = (int)data.size();
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < i; ++j) {
            if (data[i][j]) {
				edge tempArc(i, j, data[i][j], data[j][i]);
                result.push_back(tempArc);
            }
        }
    }

    //���ڼ���Ƿ�ɹ�д�����ݣ��˲�����ʵ��ʹ���п���ֱ��ע�͵�
    //writeArcsToFile(result);

    //�ֶ��ͷ��ڴ�ȷ�����ᷢ��й¶����ֹ����û�м�ʱ��նѿռ���ɷǷ�����
    std::vector<std::vector<int> >().swap(data);
    myDataFile.close();
    return result;
}

void writeArcsToFile(std::vector<edge> & myArcs){
    char * myFileCharPath = new char[100];
	GetCurrentDirectory(sizeof(char)*100, myFileCharPath);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //��ָ���ļ����µ��ƶ�·����ȡ����
    std::fstream myArcsFile;
    myArcsFile.open(myFilePathString + "/data/myArcs", std::ios::out | std::ios::in | std::ios::trunc);
    
    //д�������ļ�
    for (std::vector<edge>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
        myArcsFile << "ArcId : " << it->arcId << " FrontId : " << it->frontNodeId << " NextId : " << it->nextNodeId << " Offset : " << it->offset << std::endl;
    }

    myArcsFile.close();
}