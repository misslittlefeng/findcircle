#include "findCircle.h"

nodeForCircle::nodeForCircle(int tempId) {
    this->circleNodeId = tempId;
}

std::vector<nodeForCircle> getNodeDataFromArc (std::vector<edge> & myArcs) {
    std::vector<nodeForCircle> result;
    for (int i = 0; i < NUMOFNODE; ++i) {
        nodeForCircle tempNode(i);
        tempNode.count = 0;
        tempNode.mark = 0;
        tempNode.position = 0;
        result.push_back(tempNode);
    }

    for (std::vector<edge>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
        result[it->frontNodeId].next.push_back(&result[it->nextNodeId]);
        result[it->nextNodeId].front.push_back(&result[it->frontNodeId]);
    }

    return result;
}

void writeNodesToFile(std::vector<nodeForCircle> & myNodes) {
    char * myFileCharPath = new char[100];
	GetCurrentDirectory(sizeof(char)*100, myFileCharPath);
    std::string myFilePathString = myFileCharPath;
    delete[] myFileCharPath;
    
    //从指定文件夹下的制定路径读取数据
    std::fstream myNodesFile;
    myNodesFile.open(myFilePathString + "/data/myNodes", std::ios::out | std::ios::in | std::ios::trunc);

    for (std::vector<nodeForCircle>::iterator it = myNodes.begin(); it != myNodes.end(); ++it) {
        myNodesFile << it->circleNodeId << " Fronts are : ";
        for (std::vector<nodeForCircle *>::iterator it2 = it->front.begin(); it2 != it->front.end(); ++it2) {
            myNodesFile << (*it2)->circleNodeId << " ";
        }
        myNodesFile << " Nexts are : ";
        for (std::vector<nodeForCircle *>::iterator it2 = it->next.begin(); it2 != it->next.end(); ++it2) {
            myNodesFile << (*it2)->circleNodeId << " ";
        }
        myNodesFile << std::endl;
    }

    myNodesFile.close();
}

arcForCircle::arcForCircle(int tempId) {
    this->ciecleArcId = tempId;
}

void writeCircleArcsToFile(std::vector<arcForCircle> & myArcs) {
    char * myFileCharPath = new char[100];
	GetCurrentDirectory(sizeof(char) * 100, myFileCharPath);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //从指定文件夹下的制定路径读取数据
    std::fstream myCircleArcsFile;
    myCircleArcsFile.open(myFilePathString + "/data/myCircleArcs", std::ios::out | std::ios::in | std::ios::trunc);

    for (std::vector<arcForCircle>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
        myCircleArcsFile << it->ciecleArcId << " " << it->frontNode->circleNodeId << \
        " " << it->nextNode->circleNodeId << " " << it->value << " " << it->sum << std::endl;
    }

    myCircleArcsFile.close();
}

arcForCircle * findArc(int front, int next) {
    return intForArc[computeNodePairId(front, next)];
}

arcForCircle * findArc(nodeForCircle & front, nodeForCircle & next) {
    return findArc(front.circleNodeId, next.circleNodeId);
}

std::vector<arcForCircle> getCircleArcDataFromArc (std::vector<edge> & myArcs, std::vector<nodeForCircle> & myNodes){
    std::vector<arcForCircle> result;

    for(std::vector<edge>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
        arcForCircle tempArc((*it).arcId);
        tempArc.frontNode = &myNodes[it->frontNodeId];
        tempArc.nextNode = &myNodes[it->nextNodeId];
        tempArc.sum = it->sum;
        tempArc.value = it->offset;
        tempArc.mergeTag = 0;

        result.push_back(tempArc);
    }

    for(std::vector<arcForCircle>::iterator it = result.begin(); it != result.end(); ++it) {
        intForArc.insert(std::pair<int, arcForCircle*>(it->ciecleArcId, &(*it)));
    }

    return result;
}

