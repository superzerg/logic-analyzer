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
