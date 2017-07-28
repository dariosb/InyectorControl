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
#include "bsp.h"

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
                0, FLAT, &idle, InyectorControlAct_init, NULL);
RKH_SMA_DEF_PTR(inyectorControl);

/* ---------------------------- Local variables ---------------------------- */
/*
 *  Declare and allocate the 'e_tout' event.
 *  The 'e_tout' event with TIMEOUT signal never changes, so it can be
 *  statically allocated just once by means of RKH_ROM_STATIC_EVENT() macro.
 */
static RKH_ROM_STATIC_EVENT(e_tout, TMREVT);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ============================ Initial action ============================= */
void
oven_init(InyectorControl *const me)
{
    bsp_ovenInit();

    /* send objects to trazer */
    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_STATE(me, &idle);
    RKH_TR_FWK_STATE(me, &ready);
    RKH_TR_FWK_STATE(me, &cooking);
    RKH_TR_FWK_OBJ(&me->timer);
    RKH_TR_FWK_FUN(&oven_init);
    RKH_TR_FWK_FUN(&cook_start);
    RKH_TR_FWK_FUN(&cook_stop);

    /* send signals to trazer */
    RKH_TR_FWK_SIG(OPEN);
    RKH_TR_FWK_SIG(CLOSE);
    RKH_TR_FWK_SIG(START);
    RKH_TR_FWK_SIG(TMREVT);
    RKH_TR_FWK_SIG(TERM);

    RKH_TMR_INIT(&me->timer, &e_tout, NULL);
}

/* ============================ Effect actions ============================= */
/* ============================= Entry actions ============================= */
void
cook_start(Oven *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), COOK_TIME);
    bsp_emitterOn();
}

/* ============================= Exit actions ============================== */
void
cook_stop(Oven *const me, RKH_EVT_T *pe)
{
    (void)pe;

    rkh_tmr_stop(&me->timer);
    bsp_emitterOff();
}

/* ================================ Guards ================================= */
/* ------------------------------ End of file ------------------------------ */