bool checkFirstOrLast(std::vector<nodeForCircle> & myNodes) {
    for (std::vector<nodeForCircle>::iterator it = myNodes.begin(); it != myNodes.end(); ++it) {
        if (it->front.size() == 0 || it ->next.size() == 0) {
            return true;
        }
    }
    return false;
}

bool check(std::vector<nodeForCircle> & myNodes){
    if (myNodes.size() <= 3) return false;
    for (std::vector<nodeForCircle>::iterator it = myNodes.begin(); it != myNodes.end(); ++it) {
        if (it->front.size() == 1 && it ->next.size() == 1) {
            return true;
        }
    }
    return false;
}

void preTreatment(std::vector<nodeForCircle> & myNodes, std::vector<arcForCircle> & myArcs) {
    //删除所有起始节点或终止节点
    //指删除所有明显不构成环路的节点
    while (checkFirstOrLast(myNodes)) {
        for (std::vector<nodeForCircle>::iterator it = myNodes.begin(); it != myNodes.end(); ++it) {
            if (it->front.size() == 0 && it->next.size() == 0) {
                myNodes.erase(it);
            }
            else if (it->front.size() == 0 && it->next.size() != 0) {
                int nodeToDelete = it->circleNodeId; 
                //这特么不是一坨屎么
                for (std::vector<nodeForCircle*>::iterator it2 = it->next.begin(); it2 != it->next.end(); ++it2) {
                    int nodeToBeDelete = (*it2)->circleNodeId;
                    int arcToDelete = computeNodePairId(nodeToDelete, nodeToBeDelete);
                    intForArc.erase(arcToDelete);
                    for (std::vector<nodeForCircle*>::iterator it3 = (*it2)->front.begin(); \
                    it3 != (*it2)->front.end(); ++it3) {
                        if (*it3 == &(*it)) {(*it2)->front.erase(it3); break;} 
                    }
                }
                myNodes.erase(it);
            }
            else if (it->front.size() != 0 && it->next.size() == 0) {
                int nodeToDelete = it->circleNodeId; 
                //这特么不是另一坨屎么
                for (std::vector<nodeForCircle*>::iterator it2 = it->front.begin(); it2 != it->front.end(); ++it2) {
                    int nodeToBeDelete = (*it2)->circleNodeId;
                    int arcToDelete = computeNodePairId(nodeToBeDelete, nodeToDelete);
                    intForArc.erase(arcToDelete);
                    for (std::vector<nodeForCircle*>::iterator it3 = (*it2)->next.begin(); \
                    it3 != (*it2)->next.end(); ++it3) {
                        if (*it3 == &(*it))                         
                            (*it2)->next.erase(it3);
                    }
                }
                myNodes.erase(it);
            }
        }
    }
    
    while (check(myNodes)) {
        for (std::vector<nodeForCircle>::iterator it = myNodes.begin(); it != myNodes.end(); ++it) {
            if (it->front.size() == 1 && it->next.size() == 1) {
                if (intForArc[computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId)]) {
                    switch (intForArc[computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId)]->mergeTag) 
                    {
                    case 0 :
                    intForArc[computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId)]->mergeTag = 1;
                    break;
                    case 1 :
                    break;
                    case 2 : 
                    intForArc[computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId)]->mergeTag = 3;
                    break;
                    case 3 : 
                    break;
                    }
                    intForArc[computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId)]->mergeArcs.push_back(intForArc[computeNodePairId(it->front[0]->circleNodeId, it->circleNodeId)]);
                    intForArc[computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId)]->mergeArcs.push_back(intForArc[computeNodePairId(it->circleNodeId, it->next[0]->circleNodeId)]);
                }
                else {
                    arcForCircle tempArc(computeNodePairId(it->front[0]->circleNodeId, it->next[0]->circleNodeId));
                    tempArc.frontNode = &(*it->front[0]);
                    tempArc.nextNode = &(*it->next[0]);
                    tempArc.mergeTag = 2;
                    tempArc.sum = intForArc[computeNodePairId(it->front[0]->circleNodeId, it->circleNodeId)]->sum + \
                    intForArc[computeNodePairId(it->circleNodeId, it->next[0]->circleNodeId)]->sum;
                    tempArc.value = (intForArc[computeNodePairId(it->front[0]->circleNodeId, it->circleNodeId)]->value > intForArc[computeNodePairId(it->circleNodeId, it->next[0]->circleNodeId)]->value)? \
                    intForArc[computeNodePairId(it->circleNodeId, it->next[0]->circleNodeId)]->value : \
                    intForArc[computeNodePairId(it->front[0]->circleNodeId, it->circleNodeId)]->value;
                    myArcs.push_back(tempArc);
                }

                for (std::vector<nodeForCircle*>::iterator it2 = it->front[0]->next.begin(); \
                it2 != it->front[0]->next.end(); ++it2) {
                    if (*it2 == &(*it)) {
                        it->front[0]->next.erase(it2); break;
                    }
                }
                for (std::vector<nodeForCircle*>::iterator it2 = it->next[0]->front.begin(); \
                it2 != it->next[0]->front.end(); ++it2) {
                    if (*it2 == &(*it)) {
                        it->next[0]->front.erase(it2); break;
                    }
                }
                it->front[0]->next.push_back(it->next[0]);
                myNodes.erase(it);
            }
        }
    }
}

