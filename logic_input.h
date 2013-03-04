#include <time.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <bcm2835.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>

typedef enum
{
    LOWV = 0,
    HIGHR = 1,
    ERROR = 2,
    HIGHV = 3,
}logic_state;

class logic_input : public mglDraw
{
public:
    logic_input(uint8_t npin=0,uint8_t * pins=NULL,uint32_t npoint=0,float delay=0);
    ~logic_input();
    void init_acquisition(uint8_t * pins, uint8_t npin=0);
    void acquire(uint32_t npoint=0,float delay=0);
    int Draw(mglGraph *gr,const char *label[4]=NULL);
//private:
    uint32_t npoint;
    uint8_t npin;
    uint8_t *pins;
    mglData *rawdata;
    mglData t;
};

