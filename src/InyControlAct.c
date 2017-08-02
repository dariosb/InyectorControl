/** 
 *  \file       InyControlAct.c 
 *  \brief      Inyector Control RKH SMA actions.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.07.07  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  Daba  Darío Baliña       dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "InyControl.h"
#include "InyControlEvt.h"
#include "InyControlAct.h"
#include "RPMControl.h"
#include "TempSensor.h"
#include "RPMSensor.h"
#include "ThrottleSensor.h"
#include "PWMInyector.h"
#include "Sensor.h"

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

/* ----------------------- Local function prototypes ----------------------- */
static int
calcDutyFromThrottle(int currThrottleVal)
{
    return currThrottleVal;
}

/* ---------------------------- Global functions --------------------------- */
/* Init action */
void 
InyControlAct_init(InyControl *const me)
{
    /* send objects to trazer */
    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_STATE(me, &off);
    RKH_TR_FWK_STATE(me, &starting);
    RKH_TR_FWK_STATE(me, &normal);
    RKH_TR_FWK_OBJ(&me->timer);
    RKH_TR_FWK_FUN(&InyControlAct_init);
    RKH_TR_FWK_FUN(&InyControlAct_onIdleSpeed);
    RKH_TR_FWK_FUN(&InyControlAct_onNormal);
    RKH_TR_FWK_FUN(&InyControlAct_isPressedThrottle);
    RKH_TR_FWK_FUN(&InyControlAct_isReleasedThrottle);
    RKH_TR_FWK_FUN(&InyControlAct_starting);

    /* send signals to trazer */
    RKH_TR_FWK_SIG(evStart);
    RKH_TR_FWK_SIG(evStartTimeout);
    RKH_TR_FWK_SIG(evTick);
    RKH_TR_FWK_SIG(evTerm);

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
InyControlAct_onIdleSpeed(InyControl *const me)
{
    tempVal = Sensor_get((Sensor *)temp);
    rpmVal = Sensor_get((Sensor *)rpm);

    duty = RPMControl_compute(rpmVal);
    PWMInyector_setDuty(duty);
}

void 
InyControlAct_onNormal(InyControl *const me)
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
rbool_t  
InyControlAct_isPressThrottle(InyControl *const me)
{
    throttleVal = Sensor_get((Sensor *)throttle);
    return (throttleVal > THROTTLE_MIN) ? RKH_TRUE : RKH_FALSE;
}

rbool_t 
InyControlAct_isRelThrottle(InyControl *const me)
{
    throttleVal  = Sensor_get((Sensor *)throttle);
    return (throttleVal <= THROTTLE_MIN) ? RKH_TRUE : RKH_FALSE;
}


/* Entry actions */
void 
InyControlAct_starting(InyControl *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), START_TIME);
    PWMInyector_setDuty(START_DUTY);
}


/* Exit actions */
/* ------------------------------ File footer ------------------------------ */
