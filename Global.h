/* -* c++ *- PathPlanning - 2007 Renato Silveira
 *	
 *	global.h
 *
 *
 *				created 27/06/2008
 */

#ifndef GLOBAL_H
#define GLOBAL_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <GL/glui.h>
#include <cmath>
// Already included in GLUI
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

//-----------------------------------------------------------------------------
// Generates a pseudorandom number in [min,max] range.

template<typename real_type>
inline real_type rand_range(const real_type &min, const real_type &max)
{
	assert( min <= max );
	return ((static_cast<real_type>( rand() ) / static_cast<real_type>( RAND_MAX )) * (max - min)) + min;
}


void drawCircle(float originX, float originY, float radius);


#endif

//----------------------------------------------------------------------------//

