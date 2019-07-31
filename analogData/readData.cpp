#include "readData.h"

//构造函数，只保留节点编号数据
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

//构造函数，保留所有可能有用的信息，有一些在本代码中没有使用过，以后拓展功能的过程中可能有用
edge::edge(std::string  tempFront, std::string  tempNext, double tempFrontToNext, double tempNextToFront){
    //确保offset能够是正数
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

//重载等于操作符，同生成数据过程中一样，编号唯一
bool edge::operator==(edge & tempArc) {
    return this->arcId == tempArc.arcId;
}

std::vector<edge> readDataFromFile() {
    std::vector<edge> result;

    char * myFileCharPath = new char[100];
	GetCurrentDirectory(sizeof(char) * 100, myFileCharPath);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //从指定文件夹下的制定路径读取数据
    std::fstream myDataFile;
    myDataFile.open(myFilePathString + "/data/myData", std::ios::in);

    //将数据先读取为vector格式中的数据，这样的方法实现简单，速度较为理想
    //但是这种方法内存占用多，处理大数据的时候性能下降明显
    std::vector<std::vector<int> > data;
    std::string tempLine;
    while (getline(myDataFile, tempLine)){
        std::stringstream tempStringStream(tempLine);
        std::vector<int> tempData;
        //未定义大小，无法使用迭代器
        for (int i = 0; i < NUMOFNODE; ++i) {
            int tempNum;
            tempStringStream >> tempNum;
            tempData.push_back(tempNum);
        }
        data.push_back(tempData);
        std::vector<int>().swap(tempData);
    }

    //写入Arc结构体中
    int length = (int)data.size();
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < i; ++j) {
            if (data[i][j]) {
				edge tempArc(i, j, data[i][j], data[j][i]);
                result.push_back(tempArc);
            }
        }
    }

    //用于检测是否成功写入数据，此部分在实际使用中可以直接注释掉
    //writeArcsToFile(result);

    //手动释放内存确保不会发生泄露，防止后续没有及时清空堆空间造成非法访问
    std::vector<std::vector<int> >().swap(data);
    myDataFile.close();
    return result;
}

void writeArcsToFile(std::vector<edge> & myArcs){
    char * myFileCharPath = new char[100];
	GetCurrentDirectory(sizeof(char)*100, myFileCharPath);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //从指定文件夹下的制定路径读取数据
    std::fstream myArcsFile;
    myArcsFile.open(myFilePathString + "/data/myArcs", std::ios::out | std::ios::in | std::ios::trunc);
    
    //写入数据文件
    for (std::vector<edge>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
        myArcsFile << "ArcId : " << it->arcId << " FrontId : " << it->frontNodeId << " NextId : " << it->nextNodeId << " Offset : " << it->offset << std::endl;
    }

    myArcsFile.close();
}