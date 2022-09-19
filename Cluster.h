#ifndef CLUSTER_H
#define CLUSTER_H

#include "Grid.h"

class Cluster
{
	public:

		Cluster();
		Cluster(Grid* const grid);
		~Cluster();

		inline void setGrid(Grid* const grid)	{ m_grid = grid; };
		inline void setUpCluster(int up)		{ m_upClusterID = up;};
		inline void setDownCluster(int down)	{ m_downClusterID = down;};
		inline void setLeftCluster(int left)	{ m_leftClusterID = left;};
		inline void setRightCluster(int right)	{ m_rightClusterID = right;};
		inline void setDrawCluster(int value) { m_drawCluster = value; };

		inline std::pair<int,int> getStartIndex() const { return m_start; };
		inline int getID() const { return m_id; };
		inline int getSize() const { return m_size; };
		inline int getUpCluster() const { return m_upClusterID; };
		inline int getDownCluster() const { return m_downClusterID; };
		inline int getLeftCluster() const { return m_leftClusterID; };
		inline int getRightCluster() const { return m_rightClusterID; };
		inline int getDrawCluster() const { return m_drawCluster; };

		inline AStarCell* getCell(int i, int j) { return m_grid->getCell(i,j); };

		void symmUp();
		void symmDown();
		void symmLeft();
		void symmRight();

		void markBorders();
		void update();
		void draw(float offsetX, float offsetY);
		void createCluster(int id, std::pair<int,int> start, int size);
		void associateCellsToClusters();
		void printNeighbors() const;
		void createBordersEntrances();


	private:

		Grid*	m_grid;
		int m_id;
		std::pair<int,int> m_start;
		int m_size;
		bool m_drawCluster;

		// neighbor pointer: -1 == null
		int m_upClusterID;
		int m_downClusterID;
		int m_leftClusterID;
		int m_rightClusterID;


};

#endif 