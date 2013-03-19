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

#ifndef DEBUG_H
#define DEBUG_H
#include "debug.h"
#endif

#ifndef LOGIC_INPUT
#define LOGIC_INPUT
#include "logic_input.h"
#endif

//! Class containing activity obtained from logic_input
/*! Class containing time range when ship is active and able to draw the background of  subplots accordingly*/

class activity: public mglDraw
{
public:
    //! Constructor from a logic_input object data
    /*! Calls init if data is not NULL*/
    activity(logic_input *data=NULL,uint8_t pin=0, logic_state=HIGHV);
    /*!     \param data logic_input object
            \param pin pin index corresponding to Chip Select 
            \param active_value value when the Chip is Enabled (HIGHV or LOWV)*/
    void init(logic_input *data,uint8_t pin=0, logic_state=HIGHV);
    //!Destructor
    ~activity();
    //! Paint background of a subplot according to activity
    /*!     \param gr mglGraph object
            \param subplot index of the subplot*/
    int Draw(mglGraph *gr,uint8_t subplot);
//private:
    //! Number of active intervals
    uint32_t nactive;
    //! array of starting time of each active period
    float *t_start;
    //! array of ending time of each active period
    float *t_end;
    //! number of pin acquired
    /*! needed to know subplot number*/
    uint8_t npin;
};
