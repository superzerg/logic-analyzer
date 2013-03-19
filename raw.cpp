/*  Copyright (C) 2012,2013 Renier Yves
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
    
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

