/**
 *  \file       InyectorControl.c
 *  \brief      Example application.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.03.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Darío Baliña  dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "InyectorControl.h"
#include "InyectorControlAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ======================== States and pseudostates ======================== */
RKH_CREATE_BASIC_STATE(off, NULL, NULL,  RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(off)
    RKH_TRREG(evStart,   NULL,   NULL,   &starting),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(starting, InyectorControlAct_starting, NULL, 
                                                        RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(starting)
    RKH_TRREG(evStartTimeout, NULL, NULL, &idleSpeed),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(idleSpeed, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(idleSpeed)
    RKH_TRINT(evTick, InyectorControlAct_isReleasedThrottle, 
                      InyectorControlAct_onIdleSpeed),
    RKH_TRREG(evTick, NULL, NULL, &normal),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(normal, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(normal)
    RKH_TRINT(evTick, InyectorControlAct_isPressedThrottle, 
                      InyectorControlAct_onNormal),
    RKH_TRREG(evTick, NULL, NULL, &idleSpeed),
RKH_END_TRANS_TABLE

/* ---------------------------- Local data types --------------------------- */
struct InyectorControl
{
    RKH_SMA_T sma;  /* base structure */
    RKH_TMR_T timer;
};

/* ---------------------------- Global variables --------------------------- */
/* ============================= Active object ============================= */
RKH_SMA_CREATE( InyectorControl, inyectorControl,
                0, FLAT, &off, InyectorControlAct_init, NULL);
RKH_SMA_DEF_PTR(inyectorControl);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ============================ Initial action ============================= */
/* ------------------------------ End of file ------------------------------ */
