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
#include "InyectorControl.h"
#include "Mock_InyectorControlAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static int state, expectNextState;
static Event event;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setProfile(int currState, int nextState, int signal)
{
    InyectorControl_setState(currState);
    event.signal = signal;
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
    InyectorControlAct_init_Expect();
    expectNextState = InyectorControl_init();
    TEST_ASSERT_EQUAL(off, expectNextState);
}

void
test_AnUnhandledEventDoesNotChangeState(void)
{
    setProfile(off, UNHANDLED_EVENT, evStartTimeout);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
    TEST_ASSERT_EQUAL(off, InyectorControl_getState());
}

void
test_StateTransitionTableForOff(void)
{
    setProfile(off, starting, evStart);
    InyectorControlAct_starting_Expect(&event);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForStarting(void)
{
    setProfile(starting, idleSpeed, evStartTimeout);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForIdleSpeed(void)
{
    setProfile(idleSpeed, idleSpeed, evTick);
    InyectorControlAct_isReleasedThrottle_ExpectAndReturn(&event, true);
    InyectorControlAct_onIdleSpeed_Expect(&event);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);

    setProfile(idleSpeed, normal, evTick);
    InyectorControlAct_isReleasedThrottle_ExpectAndReturn(&event, false);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForNormal(void)
{
    setProfile(normal, normal, evTick);
    InyectorControlAct_isPressedThrottle_ExpectAndReturn(&event, true);
    InyectorControlAct_onNormal_Expect(&event);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);

    setProfile(normal, idleSpeed, evTick);
    InyectorControlAct_isPressedThrottle_ExpectAndReturn(&event, false);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
}

void
test_StateTransitionTableForNormal_UnhandledEvents(void)
{
    setProfile(normal, UNHANDLED_EVENT, evStart);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
    TEST_ASSERT_EQUAL(normal, InyectorControl_getState());

    setProfile(normal, UNHANDLED_EVENT, evStartTimeout);
    state = InyectorControl_dispatch(&event);
    TEST_ASSERT_EQUAL(expectNextState, state);
    TEST_ASSERT_EQUAL(normal, InyectorControl_getState());
}

/* ------------------------------ File footer ------------------------------ */
