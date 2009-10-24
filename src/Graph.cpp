#include "Graph.h"

//void GraphSpace::AbstractNode::addAdjacentNode(const AbstractNode *cpAn)
//{
//	lstAdjacentNodes.push_back(cpAn);
//}

bool GraphSpace::AbstractNode::operator==(const AbstractNode &rhs) const
{
	if (this->iNumNode == rhs.iNumNode && this->lstAdjacentNodes == rhs.lstAdjacentNodes)
		return true;
	else
		return false;
}

void GraphSpace::Graph::addNode(AbstractNode *pAn)
{
	mapNodes.insert(std::make_pair(pAn->numNode(), pAn));
}

void GraphSpace::Graph::addEdge(AbstractNode *pAn_1, AbstractNode *pAn_2)
{
	if (*pAn_1 == *pAn_2)
		return;
	
	pAn_1->lstAdjacentNodes.push_back(pAn_2);
	pAn_2->lstAdjacentNodes.push_back(pAn_1);
}

const GraphSpace::AbstractNode* GraphSpace::Graph::node(int numNode) const
{
	std::map<int, AbstractNode*>::const_iterator cIt = mapNodes.find(numNode);
	if (cIt != mapNodes.end())
		return cIt->second;
	else
		return 0;
}

GraphSpace::Graph::~Graph()
{
	for (std::map<int, AbstractNode*>::iterator it = mapNodes.begin(); it != mapNodes.end(); ++it) {
		delete it->second;
		mapNodes.erase(it);
	}
}

//GraphSpace::AbstractNode::~AbstractNode()
//{
//	for (std::list<AbstractNode*>::iterator it = lstAdjacentNodes.begin(); it != lstAdjacentNodes.end(); ++it) {
//		delete *it;
//		lstAdjacentNodes.erase(it);
//	}
//}