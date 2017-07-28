/** 
 * \file InyectorControlAct.c 
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2016.03.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 *  Daba  Darío Baliña       dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "InyectorControl.h"
#include "RPMControl.h"
#include "TempSensor.h"
#include "RPMSensor.h"
#include "ThrottleSensor.h"
#include "PWMInyector.h"
#include "Sensor.h"
#include "Timer.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static int tempVal, rpmVal, throttleVal;
static TempSensor *temp;
static RPMSensor *rpm;
static ThrottleSensor *throttle;
static unsigned char duty;
static RKH_ROM_STATIC_EVENT(e_StartTimeout, evStartTimeout);

/* ----------------------- Local function prototypes ----------------------- */
static int
calcDutyFromThrottle(int currThrottleVal)
{
    return currThrottleVal;
}

/* ---------------------------- Global functions --------------------------- */
/* Init action */
void 
InyectorControlAct_init(InyectorControl *const me)
{
    bsp_ovenInit();

    /* send objects to trazer */
    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_STATE(me, &off);
    RKH_TR_FWK_STATE(me, &starting);
    RKH_TR_FWK_STATE(me, &normal);
    RKH_TR_FWK_OBJ(&me->timer);
    RKH_TR_FWK_FUN(&InyectorControlAct_init);
    RKH_TR_FWK_FUN(&InyectorControlAct_onIdleSpeed);
    RKH_TR_FWK_FUN(&InyectorControlAct_onNormal);
    RKH_TR_FWK_FUN(&InyectorControlAct_isPressedThrottle);
    RKH_TR_FWK_FUN(&InyectorControlAct_isReleasedThrottle);
    RKH_TR_FWK_FUN(&InyectorControlAct_starting);

    /* send signals to trazer */
    RKH_TR_FWK_SIG(evStart);
    RKH_TR_FWK_SIG(evStartTimeout);
    RKH_TR_FWK_SIG(evTick);
    RKH_TR_FWK_SIG(evTerm);

    RKH_TMR_INIT(&me->timer, &e_StartTimeout, NULL);



    tempVal = rpmVal = throttleVal = 0;
    duty = 0;

    temp = TempSensor_init();
    rpm = RPMSensor_init();
    throttle = ThrottleSensor_init();
    RPMControl_init(IDLE_MIN_DUTY, IDLE_MAX_DUTY, IDLE_RPM, IDLE_RPM_THH, 
                    IDLE_RPM_THL);
    PWMInyector_init();
    RKH_TMR_INIT(&me->timer, &e_StartTimeout, NULL);
}

/* Effect actions */
void 
InyectorControlAct_onIdleSpeed(InyectorControl *const me, RKH_EVT_T *event)
{
    tempVal = Sensor_get((Sensor *)temp);
    rpmVal = Sensor_get((Sensor *)rpm);

    duty = RPMControl_compute(rpmVal);
    PWMInyector_setDuty(duty);
}

void 
InyectorControlAct_onNormal(InyectorControl *const me, RKH_EVT_T *event)
{
    tempVal = Sensor_get((Sensor *)temp);
    rpmVal = Sensor_get((Sensor *)rpm);

    duty = calcDutyFromThrottle(throttleVal);

    if (tempVal < ENGINE_MIN_TEMP)
    {
        duty += INC_DUTY_FOR_COLD;
    }

    PWMInyector_setDuty(duty);
}


/* Guard actions */
bool 
InyectorControlAct_isPressedThrottle(InyectorControl *const me, RKH_EVT_T *event)
{
    throttleVal = Sensor_get((Sensor *)throttle);
    return throttleVal > THROTTLE_MIN;
}

bool 
InyectorControlAct_isReleasedThrottle(InyectorControl *const me, RKH_EVT_T *event)
{
    throttleVal  = Sensor_get((Sensor *)throttle);
    return throttleVal <= THROTTLE_MIN;
}


/* Entry actions */
void 
InyectorControlAct_starting(InyectorControl *const me, RKH_EVT_T *event)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), START_TIME);
    PWMInyector_setDuty(START_DUTY);
}


/* Exit actions */
/* ------------------------------ File footer ------------------------------ */
