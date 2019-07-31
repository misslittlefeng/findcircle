#include "createFile.h"

createFile::createFile() {
    //获取创建节点数量及有向边的数量
    numOfArc = NUMOFARC;
    numOfNode = NUMOFNODE;
    
    //获取工作路径
    char * myFileCharPath = new char[100];
    getcwd(myFileCharPath, sizeof(char)*100);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //在工作路径中创建数据文件夹
    folderPath = myFilePathString + "/data";
    mkdir(folderPath.c_str(), S_IRWXU);

    //修改对应文件路径
    dataFilePath = folderPath + "/myData";

	//std::cout << "Create file folder succeed!" << std::endl;
}

void createFile::createDataFile() {
    //创建或重新生成data文件
    std::fstream myDataFile;
    myDataFile.open(dataFilePath, std::ios::trunc | std::ios::in | std::ios::out);

    //生成随机数据
    srand((unsigned)time(NULL));
    
	//确保数据生成的过程中能够保证节点数量及有向边数量
	int count = 0;
	std::vector<nodePair> myArcs;
    while (count != NUMOFARC) {
		int tempFront = rand() % NUMOFNODE;
		int tempNext = rand() % NUMOFNODE;
		//确保获取的边界前后节点不同
		if (tempFront == tempNext)	continue;

		nodePair tempNodePair(tempFront, tempNext);
		
		if (ifNodePairExist(myArcs, tempNodePair))	continue;
		tempNodePair.getValue(rand() % 901 + 100);
		myArcs.push_back(tempNodePair);
		count ++;
	}
	
	//使用二维数组暂存数据
	//可以考虑修改这部分代码，这样生成数据的过程中内存占用较高，且处理速度较慢，可以考虑使用static直接生成空数据
	//生成数据基本为稀疏矩阵，使用static默认初始化的方式可以节省大量时间，但static实现过程中出现问题，没有成功初始化
	//或者采用邻接表的方式存储数据，邻接表的方式不仅效率能大幅度提升，并且处理能力也能提高很多，切更符合再平衡策略中的数据上报格式
	std::vector<std::vector<int> > myData;
	for (int i = 0; i < NUMOFNODE; ++i) {
		std::vector<int> tempData;
		for (int j = 0; j < NUMOFNODE; ++j) {
			tempData.push_back(0);
		}
		myData.push_back(tempData);
		std::vector<int>().swap(tempData);
	}
	for (std::vector<nodePair>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
		myData[it->frontNodeId][it->nextNodeId] = it->value;
	}	

	//将生成的数据写入文件中
	for (int i = 0; i < NUMOFNODE; ++i) {
		for (int j = 0; j < NUMOFNODE; ++j) {
			if (j <= i) {
				myDataFile << myData[i][j] << " ";
			}
			else {
				if (myData[j][i] != 0) {
					myDataFile << rand() % 901 + 100 << " ";
				}
				else myDataFile << 0 << " ";
			}

		}
		myDataFile << std::endl;
	}
	

	//手动控制内存，释放堆空间
	std::vector<nodePair>().swap(myArcs);
	std::vector<std::vector<int> >().swap(myData);
    
	myDataFile.close();

	//std::cout << "Create data file succeed" << std::endl;
}

//等于操作符重载，由于节点对ID唯一，可以使用节点对ID作为关键字进行配对
bool nodePair::operator== (nodePair & tempNodePair) {
	return this->nodePairId == tempNodePair.nodePairId;
}

nodePair::nodePair(int tempFront, int tempNext) {
	frontNodeId = tempFront;
	nextNodeId = tempNext;
	nodePairId = computeNodePairId(tempFront, tempNext);
	//next = NULL;
}

void nodePair::getValue(int tempValue) {
	value = tempValue;
}

//计算节点对编号，在节点数小于10000的前提下，此做法能够确保节点对编号唯一
int computeNodePairId(int tempA, int tempB) {
	return tempA * 10000 + tempB;
}

//如果基础结构能够使用set或树结构能够大幅度提升效率，但是代码比较复杂，需要手动重写set或map
//set使用红黑树存储，map使用哈希值进行存储，需要手动重写
bool ifNodePairExist(std::vector<nodePair> &judgeNodePair, nodePair & tempNodePair) {
	//正向反向两次判断
	int RtempNodePairId = computeNodePairId(tempNodePair.nextNodeId, tempNodePair.frontNodeId);
	for (std::vector<nodePair>::iterator it = judgeNodePair.begin(); it != judgeNodePair.end(); ++it) {
		if (*it == tempNodePair || it->nodePairId == RtempNodePairId) return true;
	}
	return false;
}

/*
nodePair * mySet::begin() {
	return front;
}

nodePair * mySet::end() {
	return last;
}

int mySet::length() {
	return mySetLength;
}

void mySet::initMySet() {
	front = NULL;
	last = NULL;
	mySetLength = 0;
}

bool mySet::isEmpty() {
	return !front;
}

bool mySet::ifExist(nodePair tempNodePair) {
	for(nodePair * it = begin(); it != end(); it = it->next) {
		if (*it == tempNodePair) return true;
	} 
	return false;
}

bool mySet::insert(nodePair tempNodePair) {
	nodePair insertNodePair = tempNodePair;
	if (front == NULL && last == NULL) {
		front = &insertNodePair;
		last = &insertNodePair;
		return 1;
	}
	else {
		if (!ifExist(insertNodePair)) {
			last->next = &insertNodePair;
			last = &insertNodePair;
			return 1;
		}
		else return 0;	
	}
}

bool mySet::pop(nodePair tempNodePair) {
	if (!ifExist(tempNodePair)) {
		return 0;
	}
	else {
		nodePair * it = begin();
		nodePair * it2 = it->next;

		if (*it == tempNodePair) {
			front = it2;
			return 1;
		}
		else {
			 for (it; it != end(); it = it->next, it2= it2->next) {
				 if (*it2 == tempNodePair) {
					 it->next = it2->next;
					 return 1;
				 }
			 }
		}
	}
}
*/