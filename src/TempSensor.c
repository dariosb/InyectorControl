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
#include "Sensor.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct TempSensor
{
    Sensor sensor;
}TempSensor;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
TempSensor_get(Sensor *const me)
{
    return me->value = bsp_TempSensorRead();
}

/* ---------------------------- Global functions --------------------------- */
TempSensor *TempSensor_init(void)
{
	static const SensorVtbl vtbl = { TempSensor_get };
	static TempSensor temperature= { { &vtbl, "Temperature", SensorTemp, 0 } };

    return &temperature; 
}

/* ------------------------------ End of file ------------------------------ */


