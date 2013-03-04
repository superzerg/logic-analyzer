//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>

#ifndef BINARY
#define BINARY
#include "binary.h"
#endif

class message: public mglDraw
{
public:
    message(const binary *data=NULL,const char label[30]="",float t0=0,float tmessage=0,char color='w');
    void init(const binary *data,const char label[30]="",float t0=0,float tmessage=0,char color='w');
    int Draw(mglGraph *gr);
//private:
    binary data;
    uint8_t pin;
    uint8_t npin;
    char label[30];
    float t0;
    float tmessage;
    char color;
};
