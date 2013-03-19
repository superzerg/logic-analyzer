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
    
#include <stdarg.h>

//! The different debug level available are: 
typedef enum
{
    CRITICAL = 0,//!< Program will exit
    ERROR = 1,//!< Data corrupted
    WARNING = 2,//!< Program can continue but this should not happen
    INFORMATION = 3,//!< Normal output
    DEBUG = 4,//!< Information for programmers
}debug_level;

extern debug_level msglevel; /* the higher, the more messages... */

#if defined(NDEBUG) && defined(__GNUC__)
/* gcc's cpp has extensions; it allows for macros with a variable number of
   arguments. We use this extension here to preprocess pmesg away. */
#define pmesg(level, format, args...) ((void)0)
#else
//! Function printing execution information if level is lower than the extern variable msglevel
/*! This function should be used as printf adding level as first parameter 
        \param level determine if the message is printed to stderr
        \param format printf() format argument, followed by the usuals arguments
    \sa debug_level*/
void pmesg(debug_level level, const char format[], ...);
/* print a message, if it is considered significant enough.
      Adapted from oopweb.com/CPP/Documents/DebugCPP/Volume/techniques.html*/
#endif

