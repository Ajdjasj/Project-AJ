#ifndef _PCI_BOLTON_H_
#define _PCI_BOLTON_H_

/*
 *  Secure Digital Host Controller Interface driver
 *  Header file for Host Controller registers and I/O accessors.
 *  Copyright (C) AMD, All Rights Reserved.
 *  AMD SB CHPSET : BOLTON, SDHC
 *  SD 3.0 Host Controller Standerd Specification
 */

/* Bolton SD PCI register defines */
#define SDHC_PCI_VENDORID		0x00
#define SDHC_PCI_DEVICEID		0x02
#define SDHC_PCI_OR_MMIO_COMMAND		0x04
#define SDHC_PCI_STATUS			0x06
#define SDHC_PCI_REVISION_ID	0x08
#define SDHC_PCI_CLASS_CODE     0x09
#define SDHC_PCI_CACHELINESIZE	0x0C
#define SDHC_PCI_LATENCYTIMER	0x0D
#define SDHC_PCI_HEADERTYPE		0x0E
#define SDHC_PCI_BIST			0x0F
#define SDHC_PCI_BASEADDRESS	0x10
#define SDHC_PCI_UPPERBASEADDRESS				0x14
#define SDHC_PCI_SUBSYSTEM_SUBSYSTEMVENDOR_ID	0x2C
#define SDHC_PCI_CAPABILTYPTR	0x34
#define SDHC_PCI_INTLINE		0x3C
#define SDHC_PCI_INTPIN			0x3D
#define SDHC_PCI_SLOTINFO		0x40
#define SDHC_PCI_MSICAPHEADER	0x80
#define SDHC_PCI_MSIADDR		0x84
#define SDHC_PCI_MSIUADDR		0x88
#define SDHC_PCI_MSIDATA		0x8C
#define SDHC_PCI_PWRMGMTCAP_HEADER		0x90
#define SDHC_PCI_PMCSR			0x94
#define SDHC_PCI_CARDDETECTION	0xA0
#define SDHC_PCI_SDCAP_0		0xA4
#define SDHC_PCI_SDMAX_CURRENTCAP		0xA8
#define SDHC_PCI_SDCONTROL		0xAC
#define SDHC_PCI_DEBUG			0xB0
#define SDHC_PCI_SDDMAIFCNTRL	0xC0

/* Bolton SD PCI register defines End*/

#endif