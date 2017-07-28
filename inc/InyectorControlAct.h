/** 
 * \file InyectorControlAct.h 
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
/* --------------------------------- Module -------------------------------- */
#ifndef __INYECTORCONTROLACT_H__
#define __INYECTORCONTROLACT_H__
 
/* ----------------------------- Include files ----------------------------- */
#include "InyectorControl.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */

/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* Init action */
void InyectorControlAct_init(InyectorControl *const me) 

/* Effect actions */
void InyectorControlAct_onIdleSpeed(InyectorControl *const me, RKH_EVT_T *event);
void InyectorControlAct_onNormal(InyectorControl *const me, RKH_EVT_T *event);

/* Guard actions */
bool InyectorControlAct_isPressedThrottle(InyectorControl *const me, RKH_EVT_T *event);
bool InyectorControlAct_isReleasedThrottle(InyectorControl *const me, RKH_EVT_T *event);

/* Entry actions */
void InyectorControlAct_starting(InyectorControl *const me, RKH_EVT_T *event);

/* Exit actions */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
