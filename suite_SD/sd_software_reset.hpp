/*
##
##  Trade secret of Advanced Micro Devices, Inc.
##  Copyright 2007, Advanced Micro Devices, Inc., (unpublished)
##
##  All rights reserved.  This notice is intended as a precaution against
##  inadvertent publication and does not imply publication or any waiver
##  of confidentiality.  The year included in the foregoing notice is the
##  year of creation of the work.
##
*/

#ifndef _SD_HOST_REGISTERS_H_
#define _SD_HOST_REGISTERS_H_
#include "sdcard_test.hpp"

uint32_t RdWrMaskArray[16] = {
HOST_DMA_ADDRESS		,
HOST_BLOCK_SIZE			,
HOST_BLOCK_COUNT		,
HOST_ARGUMENT			,
HOST_TRANSFER_MODE		,
HOST_COMMAND			,
HOST_RESPONSE_0			,
HOST_RESPONSE_1			,
HOST_RESPONSE_2			,
HOST_RESPONSE_3			,
HOST_RESPONSE_4			,
HOST_RESPONSE_5			,
HOST_RESPONSE_6			,
HOST_RESPONSE_7			,
HOST_BUFFER_0			,
HOST_BUFFER_1			,
HOST_PRESENT_STATE		,
HOST_HOST_CONTROL_1		,
HOST_POWER_CONTROL		,
HOST_BLOCK_GAP_CONTROL	,
HOST_WAKE_UP_CONTROL	,
HOST_CLOCK_CONTROL		,
HOST_TIMEOUT_CONTROL	,
HOST_SOFTWARE_RESET		,
HOST_INT_STATUS			,
HOST_INT_ENABLE			,
HOST_SIGNAL_ENABLE		,
HOST_ACMD_ERR			,
HOST_HOST_CONTROL_2		,
HOST_CAPABILITIES		,
HOST_CAPABILITIES_1		,
HOST_MAX_CURRENT		,

HOST_SET_ACMD12_ERROR	,
HOST_SET_INT_ERROR		,
HOST_ADMA_ERROR			,
    
HOST_ADMA_ADDRESS		,
HOST_PRESET_INIT		,
HOST_PRESET_DEFAILT		,
HOST_PRESET_HIGHSPEED	,
HOST_PRESET_SDR12		,
HOST_PRESET_SDR25		,
HOST_PRESET_SDR50		,
HOST_PRESET_SDR104		,
HOST_PRESET_DDR50		,
HOST_SLOT_INT_STATUS	,
HOST_HOST_VERSION		,
}

#endif