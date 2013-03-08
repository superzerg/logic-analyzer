#include <mgl/mgl_zb.h>
//#include <unistd.h>


#ifndef LOGIC_INPUT
#define LOGIC_INPUT
#include "logic_input.h"
#endif

#ifndef TRANSITION
#define TRANSITION
#include "transition.h"
#endif

#ifndef BINARY
#define BINARY
#include "binary.h"
#endif

#ifndef MESSAGE
#define MESSAGE
#include "message.h"
#endif

#ifndef ACTIVITY
#define ACTIVITY
#include "activity.h"
#endif

#ifndef DEBUG_H
#define DEBUG_H
#include "debug.h"
#endif

//! protocol class, parent of all protocols.
 class protocol: public mglDraw
{
public:
    //! Pure virtual destructor. 
    /*! Need to be defined as protocol classes should contain at least one dynamic array of messages.*/
    virtual ~protocol()=0;
    //! Initialize acquisition. May not be redeclared.
    /*! Setup capture on the GPIO pins given.
            \param pins array of GPIO pin used in the capture such as:
                - pins[0] acquires CS (Chip Select)
                - pins[1] acquires CLK (the clock)
                - pins[2] acquires MOSI (Master Out Slave In)
                - pins[3] acquires MISO (Master In Slave Out)
                - any pin>3 will not be decoded but will be acquired and displayed.
            \param npin number of pin to acquire (must be smaller or equal to the size of pins)*/
    virtual void init_acquisition(uint8_t pins[],uint8_t npin);
    //! Start acquisition. May not be redeclared.
    /*!     \param npoint number of points to acquire.
            \param period wait time in ms between 2 measurements.*/
    virtual void acquire(uint32_t npoint=2000, float period=1.0);
    //! Perform all the decoding. Need to be defined as it will depend on the protocol
    virtual void decode(void)=0;
    //! Draw all.  Need to be defined as it will depend on the protocol
    /*!     \param gr pointer to mglGraph object*/
    virtual int Draw(mglGraph *gr)=0;
//private:
    //! Contain the raw capture of all pins
    logic_input capture;
    /*! Add as many activity, transition and binary member objects as needed (each one should be associated with a pin index in the decode() function).
        For each binary member object, a dynamic array of message should also be member. Example:
    activity cs;
    transition clk;
    binary mosi;
    message mosi_mess[];
    uint32_t nmessage;*/
};