void circle::completeData() {
    this->minOffset = 10000;
    this->secondMinOffset = 10000;
    this->circleSum = 0;
    this->circleString = transArcsToString(this->circleArcs);

    for (std::vector<arcForCircle*>::iterator it = this->circleArcs.begin(); it != this->circleArcs.end(); ++it) {
        if (this->minOffset > (*it)->value) {
            this->secondMinOffset = this->minOffset;
            this->minOffset = (*it)->value;
            this->minPosition = &(*(*it));
        }
        this->circleSum += (*it)->sum;
    }
}

void circle::init() {
    this->circleString = "";
    this->circleSum = 0;
    this->minOffset = 0;
    this->minPosition = NULL;
    this->secondMinOffset = 0;
    std::vector<arcForCircle*>().swap(this->circleArcs);
}

void circle::show() {
    std::cout << this->circleId << " Nodes are : ";
    for (std::vector<arcForCircle*>::iterator it = this->circleArcs.begin(); it != this->circleArcs.end(); ++it) {
        std::cout << (*it)->frontNode << " ";
    }
    std::cout << std::endl;
}

std::string transNodesToString(std::vector<nodeForCircle>::iterator & nnbegin, \
std::vector<nodeForCircle>::iterator & nnend){
    std::vector<nodeForCircle>::iterator it = nnbegin;
    std::stringstream tempSS;
    while (it != nnend) {
        tempSS << it->circleNodeId << " ";
    }
    tempSS << it->circleNodeId;
    return tempSS.str();
}

std::string transArcsToString(std::vector<arcForCircle *> & myArcs){
    std::stringstream tempSS;
    std::vector<int> temp;
    for (std::vector<arcForCircle*>::iterator it = myArcs.begin(); it != myArcs.end(); ++it) {
        temp.push_back((*it)->frontNode->circleNodeId);
    }
    std::vector<int> temp2 = adjustOrder(temp);
    std::vector<int>().swap(temp);

    for (std::vector<int>::iterator it = temp2.begin(); it != temp2.end(); ++it) {
        tempSS << *it << " ";
    }
    std::vector<int>().swap(temp2);
    return tempSS.str();
}

std::vector<int> adjustOrder(std::vector<int> & temp) {
    std::vector<int> result;
    int min = 10000;
    int minPos;
    for (int i = 0; i < temp.size(); ++i) {
        if (temp[i] < min) {
            min = temp[i];
            minPos = i;
        }
    }

    for (int i = minPos; i < temp.size(); ++i) result.push_back(temp[i]);
    for (int i = 0; i < minPos; ++i) result.push_back(temp[i]);

    return result;
}

