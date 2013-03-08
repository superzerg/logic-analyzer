#include "transition.h"


transition::transition(logic_input *data,uint8_t pin_clock,activity *cs)
{
    pmesg(DEBUG,"transition constructor.\n");
    this->ntransition_up=0;
    this->index_transition_up=NULL;
    this->ntransition_down=0;
    this->index_transition_down=NULL;
    this->npin=0;
    this->period=0;
    this->t_first_activity=0;
    if(data!=NULL)
        init(data,pin_clock,cs);
}

transition::~transition()
{
    pmesg(DEBUG,"transition destructor.\n");
    this->ntransition_up=0;
    this->ntransition_down=0;
    this->npin=0;
    this->period=0;
    this->t_first_activity=0;
    if(this->index_transition_up!=NULL)
    {
        delete[] index_transition_up;
    }
    this->index_transition_up=NULL;
    if(this->index_transition_down!=NULL)        
    {
        delete[] index_transition_down;
    }
    this->index_transition_down=NULL;
    this->t_transition_up.Create(1);
    this->t_transition_down.Create(1);
}

void transition::init(logic_input *data,uint8_t pin_clock,activity *cs)
{
    if(data==NULL)
    {
        pmesg(CRITICAL,"ERROR in transition::init(): data is NULL\n");
        return;
    }
    if(pin_clock>=data->npin)
    {
        pmesg(CRITICAL,"ERROR in  transition::init(): pin=%i not valid\n",pin_clock);
        return;
    }
    this->pin_clock=pin_clock;
    this->npin=data->npin;
    pmesg(DEBUG,"find u transitions\n");
    this->find_transition(data,'u');
    pmesg(DEBUG,"find d transitions\n");
    this->find_transition(data,'d');
    if(cs!=NULL)
        this->period=this->GetStats(cs,'u')/2+this->GetStats(cs,'d')/2;
}

float transition::GetStats(activity *cs,char sign)
{
    pmesg(DEBUG,"transition::GetStats().\n");
    uint32_t *index_transition=NULL;
    uint32_t *ntransition=NULL;
    mglData *t_transition=NULL;
    switch(sign)
    {
        case 'u':
            ntransition=&this->ntransition_up;
            index_transition=this->index_transition_up;
            t_transition=&this->t_transition_up;
           break;
        case 'd':
            ntransition=&this->ntransition_down;
            index_transition=this->index_transition_down;
            t_transition=&this->t_transition_down;
            break;
        default:
            pmesg(WARNING,"ERROR in transition::get_stats: sign unknown (must be \'u\' or \'d\')\n");
            return 0.;
    }
    if(index_transition==NULL)
    {
        pmesg(WARNING,"ERROR in transition::get_stats: transition::init() must be called first\n");
        return 0.;
    }
    float period_min=1e6, period_max=0, period_mean=0,period;
    uint32_t nperiod=0;
    uint32_t j=0;
    for(uint32_t i=0;i<cs->nactive;i++)
    {
        for(;j<*ntransition-1;j++)
        {
            if(t_transition->a[j]>=cs->t_end[i] || t_transition->a[j+1]>=cs->t_end[i])
                break;
            if(t_transition->a[j]>cs->t_start[i] && t_transition->a[j+1]>cs->t_start[i])
            {
                period=t_transition->a[j+1]-t_transition->a[j];
                pmesg(DEBUG,"mess %i T %i/%i=%f (%f - %f), limits =(%f , %f)\n",i+1, j+1, *ntransition-1 ,period, t_transition->a[j+1], t_transition->a[j], cs->t_start[i], cs->t_end[i]);
                if(period_min > period)
                    period_min=period;
                if(period_max < period)
                    period_max=t_transition->a[j+1]-t_transition->a[j];
                if(this->t_first_activity==0)
                    this->t_first_activity=cs->t_start[i]/2+cs->t_end[i]/2;
                period_mean+=period;
                nperiod++;
            }
        }
    }
    period_mean/=nperiod;
    this->period=period_mean;
    this->period_min=period_min;
    this->period_max=period_max;
    return this->period;
}

