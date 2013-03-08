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
    microwire test;
    printf("init acquisition\n");
    test.init_acquisition(pins,4);
    printf("acquire...\n");
    test.acquire(3000,0.5);
    printf("decode...\n");
    test.decode();
    printf("create graph window\n");
    mglGraphZB gr(1280,1024);
    printf("plot\n");
    test.Draw(&gr);
    printf("Write PNG\n");
    gr.WritePNG("test.png");
    return 1;
}
