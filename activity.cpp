#include "activity.h"

activity::activity(logic_input *data,uint8_t pin, logic_state active_value)
{
    pmesg(DEBUG,"activity constructor.\n");
    this->nactive=0;
    this->t_start=NULL;
    this->t_end=NULL;
    this->npin=0;
    if(data==NULL);
        return;
    this->init(data,pin,active_value);
}

activity::~activity()
{
    pmesg(DEBUG,"activity destructor.\n");
    if(this->t_start!=NULL)
        delete[] this->t_start;
    if(this->t_end!=NULL)
        delete[] this->t_end;
    this->nactive=0;
    this->t_start=NULL;
    this->t_end=NULL;
    this->npin=0;
}

void activity::init(logic_input *data,uint8_t cs_pin, logic_state active_value)
{
    pmesg(DEBUG,"activity::init().\n");
    logic_state inactive_value;
    if(active_value == HIGHV)
        inactive_value = LOWV;
    else if (active_value == LOWV)
        inactive_value = HIGHV;
    else
    {
        pmesg(ERROR,"ERROR in activity::init(): bad active_value = %i\n",active_value);
        return;
    }
    if(data==NULL)
    {
        pmesg(ERROR,"ERROR in activity::init(): data is NULL\n");
        return;
    }
    if (cs_pin>=data->npin)
    {
        pmesg(ERROR,"ERROR in activity::init(): bad pin %i\n",cs_pin);
        return;
    }
    this->npin=data->npin;
    pmesg(DEBUG,"activity::npin=%i\n",this->npin);
    //get the number of activation
    uint8_t active=0;
    pmesg(DEBUG,"look in the %i points.\n",data->npoint);
    for(uint32_t point=0;point<data->npoint;point++)
    {
        if((data->rawdata[cs_pin].a[point]==active_value ) && (active==0))
        {
            this->nactive++;
            active=1;
            pmesg(DEBUG,"activity detected\n");
        }
        if((data->rawdata[cs_pin].a[point]==inactive_value) && (active==1))
        {
            active=0;
        }   
    }
    //allocate memmory
    this->t_start=new float[this->nactive];
    this->t_end=new float[this->nactive];
    //fill t_start and t_end
    uint8_t nactive=0;
    active=0;
    for(uint32_t point=0;point<data->npoint;point++)
    {
        if((data->rawdata[cs_pin].a[point]==active_value) && (active==0))
        {
            t_start[nactive]=data->t.a[point];
            pmesg(INFORMATION,"%ith activity at t=%fs ",nactive+1,t_start[nactive]);
            active=1;
        }
        if((data->rawdata[cs_pin].a[point]==inactive_value) && (active==1))
        {
            t_end[nactive]=data->t.a[point-1];
            pmesg(INFORMATION,"end at t=%fs\n",t_end[nactive]);
            active=0;
            nactive++;
        }   
    }
    //add last point if necessary
    if(nactive!=this->nactive)
    {
        if(nactive==this->nactive-1)
        {
            t_end[nactive]=data->t.a[data->npoint-1];
        pmesg(INFORMATION,"end at t=%fs\n",t_end[nactive]);
        }else
            pmesg(WARNING,"ERROR in activity::init(): number of activation does not match between two calls.\n");
    }
}

int activity::Draw(mglGraph *gr,uint8_t subplot)
{
    pmesg(DEBUG,"activity::Draw(%i).\n",subplot);
    if(subplot>=this->npin)
    {
        pmesg(WARNING,"ERROR in activity::Draw(), subplot=%i greater than the number of plot=%i.\n",subplot,this->npin);
        return 1;
    }
    if(gr==NULL)
    {
        pmesg(ERROR,"ERROR in activity::Draw(),gr is NULL.\n");
        return 1;
    }
    gr->SubPlot(1,this->npin,subplot);
    gr->FaceZ(0,gr->Min.y,-1,this->t_start[0],gr->Max.y-gr->Min.y,"r9");
    for (uint32_t i=0;i<this->nactive;i++)
    {
        gr->FaceZ(this->t_start[i], gr->Min.y,-1.5, this->t_end[i]-this->t_start[i], gr->Max.y-gr->Min.y, "g9"); 
        if (i<this->nactive-1)
        {
            gr->FaceZ(this->t_end[i],gr->Min.y, -1.5, this->t_start[i+1]-this->t_end[i], gr->Max.y-gr->Min.y, "r9");
        }
    }
    gr->FaceZ(this->t_end[this->nactive-1], gr->Min.y, -1.5, gr->Max.x-this->t_end[this->nactive-1], gr->Max.y-gr->Min.y,"r9");
    return 0;
}

