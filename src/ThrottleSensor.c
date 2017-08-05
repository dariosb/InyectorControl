/**
 *  \file       ThrottleSensor.c
 *  \brief      Throttle Sensor
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
#include "ThrottleSensor.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct ThrottleSensor
{
    int value;
}ThrottleSensor;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static ThrottleSensor throttleSensor;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
ThrottleSensor *
ThrottleSensor_init(void)
{
    return &throttleSensor;
}

/* ------------------------------ End of file ------------------------------ */

