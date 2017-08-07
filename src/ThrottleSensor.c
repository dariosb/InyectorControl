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
#include "Sensor.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct ThrottleSensor
{
    Sensor sensor;
}ThrottleSensor;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
ThrottleSensor_get(Sensor *const me)
{
    me->value = bsp_ThrottleSensorRead();
    return me->value;
}

/* ---------------------------- Global functions --------------------------- */
ThrottleSensor *
ThrottleSensor_init(void)
{
	static const SensorVtbl vtbl = { ThrottleSensor_get };
	static ThrottleSensor throttle = {{&vtbl, "Throttle", SensorThrottle, 0}};

    return &throttle;
}

/* ------------------------------ End of file ------------------------------ */

