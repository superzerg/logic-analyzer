#include "activity.h"

activity::activity(logic_input &data,uint8_t pin, float active_value)
{
    uint8_t active=0;
    this->nactive=0;
    this->t_start=NULL;
    this->t_end=NULL;
    if (pin>=data.npin)
    {
        printf("ERROR in activity constructor : bad pin %i",pin);
        return;
    }
    this->npin=data.npin;
    //get the number of activation
    for(uint32_t point=0;point<data.npoint;point++)
    {
        if((data.rawdata[pin].a[point]==3 ) && (active==0))
        {
            this->nactive++;
            active=1;
        }
        if((data.rawdata[pin].a[point]==0) && (active==1))
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
    for(uint32_t point=0;point<data.npoint;point++)
    {
        if((data.rawdata[pin].a[point]==3) && (active==0))
        {
            t_start[nactive]=data.t.a[point];
printf("%ith activity at t=%fs ",nactive+1,t_start[nactive]);
            active=1;
        }
        if((data.rawdata[pin].a[point]==0) && (active==1))
        {
            t_end[nactive]=data.t.a[point-1];
printf("end at t=%fs\n",t_end[nactive]);
            active=0;
            nactive++;
        }   
    }
    //add last point if necessary
    if(nactive!=this->nactive)
    {
        if(nactive==this->nactive-1)
        {
            t_end[nactive]=data.t.a[data.npoint-1];
        }else
            printf("ERROR in activity constructor: number of activation does not match\n");
    }
}

activity::~activity()
{
    if(this->t_start!=NULL)
        delete[] this->t_start;
    if(this->t_end!=NULL)
        delete[] this->t_end;
}

int activity::Draw(mglGraph *gr,uint8_t subplot)
{
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

