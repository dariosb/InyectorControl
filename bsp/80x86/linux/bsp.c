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
#include "bsp.h"
#include "rkh.h"
#include "trace_io_cfg.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define ESC                         0x1B
#define kbmap(c)                    ((c) - '0')

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(e_Term, evTerm);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
printBanner(void)
{
    printf("Inyector Control example\n\n");
    printf("RKH version      = %s\n", RKH_RELEASE);
    printf("Port version     = %s\n", rkhport_get_version());
    printf("Port description = %s\n\n", rkhport_get_desc());
    printf("Description: \n\n"
           "Example from CESE2017 class 7 \n"
           "According to [AUTO-ER-0001] Iter 7\n"
           "\n\n\n");

    printf("1.- Press to send events to state machine. \n");
    printf("2.- Press ESC to quit \n\n\n");

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
    if (c == ESC)
    {
        RKH_SMA_POST_FIFO(inyControl, &e_Term, 0);
        rkhport_fwk_stop();
    }
}

void
bsp_timeTick(void)
{
}

/* ------------------------------ File footer ------------------------------ */
