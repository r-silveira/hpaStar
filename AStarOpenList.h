#ifndef ASTAROPENLIST_H
#define ASTAROPENLIST_H

#include "AStarCell.h"
#include <list>

class AStarOpenList
{
	public:
		
		AStarOpenList();
		~AStarOpenList();

		void init();
		void insert(AStarCell* cell);
		AStarCell* pop();

		inline bool isEmpty() const { return openList.empty(); };
		inline int getSize() const { return openList.size(); };

		void clear();
		void print() const;

	private:

		std::list<AStarCell*> openList;
};

#endif
