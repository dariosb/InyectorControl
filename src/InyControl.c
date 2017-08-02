/**
 *  \file       InyControl.c
 *  \brief      Inyector Control RKH SMA.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.07.07  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Darío Baliña  dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "InyControl.h"
#include "InyControlAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ======================== States and pseudostates ======================== */
RKH_CREATE_BASIC_STATE(off, NULL, NULL,  RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(off)
    RKH_TRREG(evStart,   NULL,   NULL,   &starting),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(starting, InyControlAct_starting, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(starting)
    RKH_TRREG(evStartTimeout, NULL, NULL, &idleSpeed),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(idleSpeed, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(idleSpeed)
    RKH_TRINT(evTick, InyControlAct_isRelThrottle, InyControlAct_onIdleSpeed),
    RKH_TRREG(evTick, NULL, NULL, &normal),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(normal, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(normal)
    RKH_TRINT(evTick, InyControlAct_isPressThrottle, InyControlAct_onNormal),
    RKH_TRREG(evTick, NULL, NULL, &idleSpeed),
RKH_END_TRANS_TABLE

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ============================= Active object ============================= */
RKH_SMA_CREATE(InyControl, inyControl, 0, HCAL, &off, InyControlAct_init, NULL);
RKH_SMA_DEF_PTR(inyControl);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ============================ Initial action ============================= */
/* ------------------------------ End of file ------------------------------ */
