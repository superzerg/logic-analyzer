#include "logic_input.h"

logic_input::logic_input(uint8_t npin,uint8_t * pins,uint32_t npoint,float delay)
{
    pmesg(DEBUG,"logic_input constructor\n");
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
    pmesg(DEBUG,"logic_input destructor\n");
    this->Clean();
}

void logic_input::Clean()
{
    pmesg(DEBUG,"logic_input::Clean()\n");
    if(this->npin!=0 && this->rawdata!=NULL)
    {
        for (uint8_t pin=0;pin<this->npin;pin++)
        {
            pmesg(DEBUG,"logic_input::Clean(): rawdata[%i] destruction.\n",pin);
            this->rawdata[pin].Create(1);
        }
        delete[] this->rawdata;
        this->rawdata=NULL;
    }
    this->npin=0;
    this->npoint=0;
    if (this->pins!=NULL)
    {
        pmesg(DEBUG,"logic_input::Clean(): pins destruction at 0x%X\n",this->pins);
        delete[] this->pins;
        this->pins=NULL;
    }
    
}
    
void logic_input::init_acquisition(uint8_t * pins, uint8_t npin)
{
    if (npin==0)
        npin=this->npin;
    if(npin<1)
    {
        pmesg(CRITICAL,"ERROR in init_acquisition: npin must be > 0\n");
        return;
    }
    pmesg(DEBUG,"pins memory allocution (array of %i)\n",npin);
    this->rawdata=new mglData[npin];
    this->pins = new uint8_t[npin];
    this->npin = npin;
    pmesg(DEBUG,"pins->0x%X\n",this->pins);
    for (int pin=0;pin<npin;pin++)
    {
        this->pins[pin]=pins[pin];
    }
}
    
void logic_input::acquire(uint32_t npoint,float delay)
{
    pmesg(DEBUG,"logic_input::acquire(%i , %f)\n",npoint, delay);
    int pin;
    if(npoint==0)
        npoint=this->npoint;
    if(pins==NULL)
    {
        pmesg(ERROR,"ERROR in logic_input::acquire(), logic_input::init_acquisition() must be called first to set pins\n");
        return;
    }
    pmesg(DEBUG,"allocate memory.\n");
    //allocate memory for data objects
    pmesg(DEBUG,"allocate t for %i points.\n",npoint);
    this->t.Create(npoint);
  
    pmesg(DEBUG,"allocate rawdata[pin] which is at %X.\n",this->rawdata);
    for (pin=0;pin<this->npin;pin++)
    {
        this->rawdata[pin].Create(npoint);
        pmesg(DEBUG,"pin %i, nx=%i\n",pin,this->rawdata[pin].nx);
    }
    
    struct timespec start;
    struct timespec end;
    
    if (!bcm2835_init())
    {
        pmesg(CRITICAL,"ERROR in logic_input::acquire while intializing BCM2835\n");
        return;
    }
    for (pin=0;pin<npin;pin++)
        bcm2835_gpio_fsel(this->pins[pin], BCM2835_GPIO_FSEL_INPT);
    //start aquisition
    pmesg(INFORMATION,"start aquisition\n");
    clock_gettime(CLOCK_MONOTONIC,&start);
    for (uint32_t i=0;i<npoint;i++)
    {
        for (pin=0;pin<this->npin;pin++)
            bcm2835_gpio_set_pud(this->pins[pin], BCM2835_GPIO_PUD_UP);
        for (pin=0;pin<this->npin;pin++)
            rawdata[pin].a[i]=bcm2835_gpio_lev(this->pins[pin]);
        for (pin=0;pin<this->npin;pin++)
            bcm2835_gpio_set_pud(this->pins[pin], BCM2835_GPIO_PUD_DOWN);
        for (pin=0;pin<this->npin;pin++)
            rawdata[pin].a[i]+=2*bcm2835_gpio_lev(this->pins[pin]);
        clock_gettime(CLOCK_MONOTONIC,&end);
        this->t.a[i]=(float)(end.tv_sec-start.tv_sec + ((float)(end.tv_nsec-start.tv_nsec))/1000000000);
        if(delay>=1)
            bcm2835_delay(delay);
        if(delay<1 && delay>=0.001)
            bcm2835_delayMicroseconds(delay*1000);        
    }
    bcm2835_close();
    this->npoint=npoint;
    pmesg(INFORMATION,"aquisition done\n");
    }
    
