/* -* c++ *- PathPlanning - 2007 Renato Silveira
 *	
 *	Grid.h
 *
 *
 *				created 07/11/2007
 */

#ifndef GRID_H
#define GRID_H

#include "AStarCell.h"
#include "Vec2f.h"
#include <vector>

class Grid
{
	public:

		Grid();
		~Grid();

		
		inline bool		showFunctions() const { return show_functions; };
		inline int		getNCellsHor()	const { return nCellsHor; };
		inline int		getNCellsVer()	const { return nCellsVer; };
		inline float	getCellLengthX()	const { return cellLengthX; };
		inline float 	getCellLengthY()	const { return cellLengthY; };
		inline float	getResolution() const { return res; };
		inline float	getDistX()		const { return distX; };
		inline float	getDistY()		const { return distY; };
		inline float	getXMin()		const { return xMinWorld; };
		inline float	getXMax()		const { return xMaxWorld; };
		inline float	getYMin()		const { return yMinWorld; };
		inline float	getYMax()		const { return yMaxWorld; };
		inline AStarCell* getCell(int i, int j) { return &grid[j*nCellsHor+i]; }; 
		inline AStarCell* getCell(int i) { return &grid[i]; }; 
		inline AStarCell* getCell(const std::pair<int,int>& cellIndex) 
		{ 
			return &grid[cellIndex.second*nCellsHor + cellIndex.first]; 
		};

		inline void setShowFunctions(bool s)	{ show_functions = s; };
		inline void setXMin(int n)			{ xMinWorld = n; };
		inline void setYMin(int n)			{ yMinWorld = n; };
		inline void setXMax(int n)			{ xMaxWorld = n; };
		inline void setYMax(int n)			{ yMaxWorld = n; };
		inline void setNCellsHor(int n)		{ nCellsHor = n; };
		inline void setNCellsVer(int n)		{ nCellsVer = n; };
		inline void setResolution(float r)	{ res = r; };
		

		void init(float _xMin, float _xMax, float _yMin, float _yMax, float r);
		void resize(int _size, float _res);
		void makeEmptyGrid();
		void reset();
		void fullReset();
		void draw(float offsetX, float offsetY, bool cellContour);
		void mapAsObstacleCell(int i, int j);
		void mapAsGoalCell(int i, int j);
		void mapAsFreeCell(int i, int j);

	private:

		int		nCellsHor;
		int		nCellsVer;
		int		relaxIter;
		float	relaxError;
		float	cellLengthX;
		float	cellLengthY;
		float	res;
		float	xMinWorld;
		float	xMaxWorld;
		float	yMinWorld;
		float	yMaxWorld;
		float	distX;
		float	distY;
		bool	show_functions;

		std::vector<AStarCell> grid;

		void renderString(char *string, const float color[], void *font);

};


#endif