/**
 *  \file       TempSensor.c
 *  \brief      Temperature Sensor
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
#include "TempSensor.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct TempSensor
{
    int value;
}TempSensor;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static TempSensor tempSensor;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
TempSensor *TempSensor_init(void)
{
    return &tempSensor; 
}

/* ------------------------------ End of file ------------------------------ */


