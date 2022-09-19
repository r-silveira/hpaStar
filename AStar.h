#ifndef ASTAR_H
#define ASTAR_H

#include "AStarOpenList.h"
#include "Grid.h"
#include "Graph.h"

class AStar
{
	public:

		AStar();
		AStar(Grid* const grid);
		~AStar();

		void setStartPosition(int i, int j);
		void setStartPosition(const std::pair<int,int>& p);
		void setEndPosition(int i, int j);
		void setEndPosition(const std::pair<int,int>& p);

		inline std::pair<int,int>& getStartPosition() { return m_startPosition; };
		inline std::pair<int,int>& getEndPosition() { return m_endPosition; };
		inline int getPathCost() const { return m_grid->getCell(m_endPosition)->getG(); };
		std::list<AStarCell*> getPath();

		bool run(int si, int sj, int ei, int ej);
		bool runInGraph(int nodeIDStart, int nodeIDEnd, Graph* graph);
		void reset();
		void clearPath();
		void update();
		void draw(float offsetX, float offsetY);
		void drawPath(float offsetX, float offsetY);

	private:

		// using Manhattan Distance
		void updateIndex();
		int goalDistEstimate(AStarCell* const cell);
		AStarCell* getBestCellFromOpenList();
		bool isGoal(AStarCell* cell);
		bool checkNeighborExistence(int i, int j);
		void processAllNeighborhood();
		bool isValid(const AStarCell* cell);
		int isDiagonal(AStarCell* const cell, AStarCell* const parent);
		int computeG(int parent, AStarCell* const cell);
		int computeGInGraph(int parent, std::pair<Node,int> const cell);
		void recoveryPath();
			
		void debug_neighborhood();
		void debug_openList();

		std::pair<int,int> m_startPosition;
		std::pair<int,int> m_endPosition;

		AStarOpenList*			m_openList;
		Grid*					m_grid;
		std::list<AStarCell*>	m_path;
};

#endif