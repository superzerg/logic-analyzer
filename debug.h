#include <stdarg.h>

typedef enum
{
    CRITICAL = 0,
    ERROR = 1,
    WARNING = 2,
    INFORMATION = 3,
    DEBUG = 4,
}debug_level;


#if defined(NDEBUG) && defined(__GNUC__)
/* gcc's cpp has extensions; it allows for macros with a variable number of
   arguments. We use this extension here to preprocess pmesg away. */
#define pmesg(level, format, args...) ((void)0)
#else
void pmesg(debug_level level, const char format[], ...);
/* print a message, if it is considered significant enough.
      Adapted from [K&R2], p. 174 */
#endif

