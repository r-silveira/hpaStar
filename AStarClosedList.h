#ifndef ASTARCLOSEDLIST_H
#define ASTARCLOSEDLIST_H

class AStarCell;

class AStarClosedList
{
	public:

		virtual ~AStarClosedList() {};
		// Initialize for search. Must be called before using the closed list.
		virtual void init(int maxNodeId = 0) = 0;
		virtual void add(AStarCell* ptCell) = 0;
		virtual void remove(int nodeId) = 0;
};

#endif
