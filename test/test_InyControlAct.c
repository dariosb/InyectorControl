/**
 *  \file test_InyControlAct.c
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.06.21  LeFr  v1.0.00  Initial version
 *  2017.07.07  Daba  v1.1.00  No fixture
 *  2017.07.30  Daba  v2.0.00  unitrazer
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 *  DaBa  Dario Baliña  	 dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/*     Test de estructura SM inyeccion de motor segun [AUTO-ER-0001]
 *
 *	[REQ0004] El software debe variar el ciclo de trabajo de la señal PWM 
 *            de 1 KHz de frecuencia, que enviará hacia el inyector en función
 *            de los valores leídos de: el sensor de revoluciones por minuto,
 *            el sensor de posición de mariposa de admisión, 
 *            y el sensor de temperatura de motor. 
 *  [REQ0005] El software debe fijar un ciclo de trabajo de 50% en el PWM 
 *            que envía hacia el inyector, durante 2 segundos luego de la
 *            señal de arranque del motor. 
 *  [REQ0006] El software debe fijar un ciclo de trabajo mínimo de entre 
 *            el 20% y el 30% en el PWM que envía hacia el inyector cuando 
 *            el motor se encuentra “regulando”, para asegurar que el motor
 *            genere 2000 RPM +/- 20 RPM.
 *  [REQ0007] El software debe fijar un ciclo de trabajo entre 30% y 80% en
 *            el PWM que envía hacia el inyector en función proporcionalmente
 *            lineal en que la mariposa de admisión se encuentre abierta.
 *            (Nota: El pedal de aceleración del conductor se conecta 
 *            mecánicamente con la mariposa de admisión. Cuando el pedal
 *            se encuentra sin presionar, la mariposa queda un 30% abierta.
 *            Cuando se presiona a fondo el pedal, la mariposa se abre un 80%.
 *            El ciclo de trabajo del PWM del inyector debe ser linealmente
 *            proporcional con el grado de apertura de dicha mariposa).
 *  [REQ0008] Si el software detecta que la temperatura del motor es menor
 *            a 70C, incrementará en 10% el ciclo de trabajo en el PWM que
 *            ha determinado aplicar al inyector.
 */

/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "InyControl.h"
#include "InyControlEvt.h"
#include "InyControlAct.h"
#include "Mock_RPMControl.h"
#include "Mock_TempSensor.h"
#include "Mock_RPMSensor.h"
#include "Mock_ThrottleSensor.h"
#include "Mock_PWMInyector.h"
#include "Mock_Sensor.h"
#include "Mock_rkhtmr.h"
#include "Mock_rkhassert.h"

#include "rkhfwk_dynevt.h"
#include "rkhsm.h"
#include "rkhtrc.h"
#include "rkhtrc_filter.h"
#include "rkhtrc_record.h"
#include "rkhfwk_bittbl.h"
#include "rkhport.h"
#include "rkhtrc_stream.h"

#include "unitrazer.h"
#include "unitrzlib.h"
#include "tzlink.h"
#include "tzparse.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static InyControl *ic;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    ic = (InyControl *)inyControl;

    Mock_RPMControl_Init();
    Mock_TempSensor_Init();
    Mock_RPMSensor_Init();
    Mock_ThrottleSensor_Init();
    Mock_PWMInyector_Init();
    Mock_Sensor_Init();
    Mock_rkhtmr_Init();
    Mock_rkhassert_Init();
}

void
tearDown(void)
{
    Mock_RPMControl_Verify();
    Mock_TempSensor_Verify();
    Mock_RPMSensor_Verify();
    Mock_ThrottleSensor_Verify();
    Mock_PWMInyector_Verify();
    Mock_Sensor_Verify();
    Mock_rkhtmr_Verify();
    Mock_rkhassert_Verify();

    Mock_RPMControl_Destroy();
    Mock_TempSensor_Destroy();
    Mock_RPMSensor_Destroy();
    Mock_ThrottleSensor_Destroy();
    Mock_PWMInyector_Destroy();
    Mock_Sensor_Destroy();
    Mock_rkhtmr_Destroy();
    Mock_rkhassert_Destroy();
}

