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
    
// Adapted from oopweb.com/CPP/Documents/DebugCPP/Volume/techniques.html

#include "debug.h"
#include <stdio.h>

extern debug_level msglevel;

#if defined(NDEBUG) && defined(__GNUC__)
/* Nothing. pmesg has been "defined away" in debug.h already. */
#else
void pmesg(debug_level level, const char format[], ...) {
#ifdef NDEBUG
	/* Empty body, so a good compiler will optimise calls
	   to pmesg away */
#else
        va_list args;

        if (level>msglevel)
                return;

        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
#endif /* NDEBUG */
#endif /* NDEBUG && __GNUC__ */
}
        