int logic_input::Draw(mglGraph *gr, const char *labels[], uint8_t pin_stats)
{
    pmesg(DEBUG,"logic_input::Draw()\n");
    gr->SetRanges(0,this->t.a[this->t.nx-1]*1.05,-0.3,3.5);
    char stats[50];
    sprintf(stats,"acquired %.1f points/s",this->npoint/(this->t.a[this->npoint-1]-this->t.a[0]));
    pmesg(DEBUG,"Puts(%s at (%f,%f,%f)\n",stats,gr->Max.x/10, 3., 1.);
    char deltax_ticks_str[10];
    float deltax_ticks;
    sprintf(deltax_ticks_str,"%.1f",gr->Max.x/10-gr->Min.x/10);
    deltax_ticks=atof(deltax_ticks_str);
    for (uint8_t pin=0;pin<this->npin;pin++)
    {
        pmesg(DEBUG,"subplot %i/%i\n",pin,this->npin);
        gr->SubPlot(1,this->npin,pin);
        gr->SetTicks('x',deltax_ticks);
        pmesg(DEBUG,"Box()\n");
        gr->Box();
        pmesg(DEBUG,"Axis(\"xy\")\n");
        gr->Axis("xy");       
        if(labels!=NULL)
        {
            pmesg(DEBUG,"Label: %s\n",labels[pin]);
            gr->Label('y',labels[pin]);
        }
        pmesg(DEBUG,"plot\n");
        if(pin==pin_stats)
            gr->Puts(mglPoint(gr->Max.x/10, 3.2, 1.),stats);
        gr->Plot(this->t,this->rawdata[pin],"k+-");
    }
    return 0;
}

const void logic_input::Save(const char* filename)
{
    FILE *f=fopen(filename,"w");
    size_t nwrote;
    if (f == NULL)
    {
        pmesg(ERROR,"ERROR in logic_input::Save, cannot open %s.\n",filename);
        return;
    }
    if (this->npin!=sizeof(rawdata))
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): npin=%i != sizeof(rawdata[])=%i.\n",this->npin,sizeof(rawdata));
        return;
    }
    if(this->rawdata==NULL || this->npin==0)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): rawdata is NULL.\n");
        return;
    }
    if (this->npoint!=this->rawdata[0].nx)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): npoint=%i != rawdata.nx=%i.\n",this->npoint,rawdata[0].nx);
        return;
    }
    nwrote= fwrite(&this->npin,sizeof(this->npin),1,f);
    if(nwrote!=1)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): wrote %i/1 npin.\n",nwrote);
        return;
    }
    nwrote= fwrite(&this->npoint,sizeof(this->npoint),1,f);
    if(nwrote!=1)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): wrote %i/1 npoint.\n",nwrote);
        return;
    }
    nwrote= fwrite(this->pins,sizeof(*this->pins),this->npin,f);
    if(nwrote!=this->npin)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): wrote %i/%i pins[].\n",nwrote,this->npin);
        return;
    }
    for (uint8_t pin=0;pin<this->npin;pin++)
    {
        nwrote= fwrite(this->rawdata[pin].a,sizeof(*this->rawdata[pin].a),this->npoint,f);
        if(nwrote!=this->npoint)
        {
            pmesg(ERROR,"ERROR in logic_input::Save(): wrote %i/%i rawdata[%i].\n",nwrote,this->npoint,pin);
            return;
        }
    }
    nwrote= fwrite(this->t.a,sizeof(*this->t.a),this->npoint,f);
    if(nwrote!=this->npoint)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): wrote %i/%i t.\n",nwrote,this->npoint);
        return;
    }
    if(fclose(f)!=0)
    {
        pmesg(ERROR,"ERROR in logic_input::Save(): fclose() failed.\n");
        return;
    }
}

void logic_input::Load(const char* filename)
{
    FILE *f=fopen(filename,"r");
    size_t nread;
    //cleaning
    this->Clean();
    //loading npin from file
    nread=fread(&this->npin,sizeof(this->npin),1,f);
    if(nread!=1)
    {
         pmesg(ERROR,"ERROR in logic_input::Load(): read %i/%i npin.\n",nread,1);
         return;
    }
    //allocate memory
    this->pins=new uint8_t[this->npin];
    this->rawdata=new mglData[this->npin];
    //loading npoint from file
    nread=fread(&this->npoint,sizeof(this->npoint),1,f);
    if(nread!=1)
    {
         pmesg(ERROR,"ERROR in logic_input::Load(): read %i/%i npoint.\n",nread,1);
         return;
    }
    //allocate memory
    for (uint8_t pin=0;pin<this->npin;pin++)
        this->rawdata[pin].Create(this->npoint);
    //Fill pin array    
    nread=fread(this->pins,sizeof(*this->pins),this->npin,f);
    if(nread!=this->npin)
    {
         pmesg(ERROR,"ERROR in logic_input::Load(): read %i/%i pins.\n",nread,this->npin);
         return;
    }
    //Fill rawdata array
    for (uint8_t pin=0;pin<this->npin;pin++)
    {
        nread= fread(this->rawdata[pin].a,sizeof(*this->rawdata[pin].a),this->npoint,f);
        if(nread!=this->npoint)
        {
            pmesg(ERROR,"ERROR in logic_input::Load(): read %i/%i rawdata[%i].\n",nread,this->npoint,pin);
            return;
        }
    }
    //Fill t
    nread= fread(this->t.a,sizeof(*this->t.a),this->npoint,f);
    if(nread!=this->npoint)
    {
        pmesg(ERROR,"ERROR in logic_input::Load(): read %i/%i t.\n",nread,this->npoint);
        return;
    }
    if(fclose(f)!=0)
    {
        pmesg(ERROR,"ERROR in logic_input::Load(): fclose() failed.\n");
        return;
    }    
}





