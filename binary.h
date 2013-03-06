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

//! Class containing bits information obtained from logic_input and transition.

class binary : public mglDraw
{
public:
    //! Constructor
    binary(uint32_t nbit=0);
    //! Default copy constructor, it allows to select a sub-range of the source object
    /*!     \param source source binary object
            \param index_start inclusive index to start copy from
            \param index_end inclusive index where to stop the copy.
                If index_end=0 or omitted, copy 1 bit only.*/
    binary(const binary &source, uint32_t index_start=0,  uint32_t index_end=0);
    //! Copy constructor allowing to select a sub-range in time of the source object
    /*!     \param source source binary object
            \param t_start time to start copy from
            \param t_end time when to stop the copy.
                If t_end=0 or omitted, copy until the end.*/
    binary(const binary &source, float t_start,  float t_end=0);
    //! Destructor
    ~binary();
    //! Initialize from an logic_input object data
    /*! binary values are got from data at pin pin, when the specified transition occurs.
            \param data pointer to the logic_input object
            \param clk pointer to the transition object
            \param pin index of the data where to find binary values
            \param transition_direction 'u' for low to high transitions, 'd' for the high to low transitions*/
    void init(logic_input *data, transition *clk, uint8_t pin, char transition_direction='u');
    //! Return bad reading number on an interval.
    /*!     \param index_start inclusive index to start looking for bad readings.
            \param index_end inclusive index to stop looking for bad readings.
            \return bad reading number (number of HIGHR or ERR bits).*/
    uint32_t Get_nbad(uint32_t index_start, uint32_t index_end=0);
    //! Return an uint32_t the value between inclusive bits index_start and index_end (as most significant bit first).
    /*!     \param index_start first bit to read (inclusive).
            \param index_end last bit to read (inclusive).
                If index_end=0 or omitted, only 1 bit is read.
            \return value between inclusive bits index_start and index_end (as most significant bit first).*/
    uint32_t Get(uint32_t index_start, uint32_t index_end=0);
    //! Copy the logic value of a bit in string.
    /*!     \param value 2 chars receiving the logic value:
                - \"0\" for low voltage (<1V when pull up and down used).
                - \"1\" for high voltage (>2V when pull up and down used).
                - \" \" for high resistance (<1V when pull down and >2V when pull up)
                - \"E\" for error (>2V when pull down and <1V when pull up). Abnormal state due to a transition between both measurements.
                - \"?\" error from the software, report if seen.
            \param bit index of the bit*/
    void sprint(char value[2], uint32_t bit);
    //! Display each bit value
    /*!     \param gr pointer to mglGraph object
            \return execution status (0 if OK)*/
    int Draw(mglGraph *gr);
    //! Assignment operator
    /*Checks for self assignment*/
    binary& operator = (const binary source)
    {
        if(this!=&source)
        {
            this->nbit=source.nbit;
            this->npin=source.npin;
            this->t=source.t;
            if(source.nbit>0)
            {
                this->bits=new uint8_t[this->nbit];
                for (uint32_t bit=0;bit<this->nbit;bit++)
                {
                    this->bits[bit]=source.bits[bit];
                }
            }
        }
        return *this;
    }
//private:
    //! Array of bits
    uint8_t *bits;
    //! mglData object containing the acquisition time of each bit
    mglData t;
    //! number of bits
    uint32_t nbit;
    //! number of pins used for acquisition
    /*! used to know the number of subplots*/
    uint8_t npin;
    //! pin where the binary data has been captured
    /*! it is the subplot where the binary data will be*/
    uint8_t pin;
};
