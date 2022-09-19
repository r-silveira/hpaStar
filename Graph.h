#ifndef GRAPH_H
#define GRAPH_H

#include "Grid.h"

#include <list>
#include <vector>

typedef AStarCell* Node;

class Graph
{
	public:


		Graph();
		Graph(Grid* const grid);
		~Graph();

		inline void setGrid(Grid* const grid)	{ m_grid = grid; };

		inline std::vector<Node>* getVecNodes() { return &m_nodes; };
		std::vector< std::list<std::pair<Node, int> > >* getGraph() { return &m_graph; };
		int getNodeIDCorrespondent(AStarCell* const cell);

		void addNodeList(const std::list<std::pair<AStarCell*,AStarCell*> >& entrances);
		void updateNodeList(const std::list<std::pair<AStarCell*,AStarCell*> >& entrances);
		void addEdge(const std::pair<AStarCell*,AStarCell*>& edge, int cost);
		bool removeEquals();
		void removeNodesEquals();
		int howManyNodesEquals(AStarCell* const c);
		void draw(float offsetX, float offsetY, bool drawGraph);
		void drawEmphatize(	float offsetX, 
							float offsetY, 
							bool drawGraph, 
							const std::list<Node>& pathInGraph);
		void print() const;
		

	private:

		// (node, cost)
		std::vector< std::list<std::pair<Node, int> > > m_graph;
		std::vector<Node> m_nodes;
		Grid* m_grid;
		
};


#endif