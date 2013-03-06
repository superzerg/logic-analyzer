#include "microwire.h"

microwire::microwire(uint8_t pins[],uint8_t npin)
{
    this->mosi_mess=NULL;
    this->miso_mess=NULL;
    if(pins!=NULL && npin>0)
        this->init_acquisition(pins,npin);
}

microwire::~microwire()
{
    if(mosi_mess!=NULL)
        delete[] mosi_mess;
    if(miso_mess!=NULL)
        delete[] miso_mess;
    mosi_mess=NULL;
    miso_mess=NULL;
}

void microwire::decode()
{
    cs.init(&capture, 0, HIGHV);
    clk.init(&capture, 1,&cs);
    mosi.init(&capture, &clk, 2, 'u');
    miso.init(&capture, &clk, 3, 'd');
    this->decode_mosi();
    this->decode_miso();    
}

void microwire::decode_mosi()
{
    this->nmessage=this->cs.nactive;
    this->mosi_mess=new message[this->nmessage];
    binary *data;
    float clk_period=this->clk.period;
    float t0,t1;
    uint32_t firstbit,lastbit;
    char label[30];
    for (uint32_t i=0;i<nmessage;i++)
    {
        t0=0;
        firstbit=0;
        data=new binary(this->mosi,cs.t_start[i],cs.t_end[i]);
        //look for starting 1
        for(uint32_t bit=0;bit<data->nbit;bit++)
        {
            if(data->Get(bit)==1 && data->Get_nbad(bit)==0)
            {
                firstbit=bit+1;
                break;
            }
        }
        if(firstbit==0)
        {
            t0=cs.t_start[i];
        }
        else
        {
            t0=data->t.a[firstbit]-clk_period/2;
        }
        //Decode instruction
//printf("message %i, bits[%i:%i]=%i\n",i,firstbit,firstbit+1,data->Get(firstbit,firstbit+1));
        switch(data->Get(firstbit,firstbit+1))
        {
            case 2://READ
                lastbit=firstbit+7;
                if(lastbit<data->nbit)
                    t1=data->t.a[lastbit]+clk_period/2;
                else
                     continue;
                sprintf(label,"READ at 0x%2X",data->Get(firstbit+2,lastbit));
                this->mosi_mess[i].init(data,label,t0,t1-t0,'y');
                break;
            case 3://ERASE
                lastbit=firstbit+7;
                if(lastbit<data->nbit)
                    t1=data->t.a[lastbit]+clk_period/2;
                else
                    continue;
                sprintf(label,"ERASE 0x%2X",data->Get(firstbit+2,lastbit));
                this->mosi_mess[i].init(data,label,t0,t1-t0,'y');
                break;
            case 1://WRITE
                lastbit=firstbit+23;
                if(lastbit<data->nbit)
                    t1=data->t.a[lastbit]+clk_period/2;
                else
                    continue;
                sprintf(label,"WRITE %04X at 0x%2X",data->Get(firstbit+8,lastbit),data->Get(firstbit+2,firstbit+7));
                this->mosi_mess[i].init(data,label,t0,t1-t0,'y');
                break;
            case 0://need 2 more bits to get instruction
//printf("message %i, bits[2:3]=%i\n",i,data->Get(firstbit+2,firstbit+3));
                switch(data->Get(firstbit+2,firstbit+3))
                {
                    case 3://ENABLE WRITE
                        lastbit=firstbit+7;
                        if(lastbit<data->nbit)
                            t1=data->t.a[lastbit]+clk_period/2;
                        else
                            continue;
                        this->mosi_mess[i].init(data,"ENABLE WRITE",t0,t1-t0,'y');
                        break;
                    case 0://DISABLE WRITE
                        lastbit=firstbit+7;
                        if(lastbit<data->nbit)
                            t1=data->t.a[lastbit]+clk_period/2;
                        else
                            continue;
                        this->mosi_mess[i].init(data,"DISABLE WRITE",t0,t1-t0,'y');
                        break;
                    case 2://ERASE ALL
                        lastbit=firstbit+7;
                        if(lastbit<data->nbit)
                            t1=data->t.a[lastbit]+clk_period/2;
                        else
                            continue;
                        this->mosi_mess[i].init(data,"ERASE ALL",t0,t1-t0,'y'); 
                        break;
                    case 1://WRITE ALL
                        lastbit=firstbit+23;
                        if(lastbit<data->nbit)
                            t1=data->t.a[lastbit]+clk_period/2;
                        else
                            continue;
                        sprintf(label,"WRITE ALL %04X",data->Get(firstbit+8,lastbit));
                        this->mosi_mess[i].init(data,label,t0,t1-t0,'y');
                        break;
                    default:
                        printf("ERROR while reading instruction: got 2nd OP code=%i\n",data->Get(firstbit+2,firstbit+3));
                }
                break;
            default:
                printf("ERROR while reading instruction: got OP=%i\n",data->Get(firstbit,firstbit+1));
        }
        delete data;
    }
}

void microwire::decode_miso()
{
    this->nmessage=this->cs.nactive;
    this->miso_mess=new message[this->nmessage];
    binary *data;
    float clk_period=this->clk.period;
    float t0,t1;
    uint32_t firstbit,lastbit;
    char label[30];
    for (uint32_t i=0;i<nmessage;i++)
    {
        t0=0;
        firstbit=0;
        data=new binary(this->miso,cs.t_start[i],cs.t_end[i]);
        //look for starting 0
        for(uint32_t bit=0;bit<data->nbit;bit++)
            if(data->Get(bit)==0 && data->Get_nbad(bit)==0)
            {
                firstbit=bit+1;
                break;
            }
        if(firstbit==0)
        {
            t0=cs.t_start[i];
        }else
        {
            t0=data->t.a[firstbit]-clk_period/2;
        }
        //Decode instruction
        lastbit=firstbit+15;
        if(lastbit>=data->nbit)
            continue;
        sprintf(label,"%04X",data->Get(firstbit,lastbit));
        t1=data->t.a[lastbit]+clk_period/2;
        this->miso_mess[i].init(data,label,t0,t1-t0,'y');
    }
}

int microwire::Draw(mglGraph *gr)
{
//printf("microwire::Draw() start\n");
    float logic_values[]={0, 1, 2, 3};
    const char *ylabels[]={"0V", "HR", "ERR", "3V3"};
    const char *plot_labels[4]={"CS","CLK","MOSI","MISO"};
    gr->SetTicksVal('y',4,logic_values,ylabels);
    gr->SetPlotFactor(1.15);
//printf("capture.Draw()\n");
    this->capture.Draw(gr,plot_labels);
//printf("cs.Draw()\n");
    this->cs.Draw(gr,0);
    this->cs.Draw(gr,1);
//printf("clk.Draw()\n");
    this->clk.Draw(gr);
//printf("mosi.Draw()\n");
    this->cs.Draw(gr,2);
    this->clk.Draw(gr,'u',2);
    this->mosi.Draw(gr);
//printf("miso.Draw()\n");
    this->cs.Draw(gr,3);
    this->clk.Draw(gr,'d',3);
    this->miso.Draw(gr);
    if(this->mosi_mess!=NULL)
    {
        for (uint32_t i=0;i<this->nmessage;i++)
        {
            this->mosi_mess[i].Draw(gr);
        }
    }
    if(this->miso_mess!=NULL)
    {
        for (uint32_t i=0;i<this->nmessage;i++)
        {
            this->miso_mess[i].Draw(gr);
        }
    }
    return 0;
}

