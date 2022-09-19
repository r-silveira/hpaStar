#ifndef CLUSTERHANDLER_H
#define CLUSTERHANDLER_H

#include "Grid.h"
#include "Cluster.h"

#include <vector>

class ClusterHandler
{
	public:

		ClusterHandler();
		ClusterHandler(Grid* const grid);
		~ClusterHandler();

		inline int getQuantity() const { return m_quantity; };
		inline void setQuantity(int n) { m_quantity = n; };
		inline int getEpsilon() const { return m_epsilon; };
		inline void setEpsilon(int n) { m_epsilon = n; };
		inline std::list< std::pair<AStarCell*,AStarCell*> >* getEntrances() { return &m_entrances; };
		inline std::vector<Cluster>* getVeClusters() { return &m_vecClusters; };

		void createClusters();
		void createBordersEntrances(int epsilon);
		void computeWhereIsEntrance(std::vector< std::pair< std::pair<int,int>,int> >& possibleItems, int epsilon);
		void addEntranceToList(AStarCell* const cell, int entranceType);
		void update();
		void draw(float offsetX, float offsetY, bool drawCluster);
		void resetClusterInfo();
		void debugCluster(int id);
		void debugClusterNeighbors();

		bool isValidSymmUp(int i, int j);
		bool isValidSymmDown(int i, int j);
		bool isValidSymmLeft(int i, int j);
		bool isValidSymmRight(int i, int j);

			
	private:

		void associateClusterNeighborhood();

		int		m_epsilon;
		Grid*	m_grid;
		int		m_quantity;
		int		m_drawClusterID;
		std::vector<Cluster> m_vecClusters;
		std::list< std::pair<AStarCell*,AStarCell*> > m_entrances;


};


#endif