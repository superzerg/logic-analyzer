//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <mgl/mgl_fltk.h>
#include <mgl/mgl_zb.h>
//#include <unistd.h>

#ifndef BINARY
#define BINARY
#include "binary.h"
#endif

//! Class to handle decoded information
class message: public mglDraw
{
public:
    //! Constructor
    /*! call init() if data is not NULL
        \sa init()*/
    message(const binary *data=NULL,const char label[30]="",float t0=0,float tmessage=0,char color='w');
    //! Initialize the message from parameters: 
    /*!     \param data object representing the message (subset of acquired binary data).
            \param label string representing the message (Drawn string).
            \param t0 acquisition time in seconds of the beginig of the message.
            \param tmessage duration of the message in seconds.
            \param color char giving the color of the message.
        this->pin and this->npin are taken from data
        \sa binary*/
    void init(const binary *data,const char label[30]="",float t0=0,float tmessage=0,char color='w');
    //! Draw the message
    /*!     \param gr pointer to a mglGraph object
        The subplot on which the message belongs to is taken from this->pin*/
    int Draw(mglGraph *gr);
//private:
    //! object representing the message (subset of acquired binary data).
    binary data;
    //! subplot of the message
    uint8_t pin;
    //! total number of subplot
    uint8_t npin;
    //! string representing the message (Drawn string).
    char label[30];
    //! acquisition time when message started.
    float t0;
    //! durration of the message.
    float tmessage;
    //! color of the message.
    char color;
};
