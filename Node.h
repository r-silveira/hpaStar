#ifndef NODE_H
#define NODE_H

#include "AStarCell.h"

class Node : public AStarCell
{
	public:

		Node();
		Node(int id);
		Node(AStarCell* const cell);
		~Node();

	private:

		int m_id;
};

#endif