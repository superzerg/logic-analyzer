#include "logic_input.h"

logic_input::logic_input(uint8_t npin,uint8_t * pins,uint32_t npoint,float delay)
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
    if (pins!=NULL)
    {
        init_acquisition(pins,npin);
        if (npoint>0)
            acquire(npoint,delay);
    }
}
    
logic_input::~logic_input()
{
//printf("logic_input destructor\n");
/*        if(this->npin>0)
        for (int pin=0;pin<this->npin;pin++)
            if (this->npoint>0) 
            {
printf("rawdata[%i] destruction\n",pin);
                this->rawdata[pin].~mglData();
            }
    if (this->npoint>0)
    {
printf("t destruction\n");
        this->t.~mglData();
    }*/
//It seems to be the right thing to do, but do "double free or corruption" error
    if (this->rawdata!=NULL)
    {
//printf("rawdata destruction at 0x%X\n",this->rawdata);
        delete[] this->rawdata;
    }
    if (this->pins!=NULL)
    {
//printf("pins destruction at 0x%X\n",this->pins);
        delete[] this->pins;
    }
//printf("done\n");
}
    
void logic_input::init_acquisition(uint8_t * pins, uint8_t npin)
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
   for (int pin=0;pin<npin;pin++)
    {
        this->pins[pin]=pins[pin];
    }
}
    
void logic_input::acquire(uint32_t npoint,float delay)
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
    for (uint32_t i=0;i<npoint;i++)
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
        if(delay>=1)
            bcm2835_delay(delay);
        if(delay<1 && delay>=0.001)
            bcm2835_delayMicroseconds(delay*1000);        
    }
    bcm2835_close();
    this->npoint=npoint;
    printf("aquisition done\n");
    }
    
int logic_input::Draw(mglGraph *gr, const char *labels[])
{
//printf("logic_input::Draw()\n");
    gr->SetRanges(0,this->t.a[this->t.nx-1]*1.05,-0.3,3.5);
    char stats[50];
    sprintf(stats,"acquired %.1f points/s",this->npoint/(this->t.a[this->npoint-1]-this->t.a[0]));
//printf("Puts(%s at (%f,%f,%f)\n",stats,gr->Max.x/10, 3., 1.);
    char deltax_ticks_str[10];
    float deltax_ticks;
    sprintf(deltax_ticks_str,"%.1f",gr->Max.x/10-gr->Min.x/10);
    deltax_ticks=atof(deltax_ticks_str);
    for (uint8_t pin=0;pin<this->npin;pin++)
    {
//printf("subplot %i/%i\n",pin,this->npin);
        gr->SubPlot(1,this->npin,pin);
        gr->SetTicks('x',deltax_ticks);
///printf("Box()\n");
        gr->Box();
//printf("Axis(\"xy\")\n");
        gr->Axis("xy");       
//printf("ok\n");
        if(labels!=NULL)
        {
//printf("Label: %s\n",labels[pin]);
            gr->Label('y',labels[pin]);
        }
//printf("plot\n");
        if(pin==0)
            gr->Puts(mglPoint(gr->Max.x/10, 3.2, 1.),stats);
        gr->Plot(this->t,this->rawdata[pin],"k+-");
//printf("done\n");
    }
    return 0;
}

