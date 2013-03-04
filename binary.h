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

class binary : public mglDraw
{
public:
    binary(uint32_t nbit=0);
    binary(const binary &source, uint32_t index_start=0,  uint32_t index_end=0);
    binary(const binary &source, float t_start,  float t_end=0);
    binary(logic_input *data, transition *clk, uint8_t pin, char transition_direction='u');
    ~binary();
    uint32_t Get_nbad(uint32_t index_start, uint32_t index_end=0);
    uint32_t Get(uint32_t index_start, uint32_t index_end=0);
    void sprint(char value[2], uint32_t bit);
    int Draw(mglGraph *gr);
    binary& operator = (const binary source)
    {
        if(this!=&source)
        {
            this->nbit=source.nbit;
            this->npin=source.npin;
            this->t=source.t;
            this->bits=new uint8_t[this->nbit];
            for (uint32_t bit=0;bit<this->nbit;bit++)
            {
                this->bits[bit]=source.bits[bit];
            }
        }
        return *this;
    }
//private:
    uint8_t* bits;
    mglData t;
    uint32_t nbit;
    uint8_t npin;
    uint8_t pin;
};
