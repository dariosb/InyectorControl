/** 
 * \file InyectorControlAct.h 
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.03.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina  dariosb@gmail.com
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
void InyectorControlAct_init(InyectorControl *const me); 

/* Effect actions */
void InyectorControlAct_onIdleSpeed(InyectorControl *const me);
void InyectorControlAct_onNormal(InyectorControl *const me);

/* Guard actions */
rbool_t InyectorControlAct_isPressedThrottle(InyectorControl *const me);
rbool_t InyectorControlAct_isReleasedThrottle(InyectorControl *const me);

/* Entry actions */
void InyectorControlAct_starting(InyectorControl *const me);

/* Exit actions */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
