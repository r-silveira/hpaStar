/* -* c++ *- PathPlanning - 2007 Renato Silveira
 *	
 *	Cell.h
 *
 *
 *				created 27/06/2008
 */

#ifndef _CELL_H_
#define _CELL_H_

class Cell
{
	public:

		static const char T_FREE	= 'f';
		static const char T_OBST	= 'o';
		static const char T_GOAL	= 'g';

		//----------------------------------
		Cell();
		~Cell();

		//----------------------------------
		inline char	getType() const { return m_type; };

		//----------------------------------
		inline void setType(char type)	{ m_type = type; };

	private:

		char m_type;
};

#endif

