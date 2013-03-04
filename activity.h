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


class activity: public mglDraw
{
public:
    activity(logic_input &data,uint8_t pin, float active_value=1);
    int Draw(mglGraph *gr,uint8_t subplot);
//private:    
    uint32_t nactive;
    float *t_start;
    float *t_end;
    uint8_t npin;
};
