#include "Graph.h"
#include <algorithm>
#include <QtGlobal>
//#include "Intersections.h"
//#include "SegmentContainer.h"

//void GraphSpace::BaseNode::addAdjacentNode(const BaseNode *cpAn)
//{
//	lstAdjacentNodes.push_back(cpAn);
//}

bool GraphSpace::BaseNode::operator==(const BaseNode &rhs) const
{
	if (this->iNumNode == rhs.iNumNode && this->lstAdjacentNodes == rhs.lstAdjacentNodes)
		return true;
	else
		return false;
}

void GraphSpace::BaseNode::adjacentNodes(std::list<int> &rResult) const
{
  for (std::list<int>::const_iterator cit = lstAdjacentNodes.begin(); cit != lstAdjacentNodes.end(); ++cit)
    rResult.push_back(*cit);
}

//bool GraphSpace::operator<(const BaseEdge &lhs, const BaseEdge &rhs) const
//{
//	return lhs.numNode() < rhs.numNode();
//}

void GraphSpace::Graph::addNode(BaseNode *pAn)
{
	mapNodes.insert(std::make_pair(pAn->numNode(), pAn));
  iNodeCount += 1;
}

void GraphSpace::Graph::addEdge(BaseNode *pAn_1, BaseNode *pAn_2)
{
	if (*pAn_1 == *pAn_2)
		return;
	
	pAn_1->lstAdjacentNodes.push_back(pAn_2->numNode());
	pAn_2->lstAdjacentNodes.push_back(pAn_1->numNode());
  iEdgeCount += 1;
}

const GraphSpace::BaseNode* GraphSpace::Graph::node(int numNode) const
{
	std::map<int, BaseNode*>::const_iterator cIt = mapNodes.find(numNode);
	if (cIt != mapNodes.end())
		return cIt->second;
	else
		return 0;
}

//GraphSpace::BaseNode* GraphSpace::Graph::node(int numNode) const
//{
//	return const_cast<BaseNode*>(static_cast<const BaseNode*>(node(numNode));
//}

void GraphSpace::Graph::removeNode(BaseNode *pAn)
{
  std::map<int, BaseNode*>::iterator it = mapNodes.find(pAn->numNode());
  if (it != mapNodes.end()) {
    delete it->second;
    mapNodes.erase(it);
    iNodeCount -= 1;
  }
}

void GraphSpace::Graph::removeEdge(BaseNode *pBn_1, BaseNode *pBn_2)
{
  if (*pBn_1 == *pBn_2)
    return;
  std::list<int>::iterator it = std::find(pBn_1->lstAdjacentNodes.begin(), pBn_1->lstAdjacentNodes.end(), pBn_2->numNode());
  if (it == pBn_1->lstAdjacentNodes.end())
    return;
  else
    pBn_1->lstAdjacentNodes.erase(it);
  it = std::find(pBn_2->lstAdjacentNodes.begin(), pBn_2->lstAdjacentNodes.end(), pBn_1->numNode());
  if (it == pBn_2->lstAdjacentNodes.end())
    return;
  else
    pBn_2->lstAdjacentNodes.erase(it);
}

GraphSpace::Graph::~Graph()
{
	for (std::map<int, BaseNode*>::iterator it = mapNodes.begin(); it != mapNodes.end(); ++it)
		delete it->second;
}

//void GraphSpace::GraphContainer::addGraphs()
//{
//  using SegmentSpace::segments;
//  Intersections ints;
//
//  ints.findIntersections();
//  //std::multimap<const Segment*, const Segment*> mapInts = ints.intersections();
//  Graph *pGr = new Graph;
//  for (int i = 0; i < segments().segmentCount(); ++i)
//    pGr->addNode(new BaseNode(i));
//  for (std::multimap<const Segment*, const Segment*>::const_iterator cIt = ints.intersections().begin(); cIt != ints.intersections().end(); ++cIt)
//    pGr->addEdge(pGr->node(cIt->first->numSegment()), pGr->node(cIt->second->numSegment()));
//  mapGraphs.insert(std::make_pair(0, pGr));
//}
//
//GraphSpace::GraphContainer & GraphSpace::graphs()
//{
//  static GraphContainer grContainer;
//	return grContainer;
//}
//
//GraphSpace::GraphContainer::~GraphContainer()
//{
//	for (std::map<int, Graph* >::iterator it = mapGraphs.begin(); it != mapGraphs.end(); ++it) {
//		delete it->second;
//		mapGraphs.erase(it);
//	}
//}

