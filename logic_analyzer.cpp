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
    microwire test(pins,4);
    mglGraphZB gr(1200,800);
    test.Draw(&gr);
    return 1;
}
