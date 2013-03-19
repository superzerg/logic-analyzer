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
    
#include "message.h"


message::message(const binary *data,const char label[30],float t0,float tmessage,char color)
{
    pmesg(DEBUG,"message constructor\n");
    this->t0=0;
    this->tmessage=0;
    this->color='w';
    this->pin=0;
    this->npin=0;
    strcpy(this->label,"");
    if(data!=NULL)
        init(data,label,t0,tmessage,color);
}

void message::init(const binary *data,const char label[30],float t0,float tmessage,char color)
{
    pmesg(DEBUG,"message::init()\n");
    if (data==NULL || data->nbit==0)
    {
        pmesg(CRITICAL,"ERROR in messag::init(): data is empty\n");
        return;
    }
    this->data=*data;
    this->pin=data->pin;
    this->npin=data->npin;
    strcpy(this->label,label);
    if (t0==0)
        this->t0=data->t.a[0];
    else
        this->t0=t0;
    if (tmessage==0)
        this->tmessage=data->t.a[data->nbit-1]-this->t0;
    else
        this->tmessage=tmessage;
    this->color=color;
}

int message::Draw(mglGraph *gr)
{
    pmesg(DEBUG,"message::Draw()\n");
    if(this->tmessage==0)
        return 1;
    gr->SubPlot(1,this->npin,this->pin);
    gr->FaceZ(this->t0,0.0,-1,this->tmessage,3.0,"y9");
    gr->Puts(mglPoint(this->t0+this->tmessage/2,2.25,1.0),this->label);
    return 0;
}



