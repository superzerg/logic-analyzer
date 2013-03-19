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
    
#include "binary.h"


binary::binary(uint32_t nbit)
{
    pmesg(DEBUG,"binary destructor.\n");
    this->nbit=nbit;
    if (nbit>0)
        this->bits=new uint8_t[nbit];
    else
        this->bits=NULL;
    this->nbit=0;
    this->npin=0;
    this->pin=0;
}

binary::binary(const binary &source, uint32_t index_start,  uint32_t index_end)
{
    if(index_end==0)
        index_end=source.nbit-1;
    pmesg(DEBUG,"binary copy constructor, i_start=%i i_end=%i.\n",index_start,index_end);
    this->npin=0;
    this->pin=0;
    this->bits=NULL;
    if(index_end-index_start+1<1)
    {
        pmesg(ERROR,"ERROR in binary copy constructor, empty range.\n");
        return;
    }
    this->nbit=index_end-index_start+1;
    this->bits=new uint8_t[this->nbit];
    this->t.Create(this->nbit);
    for (uint32_t bit=0;bit<this->nbit;bit++)
    {
        this->bits[bit]=source.bits[bit+index_start];
        this->t.a[bit]=source.t.a[bit+index_start];
    }
    this->npin=source.npin;
    this->pin=source.pin;        
}

binary::binary(const binary &source, float t_start,  float t_end)
{
    if(t_end==0)
        t_end=source.t.a[nbit-1];
    pmesg(DEBUG,"binary copy constructor, t_start=%f t_end=%f.\n",t_start,t_end);
    this->npin=0;
    this->pin=0;
    this->bits=NULL;
    if(t_end-t_start<0)
    {
        pmesg(ERROR,"ERROR in binary copy constructor, empty range\n");
        return;
    }
    //find the number of bits between t_start and t_end
    this->nbit=0;
    uint32_t index_start=0;
    uint32_t index_end=0;
    for(uint32_t i=0;i<source.nbit;i++)
    {
        if((source.t.a[i]>=t_start) && (this->nbit==0))
            index_start=i;
        if((source.t.a[i]>=t_start) && (source.t.a[i]<=t_end))
            this->nbit++;
        if(i>1 && source.t.a[i]>t_end)
        {
            index_end=i-1;
            break;
        }
    }
    if (index_end==0)
        index_end=source.nbit-1;
        pmesg(DEBUG,"binary copy constructor, this->nbit=%i or %i, index_end=%i (t=%fs), index_start=%i(t=%fs)\n",this->nbit,index_end-index_start+1,index_end,source.t.a[index_end],index_start,source.t.a[index_start]);
    this->bits=new uint8_t[this->nbit];
    this->t.Create(this->nbit);
    for (uint32_t bit=0;bit<this->nbit;bit++)
    {
        this->bits[bit]=source.bits[bit+index_start];
        this->t.a[bit]=source.t.a[bit+index_start];
    }
    this->npin=source.npin;
    this->pin=source.pin;        
}

binary::~binary()
{
    pmesg(DEBUG,"binary destructor\n");
    if (this->bits!=NULL)
        delete[] this->bits;
    this->bits=NULL;
    this->nbit=0;
    this->npin=0;
    this->pin=0;
}

void binary::init(logic_input *data, transition *clk, uint8_t pin, char transition_direction)
{
    pmesg(DEBUG,"binary::init().\n");
    if(data==NULL)
    {
        pmesg(CRITICAL,"ERROR in binary::init(): data is NULL\n");
        return;
    }
    uint32_t nbit;
    uint32_t *index_transition;
    mglData *t_transition;
    if (pin>=data->npin)
    {
        pmesg(CRITICAL,"ERROR in binar::init(), pin %i not valid\n",pin);
        return;
    }
    this->npin=data->npin;
    this->pin=pin;
   
    switch (transition_direction)
    {
        case 'u':
            nbit=clk->ntransition_up;
            index_transition=clk->index_transition_up;
            t_transition=&(clk->t_transition_up);
            break;
        case 'd':
            nbit=clk->ntransition_down;
            index_transition=clk->index_transition_down;
            t_transition=&(clk->t_transition_down);
            break;
         default:
            pmesg(CRITICAL,"ERROR in binary::init(), transition_direction must be \'u\' or \'d\'\n");
            return;
    }
    pmesg(DEBUG,"allocate memory for  %i bits\n",nbit);
    this->nbit=nbit;
    this->bits=new uint8_t[this->nbit];
    pmesg(DEBUG,"create t\n");
    this->t.Create(this->nbit);
    pmesg(DEBUG,"fill bits and t\n");
    for (uint32_t i=0;i<this->nbit;i++)
    {
        this->bits[i]=data->rawdata[pin].a[index_transition[i]];
        this->t.a[i]=t_transition->a[i];
        pmesg(DEBUG,"read :%i at %fs\n",this->bits[i],this->t.a[i]);
    }
} 
   
uint32_t binary::Get_nbad(uint32_t index_start, uint32_t index_end)
{
    if(index_end==0)
        index_end=index_start;
    if(index_start>=this->nbit || index_end>=this->nbit)
    {
        pmesg(WARNING,"ERROR in binary::Get_nbad(%i,%i): out of ranges index(es)\n",index_start,index_end);
        return 0;
    }
    uint32_t nbad=0;
    for (uint32_t bit=index_start;bit<=index_end;bit++)
    {
        if(this->bits[bit]!=3 && this->bits[bit]!=0)
            nbad++;
    }
    return nbad;
}

uint32_t binary::Get(uint32_t index_start, uint32_t index_end)
{
    pmesg(DEBUG,"binary::Get().\n");
    if(index_end==0)
        index_end=index_start;
    if(index_start>=this->nbit || index_end>=this->nbit)
    {
        pmesg(WARNING,"ERROR in binary::Get(%i,%i): out of ranges index(es)\n",index_start,index_end);
        return 0;
    }
    uint32_t value=0;
    uint32_t lshift;
    for (uint32_t bit=index_start;bit<=index_end;bit++)
    {
        lshift=index_end-bit;
        pmesg(DEBUG,"at index %i/%i, bit=%i<<%i, counting for %i\n",bit,this->nbit,this->bits[bit],lshift,((uint32_t)(this->bits[bit]/3))<<lshift);
        value=value+(((uint32_t)(this->bits[bit]/3))<<lshift);
    }
    return value;
}

void binary::sprint(char value[2], uint32_t bit)
{
    if(bit>this->nbit-1)
    {
        pmesg(ERROR,"ERROR in binary::sprint(): bit=%i>nbit-1\n",bit);
        strcpy(value,"");
        return;
    }
    switch (this->bits[bit])
    {
        case 1:
            strcpy(value," ");
            break;
        case 0:
            strcpy(value,"0");
            break;
        case 3:
            strcpy(value,"1");
            break;
        case 2:
            strcpy(value,"E");
            break;
        default:
            strcpy(value,"?");
            break;
    }
}

int binary::Draw(mglGraph *gr)
{
    pmesg(DEBUG,"binary::Draw().\n");
    char c[2];
    gr->SubPlot(1,this->npin,this->pin);
    for(uint32_t i=0;i<this->nbit;i++)
    {
        this->sprint(c,i);
        pmesg(DEBUG,"subplot %i: add label \"%s\" at t=%fs\n",this->pin,c,this->t.a[i]);
        gr->Puts(mglPoint(this->t.a[i],1.5,0.5),c);
    }
    return 0;
}


    