std::vector<circle> findCircles(std::vector<nodeForCircle> & myNodes) {
    std::vector<circle> result;
    int resentMark = 1;
    std::vector<nodeForCircle *> myStack;
    std::set<std::string> myStringCircles;
    myStack.push_back(&myNodes[0]);
    std::cout << "111" << std::endl;
    int circleCount = 1;

    while (1) {
        myStack[myStack.size() - 1]->mark = resentMark;
        for (int i = 0; i < myStack.size(); ++i) {
            std::cout << myStack[i]->circleNodeId;
        }
        std::cout << std::endl;
        circle tempCricle;
        tempCricle.circleId = circleCount;
        if (myStack[myStack.size() - 1]->count == myStack[myStack.size() - 1]->next.size()) {
            myStack[myStack.size() - 1]->count = 0;
            myStack.pop_back();
            myStack[myStack.size() - 1]->count++;
        }
        else {
            if (myStack[myStack.size() - 1]->next[myStack[myStack.size() - 1]->count]->mark != 0 && \
            myStack[myStack.size() - 1]->next[myStack[myStack.size() - 1]->count]->mark != resentMark) {
                continue;
            }
            else {
                bool findCircle = false;
                for (std::vector<nodeForCircle *>::iterator it = myStack.begin(); it != myStack.end(); ++it) {
                    if (&(*(*it)) == &(*myStack[myStack.size() - 1]->next[myStack[myStack.size() - 1]->count])) findCircle = true;
                    if (findCircle) {
                        tempCricle.circleArcs.push_back(findArc((*(*it)), (*(*it)->next[(*it)->count])));
                    }
                }
                if (!findCircle) {
                    myStack.push_back(&(*myStack[myStack.size() - 1]->next[myStack[myStack.size() - 1]->count]));
                }
            }
        }

        if (!tempCricle.circleArcs.empty() && (int)tempCricle.circleArcs.size() <= 30) {
            int circleNum = myStringCircles.size();
            std::string tempStringCircle = transArcsToString(tempCricle.circleArcs);
            myStringCircles.insert(tempStringCircle);
            if (circleNum == myStringCircles.size()) {
                tempCricle.init();
            }
            else {
                tempCricle.completeData();
                tempCricle.show();
                result.push_back(tempCricle);
                circleCount ++;
            }  
        }
        else if (!tempCricle.circleArcs.empty()) tempCricle.init();

        if (myStack.size() == 0) {
			resentMark++;
			bool isAll = true;
			for (std::vector<nodeForCircle>::iterator it = myNodes.begin(); it != myNodes.end(); ++it) {
				if (it->mark == 0) {
					isAll = false;
					myStack.push_back(&(*it));
					break;
				}
			}
			if (isAll) {
				return result;
			}
		}
    }
    return result;
}

void writeCirclesTofile(std::vector<circle> & myCircles) {
    char * myFileCharPath = new char[100];
    //getcwd(myFileCharPath, sizeof(char)*100);
	GetCurrentDirectory(sizeof(char) * 100, myFileCharPath);
    std::string myFilePathString(myFileCharPath);
    delete[] myFileCharPath;
    
    //从指定文件夹下的制定路径读取数据
    std::fstream myCirclesFile;
    myCirclesFile.open(myFilePathString + "/data/myCircles", std::ios::out | std::ios::in | std::ios::trunc);
    
    //写入数据文件
    for (std::vector<circle>::iterator it = myCircles.begin(); it != myCircles.end(); ++it) {
        myCirclesFile << "Circle ID : " << it->circleId;
        myCirclesFile << "Min Value : " << it->minOffset;
        myCirclesFile << "Sum Value : " << it->circleSum;
        myCirclesFile << "Total Nodes : " << (int)it->circleArcs.size() << std::endl;;
        for (std::vector<arcForCircle *>::iterator it2 = it->circleArcs.begin(); it2 != it->circleArcs.end(); ++it2) {
            myCirclesFile << (*it2)->frontNode << " ";
        }
        myCirclesFile << std::endl;
    }

    myCirclesFile.close();
}