//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>

#ifndef LOGIC_INPUT
#define LOGIC_INPUT
#include "logic_input.h"
#endif

//! Class containing activity obtained from logic_input
/*! Class containing time range when ship is active and able to draw the background of  subplots accordingly*/

class activity: public mglDraw
{
public:
    //! Constructor from a logic_input object data
    /*! Calls init if data is not NULL*/
    activity(logic_input *data=NULL,uint8_t pin=0, logic_state=HIGHV);
    /*!     \param data logic_input object
            \param pin pin index corresponding to Chip Select 
            \param active_value value when the Chip is Enabled (HIGHV or LOWV)*/
    void init(logic_input *data,uint8_t pin=0, logic_state=HIGHV);
    //!Destructor
    ~activity();
    //! Paint background of a subplot according to activity
    /*!     \param gr mglGraph object
            \param subplot index of the subplot*/
    int Draw(mglGraph *gr,uint8_t subplot);
//private:
    //! Number of active intervals
    uint32_t nactive;
    //! array of starting time of each active period
    float *t_start;
    //! array of ending time of each active period
    float *t_end;
    //! number of pin acquired
    /*! needed to know subplot number*/
    uint8_t npin;
};