//GraphSpace::BaseNode::~BaseNode()
//{
//	for (std::list<BaseNode*>::iterator it = lstAdjacentNodes.begin(); it != lstAdjacentNodes.end(); ++it) {
//		delete *it;
//		lstAdjacentNodes.erase(it);
//	}
//}

void GraphSpace::BaseNode::printAdjNodes() const
{
	for (std::list<int>::const_iterator cIt = lstAdjacentNodes.begin(); cIt != lstAdjacentNodes.end(); ++cIt)
		qDebug("%d  ", (*cIt));
}

void GraphSpace::MaxIndependentSet::findMaxIndependentSet(const Graph &crGr, std::list<int> &result)
{
	int k;
  std::list<int> tmpCandidates;
	std::list<int> tmpNotCandidates;
	std::list<int> adjNodes;
	int curNode;

	// шаг 1 (Начальная установка)
  k = 0;
	vecNodeSet.push_back(new NodeSet);
	for (int i = 0; i < crGr.nodeCount(); ++i)
		vecNodeSet[0]->CandidateNodes.push_back(i);
	
  bool step_5, state = true;
	while (/*k >= 0 && ! vecNodeSet[k]->CandidateNodes.empty()*/state) { 
		// шаг 2 (Прямой шаг) // подумать над условием в большом цикле // подумать над количеством циклов и условиями в них
    if ((k + 1) == vecNodeSet.size())
			vecNodeSet.push_back(new NodeSet(vecNodeSet[k]->NotCandidateNodes, vecNodeSet[k]->CandidateNodes));
		
		// выбираем вершину Xik из Qk+ и формируем Sk+1
		//curNode = vecNodeSet[k]->CandidateNodes.front();
		curNode = candidate(k, crGr, adjNodes); // не могу додуматься, но кажется, что candidate нет необходимости вызывать каждый раз на шаге 2
		lstIndSet.push_back(curNode);
    // получаем список вершин, смежных с вершиной curNode
    adjNodes.clear();
		crGr.node(curNode)->adjacentNodes(adjNodes);
		// формируем множества Qk+1+ и Qk+1-
		for (std::list<int>::const_iterator cit = adjNodes.begin(); cit != adjNodes.end(); ++cit) {
			vecNodeSet[k + 1]->CandidateNodes.remove_if(std::bind1st(std::equal_to<int>(), *cit));
			vecNodeSet[k + 1]->NotCandidateNodes.remove_if(std::bind1st(std::equal_to<int>(), *cit));
		}
    vecNodeSet[k + 1]->CandidateNodes.remove_if(std::bind1st(std::equal_to<int>(), curNode));
		k += 1;

		while (/*k != 0 || ! vecNodeSet[k]->CandidateNodes.empty()*/state) { 
			// шаг 3 (Проверка)
      if (returnCondition(k, crGr, adjNodes) || step_5) { 
				// шаг 5 (Шаг возвращения)
				delete vecNodeSet[k];
				vecNodeSet.pop_back();
        
				if (k == 0) {
					state = false; // в алгоритме этого нет, но мне пришлось вставить, надо разобраться
					break;
				}
				k -= 1;
				curNode = lstIndSet.back();
			  lstIndSet.pop_back();
				vecNodeSet[k]->CandidateNodes.remove_if(std::bind1st(std::equal_to<int>(), curNode));
			  vecNodeSet[k]->NotCandidateNodes.push_back(curNode);
				vecNodeSet[k]->NotCandidateNodes.sort();
				step_5 = false;
				if (k == 0 && vecNodeSet[k]->CandidateNodes.empty())
					state = false;
			  continue; // если не остановим алгоритм, то перейдем к шагу 3
		  }
      else { 
				// шаг 4
        if (vecNodeSet[k]->CandidateNodes.empty() && vecNodeSet[k]->NotCandidateNodes.empty()) {
					std::list<int> *maxIndSet = new std::list<int>(lstIndSet);
				  vecMaxIndSets.push_back(maxIndSet);
					if (maxIndSet->size() > vecMaxIndSets[iMaxMaxIndSet]->size()) {
						iMaxMaxIndSet = vecMaxIndSets.size() - 1;
					}
					step_5 = true;
				  continue; // переходим к шагу 5, это гарантирует step_5 = true
        }
				if (vecNodeSet[k]->CandidateNodes.empty() && ! vecNodeSet[k]->NotCandidateNodes.empty()) {
          step_5 = true;
					continue; // переходим к шагу 5, это гарантирует step_5 = true
				}
        else
          break; // переходим к шагу 2
      }
    }
  }
	result = *vecMaxIndSets[iMaxMaxIndSet];
}

