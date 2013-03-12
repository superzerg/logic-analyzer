#include "raw.h"

raw::raw(uint8_t pins[],uint8_t npin)
{
    pmesg(DEBUG,"raw constructor.\n");
    if(pins!=NULL && npin>0)
        init_acquisition(pins,npin);
}

raw::~raw()
{
    pmesg(DEBUG,"raw destructor.\n");
}

void raw::decode()
{
    pmesg(DEBUG,"raw::decode().\n");
}

int raw::Draw(mglGraph *gr)
{
    pmesg(DEBUG,"raw::Draw().\n");
    float logic_values[]={0, 1, 2, 3};
    const char *ylabels[]={"0V", "HR", "ERR", "3V3"};
    gr->SetTicksVal('y',4,logic_values,ylabels);
    gr->SetPlotFactor(1.15);
    pmesg(DEBUG,"capture.Draw()\n");
    this->capture.Draw(gr);
    return 0;
}

