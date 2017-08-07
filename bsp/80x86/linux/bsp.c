/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH web site:   http://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail:         francuccilea@gmail.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       bsp.c
 *  \brief      BSP for 80x86 OS Linux
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.04.14  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 *  DaBa  Dario Baliña       dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <stdlib.h>

#include "InyControl.h"
#include "InyControlEvt.h"
#include "bsp.h"
#include "rkh.h"
#include "trace_io_cfg.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
#define rpm_in_idleRange()  ((rpm >= IDLE_RPM-IDLE_RPM_THL) && \
                             (rpm <= IDLE_RPM+IDLE_RPM_THH))
/* ------------------------------- Constants ------------------------------- */
#define ESC                         0x1B
#define kbmap(c)                    ((c) - '0')
#define SAMPLE_TICK                 10
#define X_OFFSET                    2
#define Y_OFFSET                    20
#define TEMP_MIN                    20
#define TEMP_STEP                   10
#define TEMP_MAX                    110
#define RPM_MIN                     1000
#define RPM_STEP                    500
#define RPM_IDLE_STEP               5
#define RPM_MAX                     7000
#define THRT_MIN                    0
#define THRT_STEP                   10
#define THRT_MAX                    100


/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(e_Term, evTerm);
static int sampleTick = SAMPLE_TICK;
static int temperature  = TEMP_MIN;
static int rpm          = RPM_MIN;
static int throttle     = THRT_MIN;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
printBanner(void)
{
    system("clear");
    printf("Inyector Control example\n\n");
    printf("RKH version      = %s\n", RKH_RELEASE);
    printf("Port version     = %s\n", rkhport_get_version());
    printf("Port description = %s\n\n", rkhport_get_desc());
    printf("Description: \n\n"
           "Example from CESE2017 class 7 \n"
           "According to [AUTO-ER-0001] Iter 7\n"
           "\n\n\n");

    printf("1.- Press [s] to start.\n");
    printf("2.- Press [R/r] to inc/dec RPM.\n");
    printf("2.- Press [T/t] to inc/dec Temperature.\n");
    printf("2.- Press [A/a] to inc/dec Throttle.\n");
    printf("2.- Press ESC to quit \n\n\n");

}

static void
gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

/* ---------------------------- Global functions --------------------------- */
void
bsp_init(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printBanner();

    trace_io_setConfig(argc, argv);

    rkh_fwk_init();

    RKH_TRC_OPEN();

    RKH_FILTER_ON_GROUP(RKH_TRC_ALL_GROUPS);
    RKH_FILTER_ON_EVENT(RKH_TRC_ALL_EVENTS);
    RKH_FILTER_OFF_GROUP_ALL_EVENTS(RKH_TG_SM);
    RKH_FILTER_OFF_SMA(inyControl);

    RKH_TR_FWK_AO(inyControl);
    RKH_TR_FWK_STATE(inyControl, &off);
    RKH_TR_FWK_STATE(inyControl, &starting);
    RKH_TR_FWK_STATE(inyControl, &idleSpeed);
    RKH_TR_FWK_STATE(inyControl, &normal);
    RKH_TR_FWK_SIG(evStart);
    RKH_TR_FWK_SIG(evStartTimeout);
    RKH_TR_FWK_SIG(evTick);
}

void
bsp_keyParser(int c)
{
    switch(c)
    {
        case ESC:
            system("clear");
            RKH_SMA_POST_FIFO(inyControl, &e_Term, 0);
            rkhport_fwk_stop();
            break;

        case 's':
            RKH_SMA_POST_FIFO(inyControl, &e_Start, 0); 
            RKH_FILTER_ON_EVENT(evTick);
            RKH_FILTER_ON_EVENT(RKH_TE_SM_EVT_PROC);
            RKH_FILTER_ON_EVENT(RKH_TE_SM_TRN);
            RKH_FILTER_ON_EVENT(RKH_TE_SM_NTRNACT);
            break;

        case 't':
            if(temperature > TEMP_MIN)
                temperature -= TEMP_STEP;
            break;

        case 'T':
            if(temperature < TEMP_MAX)
                temperature += TEMP_STEP;
            break;

        case 'a':
            if(throttle > THRT_MIN)
                throttle -= THRT_STEP;
            break;

        case 'A':
            if(throttle < THRT_MAX)
                throttle += THRT_STEP;
            break;

        case 'r':
            if(rpm_in_idleRange())
                rpm -= RPM_IDLE_STEP;
            else if(rpm > RPM_MIN)
                rpm -= RPM_STEP;
            break;

        case 'R':
            if(rpm_in_idleRange())
                rpm += RPM_IDLE_STEP;
            else if(rpm < RPM_MAX)
                rpm += RPM_STEP;
            break;

        default:
            break;
    }
}

void
bsp_timeTick(void)
{
/*    if(sampleTick && --sampleTick == 0)*/
    {
        sampleTick = SAMPLE_TICK;
        RKH_SMA_POST_FIFO(inyControl, &e_Tick, 0); 
    }
}

void
bsp_PWMInyector_setDuty(unsigned char duty)
{
    gotoxy(X_OFFSET, Y_OFFSET+0);
    printf("PWMInyector Duty: %3d%%\n", duty);
}

int
bsp_TempSensorRead(void)
{
    gotoxy(X_OFFSET, Y_OFFSET+1);
    printf("Temperature : %3dC\n", temperature);
    return temperature;
}

int
bsp_RPMSensorRead(void)
{
    gotoxy(X_OFFSET, Y_OFFSET+2);
    printf("RPM : %4dRPM\n", rpm);
    return rpm;
}

int
bsp_ThrottleSensorRead(void)
{
    gotoxy(X_OFFSET, Y_OFFSET+3);
    printf("Throttle : %3d%%\n", throttle);
    return throttle;
}

/* ------------------------------ File footer ------------------------------ */