void
test_SetInitialValuesAfterInit(void)
{
    TempSensor *temp = (TempSensor *)0xdeadbeef;
    RPMSensor *rpm = (RPMSensor *)0xdeadbeef;
    ThrottleSensor *throttle = (ThrottleSensor *)0xdeadbeef;

    TempSensor_init_ExpectAndReturn(temp);
    RPMSensor_init_ExpectAndReturn(rpm);
    ThrottleSensor_init_ExpectAndReturn(throttle);
    RPMControl_init_Expect(IDLE_MIN_DUTY, IDLE_MAX_DUTY, IDLE_RPM, 
                           IDLE_RPM_THH, IDLE_RPM_THL);
    PWMInyector_init_Expect();
    
	rkh_tmr_init__Expect(&ic->timer, &e_StartTimeout);

    InyControlAct_init(ic);
}

void
test_SetDutyTo50ForAWhileOnStart(void)
{
	rkh_tmr_start_Expect(&ic->timer, RKH_UPCAST(RKH_SMA_T, ic), START_TIME );

    PWMInyector_setDuty_Expect(START_DUTY);

    InyControlAct_starting(ic);
}

void
test_CheckPressedThrottle(void)
{
    rbool_t result;
    Sensor *sensor = (Sensor *)0xdeadbeef;

    Sensor_get_ExpectAndReturn(sensor, THROTTLE_MIN - 1);
    Sensor_get_IgnoreArg_me();

    result = InyControlAct_isPressThrottle(ic);
    TEST_ASSERT_EQUAL(false, result);

    Sensor_get_ExpectAndReturn(sensor, THROTTLE_MIN);
    Sensor_get_IgnoreArg_me();

    result = InyControlAct_isPressThrottle(ic);
    TEST_ASSERT_EQUAL(false, result);

    Sensor_get_ExpectAndReturn(sensor, THROTTLE_MIN + 1);
    Sensor_get_IgnoreArg_me();

    result = InyControlAct_isPressThrottle(ic);
    TEST_ASSERT_EQUAL(true, result);
}

void
test_SetDutyLinearlyWithThrottle(void)
{
    Sensor *sensor = (Sensor *)0xdeadbeef;
    int throttleValues[4] = {THROTTLE_MIN + 5, 
                             THROTTLE_MIN + 10,
                             THROTTLE_MIN + 20,
                             THROTTLE_MIN + 30};
    int *pThrottleValue;

    for (pThrottleValue = throttleValues; 
         pThrottleValue < throttleValues + 4; 
         ++pThrottleValue)
    {
        Sensor_get_ExpectAndReturn(sensor, *pThrottleValue);
        Sensor_get_IgnoreArg_me();

        Sensor_get_ExpectAndReturn(sensor, 90);
        Sensor_get_IgnoreArg_me();
        Sensor_get_ExpectAndReturn(sensor, 0);   /* Ignore RPM */
        Sensor_get_IgnoreArg_me();
        PWMInyector_setDuty_Expect(*pThrottleValue);

        InyControlAct_isPressThrottle(ic);
        InyControlAct_onNormal(ic);
    }
}

void
test_IncrementDutyForColdEngine(void)
{
    Sensor *sensor = (Sensor *)0xdeadbeef;
    unsigned char duty = THROTTLE_MIN + 20;

    Sensor_get_ExpectAndReturn(sensor, duty);
    Sensor_get_IgnoreArg_me();

    Sensor_get_ExpectAndReturn(sensor, ENGINE_MIN_TEMP - 1);
    Sensor_get_IgnoreArg_me();
    Sensor_get_ExpectAndReturn(sensor, 0);   /* Ignore RPM */
    Sensor_get_IgnoreArg_me();
    PWMInyector_setDuty_Expect(duty + INC_DUTY_FOR_COLD);

    InyControlAct_isPressThrottle(ic);
    InyControlAct_onNormal(ic);
}

/* ------------------------------ File footer ------------------------------ */
