/**
 *  \file test_InyectorControl.c
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.06.21  LeFr  v1.0.00  Initial version
 *  2017.07.07  Daba  v1.1.00  No fixture
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
#include "common.h"
#include "rkh.h"

/**************************************/
/*
 * Ceedling bug????
 * https://github.com/ThrowTheSwitch/Ceedling/issues/10
 * have to include all rkh header to force compilation because is not linking
 */
#include "rkhfwk_dynevt.h"
#include "rkhsm.h"
#include "rkhtrc.h"
#include "rkhtrc_filter.h"
#include "rkhtrc_record.h"
#include "rkhfwk_bittbl.h"
#include "rkhport.h"
#include "rkhtrc_stream.h"
/**************************************/

#include "InyectorControl.h"
#include "Mock_InyectorControlAct.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhtrc_out.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ST_T *state, *expectNextState;

static RKH_STATIC_EVENT(eventStartTimeout, evStartTimeout);
static RKH_STATIC_EVENT(eventStart,        evStart);
static RKH_STATIC_EVENT(eventTick,         evTick);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setProfile(RKH_ST_T *currState, RKH_ST_T *nextState)
{
    setState((RKH_SMA_T *)inyectorControl, currState);
    expectNextState = nextState;
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
    Mock_InyectorControlAct_Init();
}

void
tearDown(void)
{
    Mock_InyectorControlAct_Verify();
    Mock_InyectorControlAct_Destroy();
}

void
test_DefaultStateAfterInit(void)
{
    setProfile(RKH_STATE_CAST(&off), RKH_STATE_CAST(&off));

    InyectorControlAct_init_Expect(RKH_CAST(InyectorControl, inyectorControl));

    rkh_sm_init((RKH_SM_T *)inyectorControl);
    
    TEST_ASSERT_TRUE(expectNextState == getState(inyectorControl));
}

void
test_AnUnhandledEventDoesNotChangeState(void)
{
    setProfile(RKH_STATE_CAST(&off), RKH_STATE_CAST(&off));
    
    rkh_sm_dispatch((RKH_SM_T *)inyectorControl, &eventStartTimeout);

    TEST_ASSERT_TRUE(expectNextState == getState(inyectorControl));
}

#if 0
void
test_StateTransitionTableForOff(void)
{
    _setProfile(off, starting);
    InyectorControlAct_starting_Expect(
                RKH_CAST(InyectorControl, inyectorControl));

    state = InyectorControl_dispatch(&eventStart);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForStarting(void)
{
    _setProfile(starting, idleSpeed);
    state = InyectorControl_dispatch(&eventStartTimeout);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForIdleSpeed(void)
{
    _setProfile(idleSpeed, idleSpeed);
    InyectorControlAct_isReleasedThrottle_ExpectAndReturn(
                RKH_CAST(InyectorControlevent, inyectorControl), RKH_TRUE);

    InyectorControlAct_onIdleSpeed_Expect(&eventTick);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);

    _setProfile(idleSpeed, normal);
    InyectorControlAct_isReleasedThrottle_ExpectAndReturn(
                RKH_CAST(InyectorControlevent, inyectorControl), RKH_FALSE);

    state = InyectorControl_dispatch(&eventTick);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForNormal(void)
{
    _setProfile(normal, normal);
    InyectorControlAct_isPressedThrottle_ExpectAndReturn(
                RKH_CAST(InyectorControlevent, inyectorControl), RKH_TRUE);

    InyectorControlAct_onNormal_Expect(&event);
    state = InyectorControl_dispatch(&eventTick);
    TEST_ASSERT_EQUAL(expectNextState, state);

    _setProfile(normal, idleSpeed);
    InyectorControlAct_isPressedThrottle_ExpectAndReturn(
                RKH_CAST(InyectorControlevent, inyectorControl), RKH_FALSE);

    state = InyectorControl_dispatch(&eventTick);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForNormal_UnhandledEvents(void)
{
    _setProfile(normal, UNHANDLED_EVENT);
    state = InyectorControl_dispatch(&eventStart);
    TEST_ASSERT_EQUAL(expectNextState, state);
    TEST_ASSERT_EQUAL(normal, InyectorControl_getState());

    _setProfile(normal, UNHANDLED_EVENT, evStartTimeout);
    state = InyectorControl_dispatch(&eventStart);
    TEST_ASSERT_EQUAL(expectNextState, state);
    TEST_ASSERT_EQUAL(normal, InyectorControl_getState());
}
#endif
/* ------------------------------ File footer ------------------------------ */
