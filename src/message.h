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

#ifndef BINARY
#define BINARY
#include "binary.h"
#endif

#ifndef DEBUG_H
#define DEBUG_H
#include "debug.h"
#endif

//! Class to handle decoded information
class message: public mglDraw
{
public:
    //! Constructor
    /*! call init() if data is not NULL
        \sa init()*/
    message(const binary *data=NULL,const char label[30]="",float t0=0,float tmessage=0,char color='w');
    //! Initialize the message from parameters: 
    /*!     \param data object representing the message (subset of acquired binary data).
            \param label string representing the message (Drawn string).
            \param t0 acquisition time in seconds of the beginning of the message.
            \param tmessage duration of the message in seconds.
            \param color char giving the color of the message.
        this->pin and this->npin are taken from data
        \sa binary*/
    void init(const binary *data,const char label[30]="",float t0=0,float tmessage=0,char color='w');
    //! Draw the message
    /*!     \param gr pointer to a mglGraph object
        The subplot on which the message belongs to is taken from this->pin*/
    int Draw(mglGraph *gr);
//private:
    //! object representing the message (subset of acquired binary data).
    binary data;
    //! subplot of the message
    uint8_t pin;
    //! total number of subplot
    uint8_t npin;
    //! string representing the message (Drawn string).
    char label[30];
    //! acquisition time when message started.
    float t0;
    //! duration of the message.
    float tmessage;
    //! color of the message.
    char color;
};
