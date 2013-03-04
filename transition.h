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

#ifndef ACTIVITY
#define ACTIVITY
#include "activity.h"
#endif

class transition: public mglDraw  //clock transitions
{
public:
    transition(logic_input *data=NULL,uint8_t pin_clock=1,activity *cs=NULL);
    void init(logic_input *data,uint8_t pin_clock,activity *cs=NULL);
    ~transition();
    float GetStats(activity *cs,char sign='u');
    void find_transition(logic_input *data,char sign='u');
    int Draw(mglGraph *gr);
    int Draw(mglGraph *gr,char sign,uint8_t subplot);
//private:
    uint32_t ntransition_up;
    uint32_t *index_transition_up;
    mglData t_transition_up;
    uint32_t ntransition_down;
    uint32_t *index_transition_down;
    mglData t_transition_down;
    uint8_t npin;
    uint8_t pin_clock;
    float period;
    float period_min;
    float period_max;
    float t_first_activity;
};
