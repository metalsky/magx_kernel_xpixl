/*
 * otg/otg/usbp-mcpc.h
 * @(#) balden@seth2.belcarratech.com|otg/otg/usbp-mcpc.h|20051116205002|23448
 *
 *      Copyright (c) 2004-2005 Belcarra
 * By:
 *      Stuart Lynne <sl@belcarra.com>,
 *      Bruce Balden <balden@belcarra.com>
 *
 * Copyright 2005-2006 Motorola, Inc.
 *
 * Changelog:
 * Date               Author           Comment
 * -----------------------------------------------------------------------------
 * 12/12/2005         Motorola         Initial distribution
 * 10/18/2006         Motorola         Add Open Src Software language
 * 12/12/2006         Motorola         Changes for Open Src compliance.
 */
/*
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
 * @file otg/otg/usbp-mcpc.h
 * @brief MCPC class descriptor structure definitions.
 *
 * @ingroup USBP
 */

#ifdef PRAGMAPACK
#pragma pack(push,1)
#endif


/*!
 * @name Management Element Requests
 * C.f. Table 11 - bRequest Value
 */
 /*! @{ */
#define MCPC_REQ_ACTIVATE_MODE                  0x60
#define MCPC_REQ_GET_MODETABLE                  0x61
#define MCPC_REQ_SET_LINK                       0x62
#define MCPC_REQ_CLEAR_LINK                     0x63
#define MCPC_REQ_MODE_SPECIFIC_REQUEST          0x64
 /*! @} */


/*!
 * @name MCPC Vendor Requests
 * C.f. 
 */
 /*! @{ */
#define MCPC_VENDOR_REQUEST                     0x44
#define MCPC_VENDOR_SUBREQUEST_MACM_AB          0x11
#define MCPC_VENDOR_SUBREQUEST_MACM_DL          0x12
 /*! @} */



/*!
* @name C.f. Table 16 - Connection Model selector
 */
 /*! @{ */

#define MCPC_MOBILE_ASTRACT_CONTROL_MODEL       0x00
#define MCPC_MOBILE_DIRECT_LINE_MODEL           0x01
 /*! @} */


/*!
 * @name Notification codes
 * C.f. Table 20 bNotification value
 */
 /*! @{ */
#define MCPC_REQUEST_MODE                       0x30
#define MCPC_REQUEST_ACKNOWLEDGE                0x31
#define MCPC_MODE_SPECIFIC_NOTIFICATION         0x30
 /*! @} */

/*!
 * @name c.f. MCPC Table 20
 */
 /*! @{ */

#define MCPC_NOTIFICATION_REQUEST_MODE          0x30
#define MCPC_NOTIFICATION_REQUEST_ACKNOWLEDGE   0x31
#define MCPC_NOTIFICATION_MODE_SPECIFIC         0x32

#define MCPC_MODE_SELECTOR_OK                   0x00
#define MCPC_MODE_SELECTOR_NG                   0x01

/*! @} */


/*!
 * @name C.f. Table 25 Mobile Abstract Control Model Specific Function Descriptor
 */
 /*! @{ */

struct mpcp_abstract_control_model_specific_functional_descriptor {
        u8 bFunctionLength;
        u8 bDescriptorType;
        u8 bDescriptorSubType;
        u8 bType;
        u8 bMode_n[1];
};

 /*! @} */

/*! 
 * @name bType pipe groups
 * C.f. MCPC Table 25
 * C.f. MCPC Table 26
 */
#define MCPC_PIPE_GROUP_AB_1    0x01
#define MCPC_PIPE_GROUP_AB_2    0x02
#define MCPC_PIPE_GROUP_AB_3    0x03
#define MCPC_PIPE_GROUP_AB_4    0x04
#define MCPC_PIPE_GROUP_AB_5    0x05
#define MCPC_PIPE_GROUP_AB_6    0x06
#define MCPC_PIPE_GROUP_DL_1    0x01
#define MCPC_PIPE_GROUP_DL_2    0x02
/*! @} */


/*!
 * @name C.f. Table 26 Mobile Direct Line Model Specific Function Descriptor
 */
 /*! @{ */

struct mpcp_direct_line_model_specific_functional_descriptor {
        u8 bFunctionLength;
        u8 bDescriptorType;
        u8 bDescriptorSubType;
        u8 bType;
        u8 bMode_n[1];
};

#define MCPC_PIPE_GROUP_DL_1                    0x01
#define MCPC_PIPE_GROUP_DL_2                    0x02
 /*! @} */

/*!
 * @name C.f. Table 27 bDescriptorSubType in Functional Descriptor
 */
 /*! @{ */
#define MCPC_MACM_FUNCTIONAL_DESCRIPTOR         0x11
#define MCPC_MDLM_FUNCTIONAL_DESCRIPTOR         0x12
 /*! @} */


/*!
 * @name C.f. Table 28 Mode (bMode_x) Value
 */
 /*! @{ */

#define MCPC_MODE_DEACTIVATE                    0x00
#define MCPC_MODE_MODEM_MODE                    0x01
#define MCPC_MODE_AT_COMMAND_CONTROL_MODE       0x02
#define MCPC_MODE_LAN_MODE                      0x03
#define MCPC_MODE_VOICE_COMMUNICATION_MODE      0x30
#define MCPC_MODE_OBJECT_EXCHANGE_MODE          0x60
#define MCPC_MODE_MDLM_FULL_SUPPORT             0x90
#define MCPC_MODE_MDLM_ASYNC_ONLY               0x91
#define MCPC_MODE_UNLINKED_STATE                0xff
 /*! @} */

/*!
 * @name MCPC
 */
 /*! @{ */

PACKED0 struct PACKED1 usbd_mobile_abstract_control_model_specific_desciptor {
        u8 bFunctionLength;
        u8 bDescriptorType;
        u8 bDescriptorSubtype;
        u8 bType;
        u8 bMode_N[4];
};

PACKED0 struct PACKED1 usbd_mobile_direct_line_model_specific_desciptor {
        u8 bFunctionLength;
        u8 bDescriptorType;
        u8 bDescriptorSubType;
        u8 bType;
        u8 bMode_N[4];
};



/*! 
 * MCPC mode
 */

typedef enum mcpc_mode {
        mcpc_unlinked,
        mcpc_linked,
        mcpc_activated
} mcpc_mode_t;


/*! @} */

#ifdef PRAGMAPACK
#pragma pack(pop)
#endif


/* End of FILE */
