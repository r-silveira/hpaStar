#ifndef	HIERARCHICALHANDLER_H
#define HIERARCHICALHANDLER_H

#include "Grid.h"
#include "ClusterHandler.h"
#include "Graph.h"
#include "AStar.h"

class HierarchicalHandler
{
	public:

		HierarchicalHandler();
		HierarchicalHandler(Grid* const grid, ClusterHandler* const clusterHandler, AStar* const aStar);
		~HierarchicalHandler();

		inline void setGrid(Grid* const grid)	{ m_grid = grid; };
		inline void setClusterHandler(ClusterHandler* const ch)	{ m_clusterHandler = ch; };
		inline void setGraph(Graph* const graph)	{ m_graph = graph; };

		void addStartAndEndNode(std::list< std::pair<AStarCell*,AStarCell*> >* entrances);
		void createGraphFromClustersEntrances(const std::list< std::pair<AStarCell*,AStarCell*> >& entrances);
		void createGraphFromClustersEntrances();
		void groupNodesPerClusters();
		void createInterEdges();
		void createIntraEdges();
		void computeLowLevelPath();
		void update();
		void updateChangingStartAndEnd();
		//void firstStep();
		//void secondStep();
		//void thirdStep();
		void processAStarInGraph();

		void draw(float offsetX, float offsetY, bool drawGraph);
		void drawFinalPath(float offsetX, float offsetY);

	private:
	
		Graph* m_graph;
		ClusterHandler* m_clusterHandler;
		Grid* m_grid;
		AStar* m_aStar;

		// each cluster has nodes insides
		std::list< std::pair<AStarCell*,AStarCell*> > m_entrances;
		std::vector< std::list<Node*> > m_nodesInCluster;
		std::list< Node > m_pathInGraph;
		std::list< Node > m_finalPath;
};

#endif