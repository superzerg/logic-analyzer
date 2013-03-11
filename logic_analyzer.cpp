//Compile with :
//make
//Tested with (look at ): 
//make test
//To look to the resulting image:
//make view

#include <mgl/mgl_zb.h>
#include "microwire.h"

int main(int argc, char **argv)
{
    uint8_t pins[4]={15,14,4,27};
    microwire capture,loaded;
    pmesg(INFORMATION,"init acquisition...\n");
    capture.init_acquisition(pins,4);
    pmesg(INFORMATION,"acquire...\n");
    capture.acquire(3000,0.5);
    pmesg(INFORMATION,"Save...\n");
    capture.Save("test.dat");
    pmesg(INFORMATION,"Load...\n");
    loaded.Load("test.dat");
    pmesg(INFORMATION,"decode...\n");
    loaded.decode();
    pmesg(INFORMATION,"create graph window...\n");
    mglGraphZB gr(1280,1024);
    pmesg(INFORMATION,"plot...\n");
    loaded.Draw(&gr);
    pmesg(INFORMATION,"Write PNG\n");
    gr.WritePNG("test.png");
    return 1;
}
