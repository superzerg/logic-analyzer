#include <time.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <bcm2835.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>

typedef enum
{
    LOWV = 0,
    HIGHR = 1,
    ERROR = 2,
    HIGHV = 3,
}logic_state;

//! Class doing aquisition from RPI's GPIO.
/*! Saving to and loading from a file to be added.*/
class logic_input : public mglDraw
{
public:
    //! Constructor.
    /*! Call init_acquisition and acquire if the corresponding parrameters are set.*/ 
    logic_input(uint8_t npin=0,uint8_t * pins=NULL,uint32_t npoint=0,float delay=0);
    //! Initialization of the object for the capture
    /*!     \param pins array of the GPIO  pin number (i for GPIOi) to be used durring acquisition.
            \param npin number of pins to be used durring acquisition. Max value is pins array size.*/
    void init_acquisition(uint8_t * pins, uint8_t npin=0);
    //! Start the acquisition.
    /*!     \param npoint number of logic values to acquire (1 point is 2 GPIO measurements: one with pull up and one with pull down).
            \param delay time in ms to wait between 2 point aquisitionb.*/ 
    void acquire(uint32_t npoint=0,float delay=0);
    //! Destructor
    ~logic_input();
    //! Plot logic values acquired
    /*!     \param gr pointer to a mglGraph object
            \param label array of strings giving label of each pin. */
    int Draw(mglGraph *gr,const char *label[]=NULL);
//private:
    //! Number of logic values aquired
    uint32_t npoint;
    //! Number of pin used in acquisition
    uint8_t npin;
    //! Array of GPIO number, eachone is used durring acquisition 
    uint8_t *pins;
    //! Array of mglData vector.
    /*! Size of the array is npin, each vector contains the npoint logic value measurements for the coresponding pin*/
    mglData *rawdata;
    //! mglData vector of the time of each aquisition.
    /*! contains npoint acquisition time in s.*/
    mglData t;
};