void transition::find_transition(logic_input *data,char sign)
{
    pmesg(DEBUG,"transition::find_transition\n");
    if (data==NULL) 
    {
        pmesg(ERROR,"ERROR in transition::find_transition: data is NULL\n");
        return;
    }
    if (data->npin==0) 
    {
        pmesg(ERROR,"ERROR in transition::find_transition: data->npin is 0\n");
        return;
    }
    if (data->npoint==0) 
    {
        pmesg(ERROR,"ERROR in transition::find_transition: data->npoint is 0\n");        
        return;
    }
    if (this->pin_clock<0 || this->pin_clock>data->npin) 
    {
        pmesg(ERROR,"ERROR in transition::find_transition: pin_clock must be >0 and <data->npin\n");
        return;
    }
    float oldstate_ok,newstate_ok;
    uint32_t *ntransition;
    uint32_t **index_transition;
    mglData *t_transition;
    switch(sign)
    {
        case 'u':
            oldstate_ok=LOWV;
            newstate_ok=HIGHV;
            ntransition=&this->ntransition_up;
            index_transition=&this->index_transition_up;
            pmesg(DEBUG,"this->t_transition_up at %X\n",&this->t_transition_up);
            t_transition=&this->t_transition_up;
            break;
        case 'd':
            oldstate_ok=LOWV;
            newstate_ok=HIGHV;
            ntransition=&this->ntransition_down;
            index_transition=&this->index_transition_down;
            t_transition=&this->t_transition_down;
            pmesg(DEBUG,"this->t_transition_down at %X\n",&this->t_transition_down);
            break;
        default:
            pmesg(ERROR,"ERROR in transition::find_transition: sign unknown (must be \'u\' or \'d\')\n");
            return;
    }
    //find number of transition first
    pmesg(DEBUG,"find number of transitions...\n");    
    uint32_t nskip=0;
    float oldstate=HIGHR;
    float clock_val;
    *ntransition=0;
    for (uint32_t i=0;i<data->npoint;i++)
    {
        clock_val=data->rawdata[this->pin_clock].a[i];
        if(oldstate==oldstate_ok && clock_val==newstate_ok)
             (*ntransition)++;
        if(clock_val==LOWV || clock_val==HIGHV ||nskip>3)
        {
            oldstate=clock_val;
            nskip=0;
        }else
            nskip++;
    }
    pmesg(DEBUG,"case %c, pin %i, %i trans found, allocate memory\n",sign,this->pin_clock,*ntransition);
    pmesg(DEBUG,"ntransition_up=%i, ntransition_down=%i\n",this->ntransition_up,this->ntransition_down);
    //Allocate memory for clock variables
    pmesg(DEBUG,"t_transition->nx=%i\n",t_transition->nx);
    pmesg(DEBUG,"t_transition->Create(%i) with t_transition=%X\n",*ntransition,t_transition);
    t_transition->Create(*ntransition);
    pmesg(DEBUG,"new uint32_t[%i])\n",*ntransition);
    (*index_transition)=new uint32_t[*ntransition];
    //Fill clock variables
    nskip=0;
    oldstate=HIGHR;
    uint32_t j=0;
    pmesg(DEBUG,"Fill variables\n");
    for (uint32_t i=0;i<data->npoint;i++)
    {
//        pmesg(DEBUG,"data point #%i, transition %i\n",i,j);
        clock_val=data->rawdata[this->pin_clock].a[i];
        if(oldstate==oldstate_ok && clock_val==newstate_ok)
        {
            (*index_transition)[j]=i;
            t_transition->a[j]=data->t.a[i];
            j++;
            pmesg(DEBUG,"transition found at t=%fs\n",data->t.a[i]);               
        }
        if(clock_val==LOWV || clock_val==HIGHV ||nskip>3)
        {
            oldstate=clock_val;
            nskip=0;
        }else
            nskip++;
    }
}

   
int transition::Draw(mglGraph *gr)
{
    pmesg(DEBUG,"transition::Draw().\n");
    gr->SubPlot(1,this->npin,this->pin_clock);
    char stats[50]="";
    if (this->t_first_activity!=0)
        sprintf(stats,"T=%.1fms (%.1fms to %.1fms)",this->period*1000,this->period_min*1000,this->period_max*1000);
    pmesg(DEBUG,"Puts(%f, %f),\"%s\")\n",this->t_first_activity,3.2,stats);
    gr->Puts(mglPoint(this->t_first_activity,3.2,1.),stats);
    return 0;
}   


int transition::Draw(mglGraph *gr,char sign,uint8_t subplot)
{
    pmesg(DEBUG,"transition::Draw(%c,%i).\n",sign,subplot);
    float oldstate,newstate;
    uint32_t *ntransition;
    mglData *t_transition;
    switch(sign)
    {
        case 'u':
            oldstate=0;
            newstate=3;
            ntransition=&this->ntransition_up;
            t_transition=&this->t_transition_up;
            break;
        case 'd':
            oldstate=3;
            newstate=0;
            ntransition=&this->ntransition_down;
            t_transition=&this->t_transition_down;
            break;
        default:
            pmesg(WARNING,"ERROR in transition::draw: sign unknown (must be \'u\' or \'d\')\n");
            return 1;
    }
    gr->SubPlot(1,this->npin,subplot);
    for (uint32_t i=0;i<(*ntransition);i++)
    {
        gr->Line(mglPoint(t_transition->a[i],oldstate,-0.5),mglPoint(t_transition->a[i],newstate,-0.5),"W-A");
    }
    return 0;
}

