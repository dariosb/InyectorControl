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
static int TempSensor_get(Sensor *const me);
static const SensorVtbl vtbl = { TempSensor_get };
static TempSensor temperature=
{
    { &vtbl, "Temperature", SensorTemp, 0 } 
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int
TempSensor_get(Sensor *const me)
{
    temperature.sensor.value = bsp_TempSensorRead();
    return temperature.sensor.value;
}

/* ---------------------------- Global functions --------------------------- */
TempSensor *TempSensor_init(void)
{
    return &temperature; 
}

/* ------------------------------ End of file ------------------------------ */


