/* -* c++ *- PathPlanning - 2007 Renato Silveira
 *	
 *	PickHandler.h
 *
 *
 *				created 07/11/2007
 */


#ifndef PICKHANDLER_H
#define PICKHANDLER_H

#include "Vec3f.h" 
#include "Grid.h"

class PickHandler
{
	public:
	
	PickHandler();
	~PickHandler();
	
	void init(Grid* g);
	void applyCamTransform();
	void drawPlaneDepthOnly(float xi, float xf, float yi, float yf);
	Vec3f getWorldSpace(int x, int y);

	private:

		Grid* grid;
};

#endif