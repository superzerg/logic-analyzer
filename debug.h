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

