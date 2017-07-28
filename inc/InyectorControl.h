/*
 *  \file       oven.h
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
/* --------------------------------- Module -------------------------------- */
#ifndef __INYECTORCONTROL_H__
#define __INYECTORCONTROL_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define START_TIME          RKH_TIME_SEC(2)
#define START_DUTY          50      /* In percentage */
#define IDLE_MIN_DUTY       20      /* In percentage */
#define IDLE_MAX_DUTY       30      /* In percentage */
#define THROTTLE_MIN        30      /* Throttle pedal position */
#define THROTTLE_MAX        80      /* Throttle pedal position */
#define IDLE_RPM            2000    /* In RPM */
#define IDLE_RPM_THH        20      /* In RPM */
#define IDLE_RPM_THL        20      /* In RPM */
#define INC_DUTY_FOR_COLD   10      /* In percentage */
#define TICK_TIME           100     /* In milliseconds */
#define ENGINE_MIN_TEMP     70      /* In Celcius */

#define UNHANDLED_EVENT     (-1)
#define false               0
#define true                1

/* ================================ Signals ================================ */
typedef enum InyectorControl_sigs_t
{
    evStart,
    evStartTimeout,
    evTick,
    evTerm,   /* to close application in x86 */
} IC_SIGS_T;

/* ======================== Declares active object ========================= */
RKH_SMA_DCLR(inyectorControl);

/* =================== Declares states and pseudostates ==================== */
RKH_DCLR_BASIC_STATE off, starting, idleSpeed, normal;

/* ------------------------------- Data types ------------------------------ */
typedef struct InyectorControl InyectorControl;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */

