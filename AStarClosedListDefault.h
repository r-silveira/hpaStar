#ifndef ASTARCLOSEDLISTDEFAULT_H
#define ASTARCLOSEDLISTDEFAULT_H

#include "AStarClosedlist.h"
#include "AStarCell.h"

#include <list>

// Implementation using a list
class AStarClosedListDefault : public AStarClosedList
{
	 public:

		 ~AStarClosedListDefault();
		 void init(int maxNodeId = 0);
         void add(AStarCell* ptCell);
         void remove(int nodeId);
		 void print() const;

        private:
		
		std::list<AStarCell*> m_list;      
};


#endif