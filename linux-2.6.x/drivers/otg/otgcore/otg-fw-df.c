
/* Generated by otg-tests-c.awk
 *
 * Do not Edit, see otg-state.awk
 */
/*
 * Copyright 2005-2006 Motorola, Inc.
 *
 * Changelog:
 * Date               Author           Comment
 * -----------------------------------------------------------------------------
 * 12/12/2005         Motorola         Initial distribution
 * 10/18/2006         Motorola         Add Open Src Software language
 * 12/12/2006         Motorola         Changes for Open Src compliance.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */


/*!
* @file otg/otgcore/otg-fw-df.c
* @brief OTG Firmware - Firmware for df
*
* This file defines the OTG State Machine tests.
*
*
* @ingroup OTGFW
*/

/*!
* @page OTGFW
* @section OTGFW - otg-fw-df.c
* This contains the input, output and timout definitions for the OTG state machine firmware
*/


#ifdef OTG_APPLICATION
#define NULL 0
#include "otg-fw.h"
#include "otg-fw-df.h"
#else	/* OTG_APPLICATION/ */
#include <otg/otg-compat.h>
#include <otg/usbp-chap9.h>
#include <otg/usbp-func.h>
#include <otg/usbp-bus.h>
#include <otg/otg-trace.h>
//#include <otg/otg-api.h>
#include <otg/otg-fw.h>
#include <otg/otg-fw-df.h>
#endif	/* OTG_APPLICATION */

char            otg_fw_name_df[] = "otg_df";


struct otg_test otg_tests_df[] = {
	/*
	 * Copyright (c) 2004 Belcarra
	 *
	 */
	/*!
	 * This is the default Firmware. It is included in the
	 * compiled modules and supports the auto Traditional USB
	 * mode. No user inputs are implemented.
	 */
	{					 /*   */
	 0,					 /* .test */
	 invalid_state,				 /* .state */
	 otg_disabled,				 /* .target */
	 enable_otg,				 /* .test1 */
	 },
	/*
	 * This is not an OTG State. It is used internally to mark the end of the
	 * list of states and inputs.
	 */
	{					 /*   */
	 1,					 /* .test */
	 terminator_state,			 /* .state */
	 invalid_state,				 /* .target */
	 0,					 /* .test1 */
	 },
	{2, invalid_state,},

};

#define OTG_TESTS_DF 2

int             otg_test_max_df = 2;

 /* eof */

/* Generated by otg-info-c.awk
 *
 * Do not Edit, see otg-state.awk
 */

struct otg_state otg_states_df[OTG_STATES_DF + 1] = {
	{					 /* 0 */
	 invalid_state,				 /* .state */
	 m_otg_init,				 /* .meta */
	 "invalid_state",			 /* .name */
	 0,					 /* .tmout */
	 0,					 /* .reset */
	 },
	{					 /* 1 */
	 otg_disabled,				 /* .state */
	 m_otg_init,				 /* .meta */
	 "otg_disabled",			 /* .name */
	 0,					 /* .tmout */
	 0,					 /* .reset */
	 },
	{					 /* 2 */
	 terminator_state,			 /* .state */
	 m_otg_init,				 /* .meta */
	 "terminator_state",			 /* .name */
	 0,					 /* .tmout */
	 0,					 /* .reset */
	 /* .outputs */
	 0,
	 },

	{0, 0, "", 0, 0,},

};

struct otg_firmware otg_firmware_df = {
	OTG_STATES_DF,				 /* number of states */
	OTG_TESTS_DF,				 /* number of tests */
	"otg-df",				 /* name of firmware */
	otg_states_df,				 /* struct otg_state * */
	otg_tests_df,				 /* struct otg_test * */
};

/* eof */
