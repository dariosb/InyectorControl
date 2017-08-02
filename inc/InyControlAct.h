/** 
 *  \file       InyControlAct.h 
 *  \brief      Inyector Control RKH SMA actions.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.07.07  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina  dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __INYCONTROLACT_H__
#define __INYCONTROLACT_H__
 
/* ----------------------------- Include files ----------------------------- */
#include "InyControl.h"

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
void InyControlAct_init(InyControl *const me); 

/* Effect actions */
void InyControlAct_onIdleSpeed(InyControl *const me);
void InyControlAct_onNormal(InyControl *const me);

/* Guard actions */
rbool_t InyControlAct_isPressThrottle(InyControl *const me);
rbool_t InyControlAct_isRelThrottle(InyControl *const me);

/* Entry actions */
void InyControlAct_starting(InyControl *const me);

/* Exit actions */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
