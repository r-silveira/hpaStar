#include "Global.h"

void drawCircle(float originX, float originY, float radius)
{
	float vectorX1 = originX; 
	float vectorY1 = originY;

	glBegin(GL_TRIANGLES);	

		for(int i = 0; i <= 360; ++i)
		{
			const float angle = (float)(((double)i)/57.29577957795135);	
			const float vectorX = originX + (radius * (float)sin((double)angle));
			const float vectorY = originY + (radius * (float)cos((double)angle));		

			glVertex2d(originX, originY);
			glVertex2d(vectorX, vectorY);
			glVertex2d(vectorX1, vectorY1);

			vectorY1 = vectorY;
			vectorX1 = vectorX;	
		}

	glEnd();
}