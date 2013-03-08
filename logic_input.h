#include <time.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <bcm2835.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>

#ifndef DEBUG_H
#define DEBUG_H
#include "debug.h"
#endif

typedef enum
{
    LOWV = 0,
    HIGHR = 1,
    ERR = 2,
    HIGHV = 3,
}logic_state;

//! Class doing acquisition from RPI's GPIO.
/*! Saving to a file and loading from a file to be added.*/
class logic_input : public mglDraw
{
public:
    //! Constructor.
    /*! Call init_acquisition and acquire if the corresponding parameters are set.*/ 
    logic_input(uint8_t npin=0,uint8_t * pins=NULL,uint32_t npoint=0,float delay=0);
    //! Initialisation of the object for the capture
    /*!     \param pins array of the GPIO  pin numbers (ex {9,10} for GPIO9 and GPIO10) to be used during acquisition.
            \param npin number of pins to be used during acquisition. Max value is pins array size.*/
    void init_acquisition(uint8_t * pins, uint8_t npin=0);
    //! Start the acquisition.
    /*!     \param npoint number of logic values to acquire (1 point is 2 GPIO measurements: one with pull up and one with pull down).
            \param delay time in ms to wait between 2 point acquisition.*/ 
    void acquire(uint32_t npoint=0,float delay=0);
    //! Destructor
    ~logic_input();
    //! Plot logic values acquired
    /*!     \param gr pointer to a mglGraph object
            \param label array of strings giving label of each pin. 
            \param pin_stats subplot where to print stats about acquisition.*/
    int Draw(mglGraph *gr,const char *label[]=NULL, uint8_t pin_stats=0);
    //! Save the object into a file
    /*!     \param filename string contenaning filename to be saved.*/
    const void Save(const char *filename);
    //! Load the object into a file
    /*!     \param filename string contenaning filename to be loaded.*/
    void Load(const char *filename);
    //! Clean the object
    void Clean();

//private:
    //! Number of logic values acquired
    uint32_t npoint;
    //! Number of pin used in acquisition
    uint8_t npin;
    //! Array of GPIO number, each one is used during acquisition 
    uint8_t *pins;
    //! Array of mglData vector.
    /*! Size of the array is npin, each vector contains the npoint logic value measurements for the co responding pin*/
    mglData *rawdata;
    //! mglData vector of the time of each acquisition.
    /*! contains npoint acquisition time in s.*/
    mglData t;
};

