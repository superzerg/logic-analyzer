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

#ifndef ACTIVITY
#define ACTIVITY
#include "activity.h"
#endif

//! class getting LTH (Low To High) and HTL (High To Low) transitions in logic_input
class transition: public mglDraw  //clock transitions
{
public:
    //! Constructor
    /*! Calls init() if data is not NULL.\sa init()*/
    transition(logic_input *data=NULL,uint8_t pin_clock=1,activity *cs=NULL);
    //! Initialization
    /*!     \param data logic_input source. if not NULL call find_transition().
            \param pin_clock index of the pin of the clock
            \param cs pointer to activity. If not NULL GetStats() is called.
        \sa GetStats() find_transition()*/
    void init(logic_input *data,uint8_t pin_clock,activity *cs=NULL);
    //! Destructor
    ~transition();
    //! Get statistics on transition period (min, average and max)
    /*!     \param cs pointer to an activity. Inactive periods are discarded.
            \param sign sign of the transition:
                - \'u\' for getting stats about LTH transitions.
                - \'d\' for getting stats about HTL transitions.*/
    float GetStats(activity *cs,char sign='u');
    //! find the transitions from a logic_input object
    /*!     \param data logic_input source.
            \param sign sign of the transition:
                - \'u\' for getting stats about LTH transitions.
                - \'d\' for getting stats about HTL transitions.*/
    void find_transition(logic_input *data,char sign='u');
    //! Draw statistics on period transitions
    /*!     \param gr pointer to a mglData object*/
    int Draw(mglGraph *gr);
    //! Draw vertical arrows at same times as transitions.
    /*!     \param gr pointer to a mglGraph object.
            \param sign sign of the transition:
                - \'u\' for getting stats about LTH transitions.
                - \'d\' for getting stats about HTL transitions.
            \param subplot subplot index where to draw.*/
    int Draw(mglGraph *gr,char sign,uint8_t subplot);
//private:
    //! number of LTH transitions
    uint32_t ntransition_up;
    //! array of index of logic_input measurements corresponding to all LTH transitions.
    uint32_t *index_transition_up;
    //! mglData object containing acquisition time corresponding to all LTH transitions.
    mglData t_transition_up;
    //! number of HTL transitions
    uint32_t ntransition_down;
    //! array of index of logic_input measurements corresponding to all HTL transitions.
    uint32_t *index_transition_down;
    //! mglData object containing acquisition time corresponding to all HTL transitions.
    mglData t_transition_down;
    //! number of pin used during acquisition.
    uint8_t npin;
    //! index of the pin of the clock
    uint8_t pin_clock;
    //! mean period of the clock measured 
    float period;
    //! min period of the clock measured 
    float period_min;
    //! max period of the clock measured 
    float period_max;
    //! time in s where to center the statistic display
    /*! initialized at the middle of the first active period*/
    float t_first_activity;
};
