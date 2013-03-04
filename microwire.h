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

#ifndef MESSAGE
#define MESSAGE
#include "message.h"
#endif

#ifndef ACTIVITY
#define ACTIVITY
#include "activity.h"
#endif

class microwire: public mglDraw
{
public:
    microwire(uint8_t pins[4],uint8_t pin=4);
    ~microwire();
    void decode_mosi();
    void decode_miso();
    int Draw(mglGraph *gr);
//private:  
    logic_input capture;
    activity cs;
    transition clk;
    binary mosi;
    binary miso;
    message *mosi_mess;
    message *miso_mess;
    uint32_t nmessage;
};
