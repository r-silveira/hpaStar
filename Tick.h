//----------------------------------------------------------------------------//
// tick.h                                                                     //
// Copyright (C) 2001 Bruno 'Beosil' Heidelberger                             //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef TICK_H
#define TICK_H

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class Tick
{	
	public:
		
		// constructors/destructor
		Tick();
		virtual ~Tick();

	public:

		// miliseconds
		static unsigned int getTick();
};

#endif

//----------------------------------------------------------------------------//
