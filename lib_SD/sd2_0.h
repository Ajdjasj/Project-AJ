#ifndef _SD2_0_H_
#define _SD2_0_H_
/*
 *  Secure Digital Host Controller Interface driver
 *  Header file for Host Controller registers and I/O accessors.
 *  Copyright (C) AMD, All Rights Reserved.
 *  AMD SB CHPSET : BOLTON, SDHC
 *  SD 2.0 Host Controller Standerd Specification
 */

/* SD 2.0 HOST register defines */
#define SDHCI_DMA_ADDRESS		0x00
#define SDHCI_BLOCK_SIZE		0x04
#define SDHCI_BLOCK_COUNT		0x06
#define SDHCI_ARGUMENT			0x08
#define SDHCI_TRANSFER_MODE		0x0C
#define SDHCI_COMMAND			0x0E
#define SDHCI_RESPONSE_0		0x10
#define SDHCI_RESPONSE_1		0x12
#define SDHCI_RESPONSE_2		0x14
#define SDHCI_RESPONSE_3		0x16
#define SDHCI_RESPONSE_4		0x18
#define SDHCI_RESPONSE_5		0x1A
#define SDHCI_RESPONSE_6		0x1C
#define SDHCI_RESPONSE_7		0x1E
#define SDHCI_BUFFER_0			0x20
#define SDHCI_BUFFER_1			0x22
#define SDHCI_PRESENT_STATE		0x24
#define SDHCI_HOST_CONTROL_1	0x28
#define SDHCI_POWER_CONTROL		0x29
#define SDHCI_BLOCK_GAP_CONTROL	0x2A
#define SDHCI_WAKE_UP_CONTROL	0x2B
#define SDHCI_CLOCK_CONTROL		0x2C
#define SDHCI_TIMEOUT_CONTROL	0x2E
#define SDHCI_SOFTWARE_RESET	0x2F
#define SDHCI_INT_STATUS		0x30
#define SDHCI_INT_ENABLE		0x34
#define SDHCI_SIGNAL_ENABLE		0x38
#define SDHCI_ACMD_ERR			0x3C
/* 3E-3F reserved */
#define SDHCI_CAPABILITIES		0x40
#define SDHCI_CAPABILITIES_1	0x44
#define SDHCI_MAX_CURRENT		0x48
/* 4C-4F reserved for more max current */
#define SDHCI_SET_ACMD12_ERROR	0x50
#define SDHCI_SET_INT_ERROR		0x52
#define SDHCI_ADMA_ERROR		0x54
/* 55-57 reserved */
#define SDHCI_ADMA_ADDRESS		0x58
/* 60-FB reserved */
#define SDHCI_SLOT_INT_STATUS	0xFC
#define SDHCI_HOST_VERSION		0xFE
/*
 * End of controller registers.
 */


typedef struct 
{
	char *name;
	uint32_t offset;
	uint32_t width;
}_sd_register;

#define BYTE  0x01
#define WORD  0x02
#define DWORD 0x04
#define QWORD 0x08


#define ADD_REGISTER(r,o,w) {#r, o , w }
_sd_register sd_register[] ={
ADD_REGISTER(	SDHCI_DMA_ADDRESS				,	0x00	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_BLOCK_SIZE				,	0x04	,	WORD  		) ,
ADD_REGISTER(	SDHCI_BLOCK_COUNT				,	0x06	, 	WORD  		) ,
ADD_REGISTER(	SDHCI_ARGUMENT					,	0x08	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_TRANSFER_MODE				,	0x0C	,	WORD  		) ,
ADD_REGISTER(	SDHCI_COMMAND					,	0x0E	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_0				,	0x10	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_1				,	0x12	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_2				,	0x14	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_3				,	0x16	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_4				,	0x18	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_5				,	0x1A	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_6				,	0x1C	,	WORD  		) ,
ADD_REGISTER(	SDHCI_RESPONSE_7				,	0x1E	,	WORD  		) ,
ADD_REGISTER(	SDHCI_BUFFER_0					,	0x20	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_BUFFER_1					,	0x21	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_BUFFER_2					,	0x22	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_BUFFER_3					,	0x23	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_PRESENT_STATE				,	0x24	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_HOST_CONTROL_1			,	0x28	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_POWER_CONTROL				,	0x29	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_BLOCK_GAP_CONTROL			,	0x2A	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_WAKE_UP_CONTROL			,	0x2B	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_CLOCK_CONTROL				,	0x2C	,	WORD  		) ,
ADD_REGISTER(	SDHCI_TIMEOUT_CONTROL			,	0x2E	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_SOFTWARE_RESET			,	0x2F	,	BYTE  		) ,
ADD_REGISTER(	SDHCI_INT_STATUS				,	0x30	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_INT_ENABLE				,	0x34	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_SIGNAL_ENABLE				,	0x38	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_ACMD_ERR					,	0x3C	,	WORD  		) ,
/* reserved */                      	                              
ADD_REGISTER(	SDHCI_CAPABILITIES				,	0x40	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_CAPABILITIES_1			,	0x44	,	DWORD 		) ,
ADD_REGISTER(	SDHCI_MAX_CURRENT				,	0x48	,	DWORD 		) ,
/* reserved for more max current */ 			                      		
ADD_REGISTER(	SDHCI_SET_ACMD12_ERROR			,	0x50	,	WORD  		) ,
ADD_REGISTER(	SDHCI_SET_INT_ERROR				,	0x52	,	WORD  		) ,
ADD_REGISTER(	SDHCI_ADMA_ERROR				,	0x54	,	BYTE  		) ,
/* reserved */                      	                              		
ADD_REGISTER(	SDHCI_ADMA_ADDRESS				,	0x58	,	QWORD 		) ,
/* reserved */                      	                              		
ADD_REGISTER(	SDHCI_SLOT_INT_STATUS			,	0xFC	,	WORD  		) ,
ADD_REGISTER(	SDHCI_HOST_VERSION				,	0xFE	,	WORD  		) 
};

#endif
