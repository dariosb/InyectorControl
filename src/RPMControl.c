/**
 *  \file       RPMControl.c
 *  \brief      RPM Control
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
/*
 * Emulated Lineal RPM control using y = m * x + n, where:
 *
 * y: RPM
 * x: Duty
 * m: delta
 * n: offset
 */

/* ----------------------------- Include files ----------------------------- */
#include "RPMControl.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
int delta, offset;
unsigned char maxDutyIdle, minDutyIdle;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
RPMControl_init(unsigned char minDuty, unsigned char maxDuty,
                     int targetRPM, int thHighRPM, int thLowRPM)
{
    maxDutyIdle = maxDuty;
    minDutyIdle = minDuty;
    delta = (thHighRPM + thLowRPM)/(minDuty - maxDuty); 
    offset = (targetRPM + thHighRPM)-(thLowRPM * delta);
}

unsigned char
RPMControl_compute(int currRPM)
{
    int dutty;

    dutty = (currRPM - offset) / delta;

    if(dutty > maxDutyIdle)
        return maxDutyIdle;

    if(dutty < minDutyIdle)
        return minDutyIdle;

    return (unsigned char)dutty;
}

/* ------------------------------ End of file ------------------------------ */

