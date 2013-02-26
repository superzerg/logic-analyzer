#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bcm2835.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <mgl/mgl_define.h>

#include <unistd.h>
//Compile with :
// g++ -o logic_input -l rt logic_input.cpp -l bcm2835 -l mgl -O3

#define DELAY 2 //delay in ms between 2 measurements
#define MISO 27 
#define MOSI 4
#define CLK 14
#define CE1 15
#define NPIN 4
#define NDATA 500

/*typedef enum
{
    INPUT_0=CE1,
    INPUT_1=CLK,
    INPUT_2=MOSI,
    INPUT_3=MISO,
}logic_inputs;*/

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
    logic_input(uint8_t npin=0,uint8_t * pins=NULL, const char *labels[]=NULL,uint32_t npoint=0)
    {
//printf("logic_input constructor\n");
        this->npin=npin;
        if (npin>0)
            this->rawdata=new mglData[npin];
        else
            this->rawdata=NULL;
//printf("rawdata->0x%X\n",this->rawdata);
        this->npoint=0;
        this->pins=NULL;
        this->labels=NULL;
        if (pins!=NULL && labels!=NULL)
        {
            init_acquisition(pins, labels,npin);
            if (npoint>0)
                acquire(npoint);
        }
    }
    
    ~logic_input()
    {
//printf("logic_input destructor\n");
        if(this->npin>0)
            for (int pin=0;pin<this->npin;pin++)
                if (this->npoint>0) 
                {
//printf("rawdata[%i] destruction\n");
                    this->rawdata[pin].~mglData();
                }
        if (this->npoint>0)
        {
//printf("t destruction\n");
            this->t.~mglData();
        }
        if (this->rawdata!=NULL)
        {
//printf("rawdata destruction at 0x%X\n",rawdata);
            delete[] rawdata;
        }
        if (this->pins!=NULL)
        {
//printf("pins destruction at 0x%X\n",pins);
            delete[] pins;
        }
        if (this->labels!=NULL)
        {
//printf("labels destruction at 0x%X\n",labels);
            delete[] labels;
        }
    }
    
    void init_acquisition(uint8_t * pins, const char *labels[], uint8_t npin=0)
    {
        if (npin==0)
            npin=this->npin;
        if(npin<1)
        {
            printf("ERROR in init_acquisition: npin must be > 0\n");
            return;
        }
        if(npin!=this->npin)
        {
            printf("ERROR in init_acquisition: pin number varied from construction.\n");
            return;
        }
//printf("pins memory allocution\n");
        this->pins = new uint8_t[npin];
//printf("pins->0x%X\n",this->pins);
//printf("labels memory allocution\n");
        this->labels = new char[npin][30];
//printf("labels->0x%X\n",this->labels);
//printf("dummy memory allocution\n");
//uint8_t *dummy=new uint8_t;
//printf("dummy->0x%X\n",dummy);
//delete dummy;
        for (int pin=0;pin<npin;pin++)
        {
            this->pins[pin]=pins[pin];
//printf("copy labels to %X\n",this->labels[pin]);
            strcpy(this->labels[pin],labels[pin]);
        }
    }
    
    void acquire(uint32_t npoint=0)
    {
        int pin;
        if(npoint==0)
            npoint=this->npoint;
        if(npoint<1)
        {
            printf("ERROR in aquire: npoint must be > 0\n");
            return;
        }
        printf("allocate memory.\n");
        //allocate memory for data objects
//printf("allocate t.\n");
        this->t.Create(npoint);
//printf("allocate rawdata[pin].\n");
        for (pin=0;pin<this->npin;pin++)
        {
//printf("pin %i, nx=%i\n",pin,rawdata[pin].nx);
            this->rawdata[pin].Create(npoint);
        }
        struct timespec start;
        struct timespec end;
        
        if (!bcm2835_init())
        {
            printf("ERROR in acquire while intializing BCM2835\n");
            return;
        }
        for (pin=0;pin<npin;pin++)
            bcm2835_gpio_fsel(this->pins[pin], BCM2835_GPIO_FSEL_INPT);
        //start aquisition
        printf("start aquisition\n");
        clock_gettime(CLOCK_MONOTONIC,&start);
        for (int i=0;i<npoint;i++)
        {
            for (pin=0;pin<this->npin;pin++)
                bcm2835_gpio_set_pud(this->pins[pin], BCM2835_GPIO_PUD_UP);
            for (pin=0;pin<this->npin;pin++)
                rawdata[pin].a[i]=bcm2835_gpio_lev(this->pins[pin]);
//for (pin=0;pin<this->npin;pin++)
//    printf("gpio%i (pull UP)=%f\n",this->pins[pin],rawdata[pin].a[i]);
            for (pin=0;pin<this->npin;pin++)
                bcm2835_gpio_set_pud(this->pins[pin], BCM2835_GPIO_PUD_DOWN);
            for (pin=0;pin<this->npin;pin++)
                rawdata[pin].a[i]+=2*bcm2835_gpio_lev(this->pins[pin]);
//for (pin=0;pin<this->npin;pin++)
//    printf("gpio%i (pull DOWN)=%f\n",this->pins[pin],rawdata[pin].a[i]);
            clock_gettime(CLOCK_MONOTONIC,&end);
            t.a[i]=(float)(end.tv_sec-start.tv_sec + ((float)(end.tv_nsec-start.tv_nsec))/1000000000);
            if(DELAY>=1)
                bcm2835_delay(DELAY);
            if(DELAY<1 && DELAY>=0.001)
                bcm2835_delayMicroseconds(DELAY*1000);        
        }
        bcm2835_close();
        printf("aquisition done\n");
    }
    
    int Draw(mglGraph *gr)
    {
printf("Drawing\n");
        gr->SetRanges(0,this->t.a[this->t.nx-1]*1.05,-0.3,3.3);
        for (int pin=0;pin<this->npin;pin++)
        {
printf("subplot %i\n",pin);
            gr->SubPlot(1,this->npin,pin);
            gr->Box();
            gr->Axis("xy");
printf("Label: %s\n",this->labels[pin]);
            gr->Label('y',this->labels[pin]);
printf("plot\n");
            gr->Plot(this->t,this->rawdata[pin],"+-");
printf("done\n");
        }
        return 0;
    }
    
//private:
    uint32_t npoint;
    uint8_t npin;
    uint8_t *pins;
    char (*labels)[30];
    mglData *rawdata;
    mglData t;
};

/*
class clock: public mglDraw  //clock transition
{
    clock()
    {
        this->index_transition_up=NULL;
        this->index_transition_down=NULL;        
    }
    
    clock(logic_input *data,uint8_t pin_clock)
    {
        find_transition_up(data, pin_clock);
        find_transition_down(data, pin_clock);
    }

    ~clock()
    {
        if(this->index_transition_up!=NULL)
            delete[] index_transition_up;
        if(this->index_transition_down!=NULL)        
            delete[] index_transition_down;
    }
    
    void find_transition(logic_input *data,uint8_t pin_clock,char sign='u')
    {
        if (data==NULL) {
            printf("ERROR in clock::find_transition: data is NULL");
            return;
        }else if (data->npin==0) {
            printf("ERROR in clock::find_transition: data->npin is 0");
            return;
        }else if (data->npoint==0) {
            printf("ERROR in clock::find_transition: data->npoint is 0");        
            return;
        }else if (pin_clock<0 || pin_clock>data->npin) {
            printf("ERROR in clock::find_transition: pin_clock must be >0 and <data->npin\n");
            return;
        }
        float oldstate_ok,newstate_ok;
        uint32_t *ntransition;
        uint32_t **index_transition;
        mglData *t_transition;
        switch(sign)
        {
            case 'u':
                oldstate_ok=0;
                newstate_ok=1;
                ntransition=&this->ntransition_up;
                index_transition=&this->index_transition_up;
                t_transition=&this->t_transition_up;
                break;
            case 'd':
                oldstate_ok=1;
                newstate_ok=0;
                ntransition=&this->ntransition_down;
                index_transition=&this->index_transition_down;
                t_transition=&this->t_transition_down;
                break;
            default:
                printf("ERROR in clock::find_transition: sign unknown (must be \'u\' or \'d\')\n");
                return;
        }
        //find number of transition first
        uint32_t nskip=0;
        float oldstate=1./3;
        *ntransition=0;
        for (uint32_t i=0;i<data->npoint;i++)
        {
            clock_val=data->rawdata[pin_clock].a[i];
            if(oldstate==0 && clock_val==1)
                 (*ntransition)++;
            if(clock_val==0 || clock_val==1 ||nskip>3)
            {
                oldstate=clock[i];
                nskip=0;
            }else
                nskip++;
        if ()
        //Allocate memory for clock variables
        t_transition->Create(*ntransition);
        *index_transition=new uint32_t[*ntransition]
        //Fill clock variables
        nskip=0;
        oldstate=1./3;
        float clock_val;
        for (uint32_t i=0;i<data->npoint;i++)
        {
            clock_val=data->rawdata[pin_clock].a[i];
            if(oldstate==0 && clock_val==1)
            {
                *index_transition=i;
                t_transition->a[i]=data->t.a[i];
            }
            if(clock_val==0 || clock_val==1 ||nskip>3)
            {
                oldstate=clock[i];
                nskip=0;
            }else
                nskip++;
        }
    }
    
    void find_transition_up(logic_input *data,uint8_t pin_clock)
    {
        this->find_transition(data, pin_clock,'u');
    }
    
    void find_transition_down(logic_input *data,uint8_t pin_clock)
    {
        this->find_transition(data, pin_clock,'d');
    }
    
    int draw(grap *gr)
    {
        return 0;
    }
    
//private:
    uint32_t ntransition_up;
    uint32_t *index_transition_up;
    mglData t_transition_up;
    uint32_t ntransition_down;
    uint32_t *index_transition_down;
    mglData t_transition_down;
};

class binary : public mglDraw
{
    binary(uint32_t nbit=0)
    {
        if(nbit<0)
        {
            printf("ERROR in binary constructor, nbit must be>0\n");
            return;
        }
        if (this->bits!=NULL)
            delete[] this->bits;
        if (t.nx>1)
            this->t.~mglData();
        this->nbit=nbit;
        this->bits=new logic_state[nbit];
        t.Create(nbit);
    }
    
    binary(logic_input *data, clock *clk, uint8_t pin, char transition_direction='u')
    {
        uint32_t nbit;
        uint32_t *index_transition;
        mglData *t_clk;
        mglData *t;
        if (pin<0 || pin>=data->npin)
        {
            printf("ERROR in binary constructor, pin must be valid in data\n");
            return;
        }
        switch (transition_direction)
        {
            case 'u':
                nbit=clk->ntransition_up;
//To check : index_transition[i]==clk->index_transition_up[i]
                index_transition=clk->index_transition_up;
//same for t                
                t_clk=&(clk->t_transition_up);
                break;
            case 'd':
                nbit=clk->ntransition_down;
//To check : index_transition[i]==clk->index_transition_down[i]
                index_transition=clk->index_transition_down;
//same for t                
                t_clk=&(clk->t_transition_down);
                break;
             default:
                printf("ERROR in binary constructor, transition_direction must be \'u\' or \'d\'\n");
                return;
        }
        this->nbit=nbit;
        this->bits=new logic_state[nbit];
        t.Create(nbit);
        for (int i=0;i<nbit;i++)
        {
            this->bits[i]=data->rawdata[pin].a[index_transition[i]]*3;
            this-t.a[i]=t_clk->a[i];
        }
    } 
       
    ~binary()
    {
        if (this->bits!=NULL)
            delete[] this->bits;
        if (t.nx>1)
            this->t.~mglData();
        this->nbit=0;
    }

    int draw(grap *gr)
    {
        return 0;
    }
    
//private:
    logic_state* bits=NULL;
    mglData t;
    uint32_t nbit=0
};

class message: public mglDraw
{
    int draw(grap *gr)
    {
        return 0;
    }
//private:
    char * label;
    bits * binary;
    char color;
    float t0;
    float tmessage;
}

class microwire: public mglDraw
{
    
};*/
    
int main(int argc, char **argv)
{
    logic_input capture(4);
    uint8_t pins[4]={15,14,4,27};
    const char *labels[4]={"CS","CLK","MOSI","MISO"};
    capture.init_acquisition(pins, labels);
    capture.acquire(NDATA);

    mglGraphZB gr(1200,800);
    float logic_values[]={0, 1, 2, 3};
    const char *ylabels[]={"0V", "HR", "ERR", "3V3"};
    gr.SetTicksVal('y',4,logic_values,ylabels);
    gr.SetPlotFactor(1.15);
    capture.Draw(&gr);
    gr.WritePNG("test.png");
    return 0;
}
