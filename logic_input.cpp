#include "logic_input.h"

logic_input::logic_input(uint8_t npin,uint8_t * pins,uint32_t npoint,float delay)
{
printf("logic_input constructor with npin=%i\n",npin);
    this->npin=0;
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
printf("logic_input destructor\n");
    this->npin=0;
    this->npoint=0;
    if (this->rawdata!=NULL)
    {
printf("logic_input destructor: rawdata destruction at 0x%X\n",this->rawdata);
        delete[] this->rawdata;
        this->rawdata=NULL;
    }
    if (this->pins!=NULL)
    {
printf("logic_input destructor: pins destruction at 0x%X\n",this->pins);
        delete[] this->pins;
        this->pins=NULL;
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
//test disable destructors
//    this->~logic_input();
printf("pins memory allocution (array of %i)\n",npin);
    this->rawdata=new mglData[npin];
    this->pins = new uint8_t[npin];
    this->npin = npin;
//printf("pins->0x%X\n",this->pins);
   for (int pin=0;pin<npin;pin++)
    {
        this->pins[pin]=pins[pin];
    }
}
    
void logic_input::acquire(uint32_t npoint,float delay)
{
//printf("logic_input::acquire(%i , %f)\n",npoint, delay);
    int pin;
    if(npoint==0)
        npoint=this->npoint;
    if(pins==NULL)
    {
        printf("ERROR in logic_input::acquire(), logic_input::init_acquisition() must be called first to set pins\n");
        return;
    }
//printf("allocate memory.\n");
    //allocate memory for data objects
//printf("allocate t for %i points.\n",npoint);
    this->t.Create(npoint);
  
//printf("allocate rawdata[pin] which is at %X.\n",this->rawdata);
    for (pin=0;pin<this->npin;pin++)
    {
        this->rawdata[pin].Create(npoint);
//printf("pin %i, nx=%i\n",pin,this->rawdata[pin].nx);
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
        this->t.a[i]=(float)(end.tv_sec-start.tv_sec + ((float)(end.tv_nsec-start.tv_nsec))/1000000000);
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

