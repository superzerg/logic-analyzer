//Compile with :
//make
//Tested with (look at ): 
//make test
//To look to the resulting image:
//make view

#include <mgl/mgl_zb.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

#ifndef RAW
#define RAW
#include "raw.h"
#endif

#ifndef MICROWIRE
#define MICROWIRE
#include "microwire.h"
#endif

debug_level msglevel=INFORMATION;
//int msglevel=DEBUG;

void usage(char *program_name)
{
	printf("usage: %s [-h] [-v level] [-w sec] [-l file | -c pins [-i ms] [-n npoint]] [-d protocol] [-s file] [-p file].\n",program_name);
	printf("program displaying GPIO states over time and decoding protocols if supported.\nprotocols supported are :\n- microwire\n- raw\nprogram parameters are:\n");
	printf("\t -h|--help : this help\n");
	printf("\t -l|--load file : Use data in binary file as captured data.\n");
	printf("\t -c|--capture pins: pins to use durring the capture. Must be the GPIO numbers separated by \',\' or \' \'. Space may be used if le string pins is protected by \"\". ex 14,15,27,4 or \"14 15 27 4\". Order may be inportant if --decode is used depending on the protocol.\n");
	printf("\t -i|--interval ms: interval in ms between 2 captures (default is 0.5 ms wich is the recomended minimum). For low intervals (ms<10), the interval can be sometime higher if the RPi is busy durring capture.\n");
	printf("\t -n|--npoint npoint: number of data point to capture (default 2000).\n");
	printf("\t -d|--decode protocol: decode the captured data using the specified protocol. Some protocols may need a special order for the pins given to the --capture option.\n");	
	printf("\t -s|--save file : save captured data to binary file.\n");
	printf("\t -p|--plot file : plot captured data to PNG file.\n");
	printf("\t -w|--wait sec : time in second to wait before the capture.\n");
	printf("\t -v|--verbose level: set the verbosity to level (0 to 4, higher means more messages,default is 3).\n");
}


int main(int argc, char **argv)
{
	opterr = 0;
    int option_index = 0;
	float wait = 0;
	float interval =0.5;
	uint8_t pins[256];
	uint8_t npin=0;
	uint32_t npoint = 0;
	char loadfile[256] = "";
	char savefile[256] = "";
	char plotfile[256] = "";
	char protocol_name[25] = "";
	int c;
	static int save_flag=0,wait_flag=0,help_flag=0,plot_flag=0;
	static int load_flag=0,capture_flag=0,decode_flag=0;
	char prog_name[256] = "";
    protocol* capture=NULL;


	strncpy(prog_name,argv[0],256);
 	while (1)
	{
		static struct option long_options[] =
		{
			{"save", required_argument,  0, 's'},
			{"wait", required_argument, 0, 'w'},
			{"verbose", required_argument, 0, 'v'},
			{"help", no_argument, 0, 'h'},
			{"load", required_argument, 0, 'l'},
			{"capture", required_argument, 0, 'c'},
			{"interval", required_argument, 0, 'i'},
			{"npoint", required_argument, 0, 'n'},
			{"decode", required_argument,  0, 'd'},
			{"plot", required_argument,  0, 'p'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
	 
		c = getopt_long (argc, argv, "hv:w:l:c:i:n:d:s:p:",long_options, &option_index);
	 
		if (c == -1)
			break;

		switch (c)
		{
			case 'h': //help
				help_flag = 1;
				break;
			case 'v': //verbose
				msglevel=(debug_level) atoi(optarg);
				break;
			case 'w': //wait
				wait_flag = 1;
				wait=atof(optarg);
				break;
			case 'l': //load
				load_flag = 1;
				strncpy(loadfile,optarg,256);
				break;
			case 'c': //capture
				capture_flag = 1;
				npin=0;
				for (const char* p = strtok(optarg, ", "); p ;p=strtok(NULL, ", "))
				{
				    pins[npin]=atoi(p);
				    npin++;
				    if(npin>=256)
				        break;
				}
				break;
			case 'i': //interval
				interval = atof(optarg);
				break;
			case 'n': //npoint
				npoint= atoi(optarg);
				break;
			case 'd': //decode
				decode_flag=1;
				strncpy(protocol_name,optarg,25);
				break;
			case 's': //save
				save_flag=1;
				strncpy(loadfile,optarg,256);
				break;
			case 'p': //plot
				plot_flag=1;
				strncpy(plotfile,optarg,256);
				break;
			case '?': 
			    /*getopt_long already printed an error message*/
				break;
			default:
				usage(prog_name);
				return 1;
		}
	}
	for (int index = option_index; index < argc; index++)
		pmesg(WARNING,"Non-option argument %s\n", argv[index]);

	if(help_flag)
	{
		usage(prog_name);
		return 1;
	}

//Check for incompatibilities in paresed options
	if(!load_flag && !capture_flag)
	{
		usage(prog_name);
		pmesg(CRITICAL,"%s: no action given (--load or --capture needed).\n",prog_name);
		return 1;
	}
	if(load_flag && capture_flag)
	{
		usage(prog_name);
		pmesg(CRITICAL,"--load and --capture can t be used at the same time\n");
		return 1;
	}
//do what has been required	
    if(wait_flag)
    {
        if(wait>=1)
            bcm2835_delay(wait);
        if(wait<1 && wait>=0.001)
            bcm2835_delayMicroseconds(wait*1000);        
    }
    if( !decode_flag )
    {
        pmesg(INFORMATION,"no decoding, creating raw object...\n");
        capture=new raw();
    }
    else if(strcmp(protocol_name,"raw")==0)
    {
        pmesg(INFORMATION,"%s decoding, creating raw object...\n",protocol_name);
        capture=new raw();
    }
    else if(strcmp(protocol_name,"microwire")==0)
    {
        pmesg(INFORMATION,"%s decoding, creating microwire object...\n",protocol_name);
        capture=new microwire();
    }
    else
    {
        usage(prog_name);
	    pmesg(CRITICAL,"unknown protocol \"%s\".\n",protocol_name);
	    return 1;
	}
    
    if(capture_flag)
    {
        pmesg(INFORMATION,"init acquisition...\n");
        capture->init_acquisition(pins,npin);
        pmesg(INFORMATION,"acquire...\n");
        capture->acquire(npoint,interval);
    }
    if(load_flag)
    {
        pmesg(INFORMATION,"Load...\n");
        capture->Load(loadfile);
    }
    if(save_flag)
    {
        pmesg(INFORMATION,"Save...\n");
        capture->Save(savefile);
    }
    if(decode_flag)
    {
        pmesg(INFORMATION,"decode...\n");
        capture->decode();
    }
    if(plot_flag)
    {
        pmesg(INFORMATION,"create graph window...\n");
        mglGraphZB gr(1280,1024);
        pmesg(INFORMATION,"plot...\n");
        capture->Draw(&gr);
        pmesg(INFORMATION,"Write PNG\n");
        gr.WritePNG(plotfile);
    }
    if(capture!=NULL)
        delete capture;
    return 0;
}
