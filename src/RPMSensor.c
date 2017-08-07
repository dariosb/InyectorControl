/**
 *  \file       RPMSensor.c
 *  \brief      RPM Sensor
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
#include "RPMSensor.h"
#include "Sensor.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct RPMSensor
{
    Sensor sensor;
}RPMSensor;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static int RPMSensor_get(Sensor *const me);
static const SensorVtbl vtbl = { RPMSensor_get };
static RPMSensor rpm = { { &vtbl, "RPM", SensorRPM, 0 } };

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
RPMSensor_get(Sensor *const me)
{
    rpm.sensor.value = bsp_RPMSensorRead();
    return rpm.sensor.value;
}

/* ---------------------------- Global functions --------------------------- */
RPMSensor *
RPMSensor_init(void)
{
    return &rpm;
}

/* ------------------------------ End of file ------------------------------ */

