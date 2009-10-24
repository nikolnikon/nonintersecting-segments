#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>

namespace GraphSpace
{
	class AbstractNode;
	class Graph
	{
	public:
		Graph();
		~Graph();

		void addNode(AbstractNode *pAn);
		void addEdge(AbstractNode *pAn_1, AbstractNode *pAn_2);

		const AbstractNode* node(int numNode) const;
	private:
		int iEdgeCount;
		std::map<int, AbstractNode*> mapNodes;		
	};

	class AbstractNode
	{
	public:
		friend void Graph::addEdge(AbstractNode *pAn_1, AbstractNode *pAn_2);

		AbstractNode(int numNode) : iNumNode(numNode) {}
		
		int numNode() const { return iNumNode; }

		bool operator==(const AbstractNode &rhs) const;
	
	private:
		//void addAdjacentNode(const AbstractNode *cpAn); // private позволяет осуществить только парное добаление смежных вершин
		
		int iNumNode;
		std::list<const AbstractNode*> lstAdjacentNodes;
	};

	/*class NodeContainer
	{
	public:
		NodeContainer() {}
		~NodeContainer();
	private:
		std::map<int, AbstractNode*> mapNodes;
	};*/
}
#endif
