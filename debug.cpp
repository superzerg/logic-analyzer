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
        
