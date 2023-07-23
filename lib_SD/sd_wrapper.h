
#ifndef _SD_WRAPPER_H_
#define _SD_WRAPPER_H_

 #include "host_types.h"
#include "core_logger.h"
#include "resource_manager.h"

#if SD_PCI
	 #include <env_pci.h>
#endif
 
 /* macros for reading/writing memory-mapped registers */
#define WRITE_ULONG(a, b)    *(volatile unsigned long *)(a) = (b)
#define READ_ULONG(a)        (*(volatile unsigned long *)(a))
#define WRITE_UWORD(a, b)    *(volatile uint16_t *)(a) = (b)
#define READ_UWORD(a)        (*(volatile uint16_t *)(a))
#define WRITE_UBYTE(a, b)    *(volatile uint8_t *)(a) = (b)
#define READ_UBYTE(a)        (*(volatile uint8_t *)(a))


#define SD_CFG_PAGE_SIZE     		 2048 
#define SD_HOSTCNTRL_PAGE_SIZE     2048 


 // SD cfg space and host controller space virtual addresses
	extern uintptr_t		SdCfgVirtAddrss;
	extern uintptr_t		SdHostcntrlVirtAddrss;
	
	/* structures, typedefs, and unions */
	typedef struct _SD_INFO
	{
		// Input
		void *pRegMap;    // Memory-mapped I/O registers
	} SD_INFO, *PSD_INFO;
	
	
 
	void SET_SD_REGISTER_WRITE(_reg *x,uint32_t y,uint32_t t_device,uint32_t t_slot,uint32_t t_value);
	void SET_SD_REGISTER_READ(_reg *x,uint32_t y,uint32_t t_device,uint32_t t_slot);
	bool rhost_reg(_reg *preg);
	bool whost_reg(_reg *preg);

	uint64_t sd_read_reg(size_t offset,unsigned short width);
	void sd_write_reg(size_t offset,uint64_t value,unsigned short width);
	uint32_t port_read(uint8_t value);

	uint64_t sd_hd_read_mmio(size_t offset,unsigned short width);

	bool Enable_IO_Mux_AM4_ZP_RV_SD_MMIO();
	bool SdGetVirtualAdress(void);
	uint64_t  SdMMIORegRead( uint32_t cfg_or_hostcntrl, size_t offset, uint16_t nbytes);
	void sd_hd_write_mmio(size_t offset,size_t value,unsigned short width);
	void  SdMMIORegWrite(uint32_t cfg_or_hostcntrl, size_t offset, size_t value, uint16_t nbytes);

#if SD_PCI
	bool create_pci_device(uint32_t pci_bus, uint32_t pci_device, uint32_t pci_function); 
	int64_t hd_read_pci(unsigned short bus1,unsigned short device1,unsigned short function1,size_t offset,unsigned short width);
	void hd_write_pci(unsigned short bus1,unsigned short device1,unsigned short function1,size_t offset,size_t value,unsigned short width);
#endif



enum{
	HOST_DMA_ADDRESS			,	
	HOST_BLOCK_SIZE			    ,
	HOST_BLOCK_COUNT		    ,
	HOST_ARGUMENT			    ,
	HOST_TRANSFER_MODE		    ,
	HOST_COMMAND			    ,
	HOST_RESPONSE_0			    ,
	HOST_RESPONSE_1			    ,
	HOST_RESPONSE_2			    ,
	HOST_RESPONSE_3			    ,
	HOST_RESPONSE_4			    ,
	HOST_RESPONSE_5			    ,
	HOST_RESPONSE_6			    ,
	HOST_RESPONSE_7			    ,
	HOST_BUFFER_0			    ,
	HOST_BUFFER_1			    ,
	HOST_BUFFER_2			    ,
	HOST_BUFFER_3			    ,
	HOST_PRESENT_STATE		    ,
	HOST_HOST_CONTROL_1		    ,
	HOST_POWER_CONTROL		    ,
	HOST_BLOCK_GAP_CONTROL	    ,
	HOST_WAKE_UP_CONTROL	    ,
	HOST_CLOCK_CONTROL		    ,
	HOST_TIMEOUT_CONTROL	    ,
	HOST_SOFTWARE_RESET		    ,
	HOST_INT_STATUS			    ,
	HOST_INT_ENABLE			    ,
	HOST_SIGNAL_ENABLE		    ,
	HOST_ACMD_ERR			    ,
	HOST_HOST_CONTROL_2			,
	HOST_CAPABILITIES			,
	HOST_CAPABILITIES_1		    ,
	HOST_MAX_CURRENT			,
	/* 4C-4F reserved for more max current */
	HOST_SET_ACMD_ERROR			,
	HOST_SET_INT_ERROR			,
	HOST_ADMA_ERROR				,
	/* 55-57 reserved */
	HOST_ADMA_ADDRESS			,

	HOST_PRESET_INIT			 ,
	HOST_PRESET_DEFAILT			 ,
	HOST_PRESET_HIGHSPEED	 	 ,
	HOST_PRESET_SDR12			 ,
	HOST_PRESET_SDR25			 ,
	HOST_PRESET_SDR50			 ,
	HOST_PRESET_SDR104			 ,
	HOST_PRESET_DDR50			 ,
	HOST_SHARED_BUS_CONTROL		 ,

	/* 60-FB reserved */
	HOST_SLOT_INT_STATUS		,
	HOST_HOST_VERSION			
};


class SDWrLogger
{
	public:
		static coreutil::Logger& log() { return mLogger; }

	private:
		static coreutil::Logger mLogger;

		SDWrLogger();
};

#define SD_WLOG_DEBUG() CORE_LOG_DEBUG(SDWrLogger::log())
#define SD_WLOG_INFO()  CORE_LOG_INFO(SDWrLogger::log())
#define SD_WLOG_WARN()  CORE_LOG_WARN(SDWrLogger::log())
#define SD_WLOG_ERROR() CORE_LOG_ERROR(SDWrLogger::log())

#endif
