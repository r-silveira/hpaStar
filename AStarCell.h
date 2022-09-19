#ifndef ASTARCELL_H
#define ASTARCELL_H

#include "Cell.h"

#include <list>


class AStarCell : public Cell
{
	
	public:
		
		static const int NULLPARENT;

		// to be used with multiset (no more :-P)
		class Compare
		{
			public:
                
				virtual ~Compare();

                bool operator()(const AStarCell* cell1,
                                const AStarCell* cell2);
        };

		static const int IN_OPENLIST	= 1;
		static const int IN_CLOSEDLIST	= 0;
		static const bool IS_DIAGONAL	= true;

		AStarCell();
		AStarCell(int id);
		~AStarCell();

		inline int getCluster() const { return m_cluster; };
		inline bool isEntrance() const { return m_entrance; };
		inline int getInListID() const { return m_inListID; }; 
		inline int getID()	const { return m_id; };
		inline int getF() const { return m_f; };
		inline int getG() const { return m_g; };
		inline int getH() const { return m_h; };
		inline int getParent() const { return m_parent; };
		inline std::list<AStarCell*>* getNeighborhoodList() { return &m_neighborhood; };
		inline std::pair<int,int>* getIndex() { return &m_index; };

		inline void setIsEntrance(bool value) { m_entrance = value; };
		inline void setCluster(int value) { m_cluster = value; };
		inline void setInListID(int inListID) { m_inListID = inListID; };
		inline void setID(int id) { m_id = id; };
		inline void setF(int f) { m_f = f; };
		inline void setG(int g) { m_g = g; };
		inline void setH(int h) { m_h = h; };
		inline void setParent(const int parent) { m_parent = parent; };
		void setIndex(int i, int j);
		void setIndex(const std::pair<int,int>& index);
		
		void reset();
		void print() const;

		static bool compare(const AStarCell& cell1,
							const AStarCell& cell2);

		
		//bool operator< (const AStarCell &a) const;


	private:
		
		std::pair<int,int> m_index;
		int m_id;
		int m_inListID;
		int m_f;
		int m_g;
		int m_h;
		int m_parent; // index of the parent node
		int m_cluster;
		bool m_entrance;

		std::list<AStarCell*> m_neighborhood;
};


#endif