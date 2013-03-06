#include "binary.h"


binary::binary(uint32_t nbit)
{
    this->nbit=nbit;
    if (nbit>0)
        this->bits=new uint8_t[nbit];
    else
        this->bits=NULL;
    this->t.Create(nbit);
    this->npin=0;
    this->pin=0;
}

binary::binary(const binary &source, uint32_t index_start,  uint32_t index_end)
{
    if(index_end==0)
        index_end=source.nbit-1;
    this->npin=0;
    this->pin=0;
    this->bits=NULL;
    if(index_end-index_start+1<1)
    {
        printf("ERROR in binary copy constructor, empty range\n");
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
//printf("binary copy constructor, t_start=%f t_end=%f\n",t_start,t_end);
    this->npin=0;
    this->pin=0;
    this->bits=NULL;
    if(t_end-t_start<0)
    {
        printf("ERROR in binary copy constructor, empty range\n");
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
//printf("binary copy constructor, this->nbit=%i or %i, index_end=%i (t=%fs), index_start=%i(t=%fs)\n",this->nbit,index_end-index_start+1,index_end,source.t.a[index_end],index_start,source.t.a[index_start]);
//        this->nbit=index_end-index_start+1;
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
    if (this->bits!=NULL)
        delete[] this->bits;
    this->bits=NULL;
    this->nbit=0;
    this->npin=0;
    this->pin=0;
}

void binary::init(logic_input *data, transition *clk, uint8_t pin, char transition_direction)
{
    this->~binary();
    uint32_t nbit;
    uint32_t *index_transition;
    mglData *t_transition;
    if (pin>=data->npin)
    {
        printf("ERROR in binary initialization, pin %i not valid\n",pin);
        return;
    }
    this->npin=data->npin;
    this->pin=pin;
   
    switch (transition_direction)
    {
        case 'u':
            nbit=clk->ntransition_up;
//To check : index_transition[i]==clk->index_transition_up[i]
            index_transition=clk->index_transition_up;
//same for t                
            t_transition=&(clk->t_transition_up);
            break;
        case 'd':
            nbit=clk->ntransition_down;
//To check : index_transition[i]==clk->index_transition_down[i]
            index_transition=clk->index_transition_down;
//same for t                
            t_transition=&(clk->t_transition_down);
            break;
         default:
            printf("ERROR in binary initialization, transition_direction must be \'u\' or \'d\'\n");
            return;
    }
    this->nbit=nbit;
    this->bits=new uint8_t[nbit];
    this->t.Create(nbit);
    for (uint32_t i=0;i<this->nbit;i++)
    {
        this->bits[i]=data->rawdata[pin].a[index_transition[i]];
        this->t.a[i]=t_transition->a[i];
//printf("read :%i at %fs\n",this->bits[i],this->t.a[i]);
    }
} 
   
uint32_t binary::Get_nbad(uint32_t index_start, uint32_t index_end)
{
    if(index_end==0)
        index_end=index_start;
    if(index_start>=this->nbit || index_end>=this->nbit)
    {
        printf("ERROR in binary::Get(%i,%i): out of ranges index(es)\n",index_start,index_end);
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
    if(index_end==0)
        index_end=index_start;
    if(index_start>=this->nbit || index_end>=this->nbit)
    {
        printf("ERROR in binary::Get(%i,%i): out of ranges index(es)\n",index_start,index_end);
        return 0;
    }
    uint32_t value=0;
    uint32_t lshift;
    for (uint32_t bit=index_start;bit<=index_end;bit++)
    {
        lshift=index_end-bit;
//printf("at index %i/%i, bit=%i<<%i, counting for %i\n",bit,this->nbit,this->bits[bit],lshift,((uint32_t)(this->bits[bit]/3))<<lshift);
        value=value+(((uint32_t)(this->bits[bit]/3))<<lshift);
    }
    return value;
}

void binary::sprint(char value[2], uint32_t bit)
{
    if(bit>this->nbit-1)
    {
        printf("ERROR in binary::Print: bit=%i>nbit\n",bit);
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
    char c[2];
    gr->SubPlot(1,this->npin,this->pin);
    for(uint32_t i=0;i<this->nbit;i++)
    {
        this->sprint(c,i);
//printf("subplot %i: add label \"%s\" at t=%fs\n",this->pin,c,this->t.a[i]);
        gr->Puts(mglPoint(this->t.a[i],1.5,0.5),c);
    }
    return 0;
}


    
