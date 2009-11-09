#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>
#include <stack>
#include <vector>

namespace GraphSpace
{
	class BaseNode;
  class BaseEdge;
  //class Intersections;
	
  class Graph
	{
	public:
    Graph() : iNodeCount(0), iEdgeCount(0) {} 
		~Graph();

		void addNode(BaseNode *pAn);
		void addEdge(BaseNode *pAn_1, BaseNode *pAn_2);
    void removeNode(BaseNode *pAn);
    void removeEdge(BaseNode *pAn_1, BaseNode *pAn_2);

		const BaseNode* node(int numNode) const;
		int nodeCount() const {return iNodeCount; };
		//BaseNode* node(int numNode) const;
    //const BaseNode* incidentNodes(const BaseNode *cpAn_1, const BaseNode *cpAn_2);
    void incidentEdges(const BaseNode *cpAn, std::list<const BaseNode*> &rIncEdges);
		void print() const;
	private:
    int iNodeCount;
		int iEdgeCount;
		std::map<int, BaseNode*> mapNodes;	
    std::map<int, BaseEdge*> mapEdges;
	};

	class BaseNode
	{
	public:
		friend void Graph::addEdge(BaseNode *pAn_1, BaseNode *pAn_2);
    friend void Graph::removeEdge(BaseNode *pBn_1, BaseNode *pBn_2);

		BaseNode(int numNode) : iNumNode(numNode) {}
    virtual ~BaseNode() {}
		
		int numNode() const { return iNumNode; }
		void adjacentNodes(std::list<int> &rResult) const;

		bool operator==(const BaseNode &rhs) const;
		void printAdjNodes() const;
	
	private:
		//void addAdjacentNode(const BaseNode *cpAn); // private позволяет осуществить только парное добаление смежных вершин
		
		int iNumNode;
		std::list<int> lstAdjacentNodes;
	};
	
	//bool operator<(const BaseEdge &lhs, const BaseEdge &rhs) const;

  class BaseEdge
  {
  public:
    BaseEdge(const BaseNode *cpAn_1, const BaseNode *cpAn_2);
    virtual ~BaseEdge() {}

  private:
    int iNumEdge;
    const BaseNode *fstNode;
    const BaseNode *sndNode;
  };

	class MaxIndependentSet
	{
	public:
		MaxIndependentSet() : iMaxMaxIndSet(0) {}
		~MaxIndependentSet();
		void findMaxIndependentSet(const Graph &rGr);
	private:
		// сожержит множества Qk- и Qk+ для каждого шага k
		struct NodeSet
		{
			NodeSet() {}
			NodeSet(const std::list<int> &list_1, const std::list<int> &list_2) : NotCandidateNodes(list_1), CandidateNodes(list_2) {} 
			std::list<int> NotCandidateNodes; // содержит множество вершин Qk-
			std::list<int> CandidateNodes; // содержит множество вершин Qk+
		};
		bool returnCondition(int k, const Graph &crGr, std::list<int> &adjNodes) const;
		int candidate(int k, const Graph &crGr, std::list<int> &adjNodes) const;
		std::list<int> lstIndSet; // используется в качестве стека
		std::vector<std::list<int>*> vecMaxIndSets;
		int iMaxMaxIndSet;
		//std::list<int> lstMaxMaxIndSet;
		std::vector<NodeSet*> vecNodeSet;
	};
}
#endif
