#ifndef _SD_PCI_H
#define _SD_PCI_H

#if SD_PCI
#include <iostream>
#include "env_pci.h"
#include "sd_wrapper.h"

	//#include "sd2_0.h" /* Include SD2.0 specific register defines */
	//#include "pci_bolton.h" /* Include bolton specific register defines */
#include "pci_kabini.h" 

#define MAX_SD_PCI_DEV 0x05
#define SDHC_PCI_SUB_CLASS_CLASS_CODE 0x0A
#define SDHC_PCI_VENDOR_ID SDHC_PCI_VENDORID
#define SDHC_PCI_DEVICE_ID SDHC_PCI_DEVICEID
#define SD_DEVICE_CLASS 0x0805
#define SDHC_PCI_BAR0 0x10
#define SDHC_PCI_BAR1 0x14
#define SDHC_PCI_BAR2 0x18
#define SDHC_PCI_BAR3 0x1C
#define SDHC_PCI_BAR4 0x20
#define SDHC_PCI_BAR5 0x24
#define PCI_BAR_WIDTH 0x04
#define PCI_ENABLE_MEMACCESS_BUSMASTER 0x0006
#define PCI_LOCATION              (unsigned short)(bus),(unsigned short)(device),(unsigned short)(function)
#define PCI_TOTAL_SLOT(slot_info) (slot_info >> 0x04) 
#define PCI_FIRST_BAR(slot_info)  (slot_info & 0x0F)
#define GET_BAR(slot_info)        (PCI_FIRST_BAR(slot_info) * 4 + SDHC_PCI_BAR0)


typedef struct
{
	size_t bar0;
	size_t bar1;
	size_t bar2;
	size_t bar3;
	size_t bar4;
	size_t bar5;
	uint32_t classcode:24;	
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;	
	uint16_t status;
	uint8_t revision_id;
	uint8_t cachelinesize;
	uint8_t latencytimer;
	uint8_t headertype;
	uint8_t bist;
	uint8_t slotinfo;
	uint8_t bus;
    uint8_t device;
	uint8_t function; 	
}_pci_type0_header;

class _pci_node  
{
private:
public:
	uint8_t bus;
    uint8_t device;
	uint8_t function;
    _pci_type0_header pci_header;
	_reg reg;
	bool configured;

    _pci_node();
	~_pci_node();

	bool init_node(uint8_t bus, uint8_t device, uint8_t function);
	bool get_header(_pci_type0_header*);
	void get_bus_device_function(uint16_t *bus_number,uint16_t *device_number , uint16_t *function_number);
	bool read_pci();
	bool write_pci();
}; 

class _pci_handler
{
private:

public:
	uint8_t total_pci;
	_pci_node node[MAX_SD_PCI_DEV];
	bool get_pci_header(_pci_type0_header *header_ptr, uint32_t dev_number);
	bool read_pci(_reg *preg);
	bool write_pci(_reg *preg);
	uint8_t get_total_dev();
	size_t get_bar(uint32_t dev_number, uint32_t slot);
	uint8_t get_total_slot(uint32_t device);
	void get_bus_device_function(uint32_t device, uint16_t *bus_number,uint16_t *device_number , uint16_t *function_number);
    _pci_handler();
	~_pci_handler();
};
#endif


#endif

