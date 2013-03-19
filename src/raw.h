/*  Copyright (C) 2012,2013 Renier Yves
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
    
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>


#ifndef LOGIC_INPUT
#define LOGIC_INPUT
#include "logic_input.h"
#endif

#ifndef TRANSITION
#define TRANSITION
#include "transition.h"
#endif

#ifndef BINARY
#define BINARY
#include "binary.h"
#endif

#ifndef ACTIVITY
#define ACTIVITY
#include "activity.h"
#endif

#ifndef PROTOCOL
#define PROTOCOL
#include "protocol.h"
#endif

#ifndef DEBUG_H
#define DEBUG_H
#include "debug.h"
#endif

//! Default class herited from protocol. No decoding performed
class raw: public protocol
{
public:
    //! Constructor.
    /*! call init_acquisition()*/
    raw(uint8_t pins[]=NULL,uint8_t npin=4);
    
    //! Destructor
    ~raw();
    
    //! no decoding performed.
    void decode ();
    //! Draw all captured data
    /*!     \param gr pointer to mglGraph object*/
    int Draw(mglGraph *gr);
};