bool GraphSpace::MaxIndependentSet::returnCondition(int k, const Graph &crGr, std::list<int> &adjNodes) const
{
	if (vecNodeSet[k]->NotCandidateNodes.empty())
		return false;
	if (vecNodeSet[k]->CandidateNodes.empty())
		return true;
	
	for (std::list<int>::const_iterator cit_1 = vecNodeSet[k]->NotCandidateNodes.begin(); cit_1 != vecNodeSet[k]->NotCandidateNodes.end(); ++cit_1) {
		adjNodes.clear();
		crGr.node(*cit_1)->adjacentNodes(adjNodes);
		for (std::list<int>::const_iterator cit_2 = adjNodes.begin(); cit_2 != adjNodes.end(); ++cit_2) { 
			for (std::list<int>::const_iterator cit_3 = vecNodeSet[k]->CandidateNodes.begin(); cit_3 != vecNodeSet[k]->CandidateNodes.end(); ++cit_3) {
				if (*cit_3 == *cit_2)
					return false;
			}
		}
	}
	return true;
}

int GraphSpace::MaxIndependentSet::candidate(int k, const Graph &crGr, std::list<int> &adjNodes) const
{
	/*int x;
	int count
	int min_count;*/
	std::list<int> intersec;
	std::list<int> min;
	
	for (std::list<int>::const_iterator cit_1 = vecNodeSet[k]->NotCandidateNodes.begin(); cit_1 != vecNodeSet[k]->NotCandidateNodes.end(); ++cit_1) {
		adjNodes.clear();
		crGr.node(*cit_1)->adjacentNodes(adjNodes);
		adjNodes.sort();
		intersec.clear();
		std::set_intersection(adjNodes.begin(), adjNodes.end(), vecNodeSet[k]->CandidateNodes.begin(), vecNodeSet[k]->CandidateNodes.end(), std::inserter(intersec, intersec.end()));
		if (intersec.size() < min.size())
			min = intersec;
	}
	if (min.empty())
		return vecNodeSet[k]->CandidateNodes.front();
	else
		return min.front();
	
	//for (std::list<int>::const_iterator cit_1 = vecNodeSet[k]->NotCandidateNodes.begin(); cit_1 != vecNodeSet[k]->NotCandidateNodes.end(); ++cit_1) {
	//	adjNodes.clear();
	//	crGr.node(*cit_1)->adjacentNodes(adjNodes);
	//	count = adjNodes.size();
	//	for (std::list<int>::const_iterator cit_2 = adjNodes.begin(); cit_2 != adjNodes.end(); ++cit_2) { 
	//		std::list<int>::const_iterator cit_3 = std::find(vecNodeSet[k]->CandidateNodes.begin(), vecNodeSet[k]->CandidateNodes.end(), *cit_2);
	//		if (cit_3 != vecNodeSet[k]->CandidateNodes.end()) {
	//			count += 1;

	//		}

	//		/*for (std::list<int>::const_iterator cit_3 = vecNodeSet[k]->CandidateNodes.begin(); cit_3 != vecNodeSet[k]->CandidateNodes.end(); ++cit_3) {
	//			if (*cit_3 == *cit_2)
	//				count += 1;
	//		}*/
	//	}
	//	if (count < min_count) {
	//		min_count = count;
	//		x = *cit_1;
	//	}
	//}
	//return x;
}

GraphSpace::MaxIndependentSet::~MaxIndependentSet()
{
	for (std::vector<NodeSet*>::iterator it = vecNodeSet.begin(); it != vecNodeSet.end(); ++it) 
		delete *it;
	for (std::vector<std::list<int>*>::iterator it = vecMaxIndSets.begin(); it != vecMaxIndSets.end(); ++it) 
		delete *it;
}
