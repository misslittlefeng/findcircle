#include "createFile.h"

createFile::createFile() {
    //��ȡ�����ڵ�����������ߵ�����
    numOfArc = NUMOFARC;
    numOfNode = NUMOFNODE;
    
    //��ȡ����·��
    char * myFileCharPath = new char[100];
    getcwd(myFileCharPath, sizeof(char)*100);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //�ڹ���·���д��������ļ���
    folderPath = myFilePathString + "/data";
    mkdir(folderPath.c_str(), S_IRWXU);

    //�޸Ķ�Ӧ�ļ�·��
    dataFilePath = folderPath + "/myData";

	//std::cout << "Create file folder succeed!" << std::endl;
}

void createFile::createDataFile() {
    //��������������data�ļ�
    std::fstream myDataFile;
    myDataFile.open(dataFilePath, std::ios::trunc | std::ios::in | std::ios::out);

    //�����������
    srand((unsigned)time(NULL));
    
	//ȷ���������ɵĹ������ܹ���֤�ڵ����������������
	int count = 0;
	std::vector<nodePair> myArcs;
    while (count != NUMOFARC) {
		int tempFront = rand() % NUMOFNODE;
		int tempNext = rand() % NUMOFNODE;
		//ȷ����ȡ�ı߽�ǰ��ڵ㲻ͬ
		if (tempFront == tempNext)	continue;

		nodePair tempNodePair(tempFront, tempNext);
		
		if (ifNodePairExist(myArcs, tempNodePair))	continue;
		tempNodePair.getValue(rand() % 901 + 100);
		myArcs.push_back(tempNodePair);
		count ++;
	}
	
	//ʹ�ö�ά�����ݴ�����
	//���Կ����޸��ⲿ�ִ��룬�����������ݵĹ������ڴ�ռ�ýϸߣ��Ҵ����ٶȽ��������Կ���ʹ��staticֱ�����ɿ�����
	//�������ݻ���Ϊϡ�����ʹ��staticĬ�ϳ�ʼ���ķ�ʽ���Խ�ʡ����ʱ�䣬��staticʵ�ֹ����г������⣬û�гɹ���ʼ��
	//���߲����ڽӱ�ķ�ʽ�洢���ݣ��ڽӱ�ķ�ʽ����Ч���ܴ�������������Ҵ�������Ҳ����ߺܶ࣬�и�������ƽ������е������ϱ���ʽ
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

	//�����ɵ�����д���ļ���
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
	

	//�ֶ������ڴ棬�ͷŶѿռ�
	std::vector<nodePair>().swap(myArcs);
	std::vector<std::vector<int> >().swap(myData);
    
	myDataFile.close();

	//std::cout << "Create data file succeed" << std::endl;
}

//���ڲ��������أ����ڽڵ��IDΨһ������ʹ�ýڵ��ID��Ϊ�ؼ��ֽ������
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

//����ڵ�Ա�ţ��ڽڵ���С��10000��ǰ���£��������ܹ�ȷ���ڵ�Ա��Ψһ
int computeNodePairId(int tempA, int tempB) {
	return tempA * 10000 + tempB;
}

//��������ṹ�ܹ�ʹ��set�����ṹ�ܹ����������Ч�ʣ����Ǵ���Ƚϸ��ӣ���Ҫ�ֶ���дset��map
//setʹ�ú�����洢��mapʹ�ù�ϣֵ���д洢����Ҫ�ֶ���д
bool ifNodePairExist(std::vector<nodePair> &judgeNodePair, nodePair & tempNodePair) {
	//�����������ж�
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