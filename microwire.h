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
//! Example class decoding instructions sent to a NMC9314 EEPROM memory (Microwire interface)
class microwire: public mglDraw
{
public:
    //! Constructor.
    /*! Setup and start capture on the GPIO pins given.
            \param pins array of GPIO pin used in the capture such as:
                - pins[0] acquires CS (Chip Select)
                - pins[1] acquires CLK (the clock)
                - pins[2] acquires MOSI (Master Out Slave In)
                - pins[3] acquires MISO (Master In Slave Out)
                - any pin>3 will not be decoded but will be acquired and displayed.
            \param npin number of pin to acquire (must be smaller or equal to the size of pins)*/
    microwire(uint8_t pins[],uint8_t npin=4);
    //! Destructor
    ~microwire();
    //! function extracting messages
    /*! Puts the extracted messages array in mosi_mess, from binary mosi and activity cs.*/
    void decode_mosi();
    //! function extracting messages
    /*! Puts the extracted messages array in miso_mess, from binary miso and activity cs.*/
    void decode_miso();
    //! Draw all messages
    /*!     \param gr pointer to mglGraph object*/
    int Draw(mglGraph *gr);
//private:
    //! Contain the raw capture of all pins
    logic_input capture;
    //! Contain the activity 
    /*! Determined from CS pin*/
    activity cs;
    //! Contain the clock transitions 
    /*! Determined from CLK pin*/
    transition clk;
    //! Contain the MOSI binary data 
    /*! Determined from MOSI pin, clock transitions and activity*/
    binary mosi;
    //! Contain the MISO binary data 
    /*! Determined from MISO pin, clock transitions and activity*/
    binary miso;
    //! array of message decoded from mosi \sa decode_mosi()
    message *mosi_mess;
    //! array of message decoded from miso \sa decode_miso()
    message *miso_mess;
    //! number of messages
    uint32_t nmessage;
};
