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

#ifndef _SDCARD_TEST_H
#define _SDCARD_TEST_H

#include <ts_test.h>
#include "core_logger.h"
#include "resource_manager.h"


#include <assert.h>
#include "host.h"
#include <ts_test.h>
#include <env_device.h>
#include <env_time.h>
#include <env_device_interface.h>
using namespace std;

//Insert and Remove Simulation
#define CARD_INSTERT_STATUS_ENABLE_BIT 6
#define CARD_REMOVAL_STATUS_ENABLE_BIT 7

#define CARD_INSTERT_STATUS_BIT 6
#define CARD_REMOVAL_STATUS_BIT 7

#define CARD_DETECT_TEST_LEVEL 6
#define CARD_DETECT_SIGNAL_SELECT_ 7

#define CARD_INSERT_BIT 16
#define MAX_BLOCK_PER_ONE_TABLE_ENTRY 128 //block , 1block = 512byte = .5Kb
#define ONE_BLOCK 0x200
enum data_size
{
	data_size_singleblock	= 1,
	data_size_30k 			= 60,
	data_size_70k 			= 140,
	data_size_100k 			= 200,
	data_size_500k			= 1000,
	data_size_1mb			= 2000,
	data_size_1280k			= 2560,
	data_size_2mb			= 4000,
	data_size_3mb			= 6000,	
	data_size_4mb			= 8000,
	data_size_5mb			= 10000,
	data_size_6mb			= 12000,
	data_size_7mb			= 14000,
	data_size_8mb			= 16000,
	data_size_9mb			= 18000,	
	data_size_10mb			= 20000,
	data_size_50mb			= 100000,
	data_size_500mb			= 1000000
};

class SDCardTest : public ts::Test
{

  public:
    //PTCOREWRAPPER_USER_CONTEXT     mTCoreUserContext;
	//TCORE_BUS_INFO     sBusInfo;
	//Allocate physical memory
	env::System *device1;
	env::System *device2;
	env::System *device3,*device4,*device5,*device6,*device7,*device8,*device9,*device10,*device11,*device12,*device13,*device14,*device15,*device16,*device17,*device18,*device19;
	env::Resource *resource1, *resource2, *resource3, *resource4, *resource5, *resource6, *resource7, *resource8, *resource9, *resource10, *resource11, *resource12, *resource13, *resource14, *resource15, *resource16, *resource17, *resource18, *resource19;
	
	env::Resource* dma_table1;
	env::Resource* dma_table2;
	env::Resource* dma_table3;
	env::Resource* dma_table4;
	env_device_interface env_dev;
  protected:

    coreutil::Logger    m_lg;
    Target*             m_target_device ;
    Resource_manager*   m_resource_mgr;
	

	_init_status init_status;
    SDCardTest(const TServerTestFactory& Factory);
    virtual ~SDCardTest();

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun();

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun();
	
	#if SD_PCI
		void EnableMMIOBusMaster(uint32_t device);
	#elif SD_MMIO	
		void EnableMMIOBusMaster();
	#endif
	
	//ADMA2ReadWrite_with_asyncabort
	
	//bool Initalization(uint32_t Device, uint32_t slot, bool voltage_switch_enabled, _init_status *init_status);
	bool Initalization(uint32_t Device, uint32_t slot, bool voltage_switch_enabled);
	void DMAMemoryAlloc();
	bool SDMAReadWrite(uint32_t card_address, uint32_t memsize,uint32_t blk_size, uint32_t blk_count, bool Speed, bool BusWidth, uint8_t autocmd);
	bool ADMA2ReadWrite(uint32_t device, uint32_t slot, uint32_t card_address, data_size size,bool Speed, bool BusWidth, uint8_t autocmd, uint8_t adma_type);
	bool ADMA2Write_3_4_16(uint32_t device, uint32_t slot, uint32_t card_address, data_size size,bool Speed, bool BusWidth, uint8_t autocmd, uint8_t adma_type, uint8_t* LockData);
	bool pio_data_transfer(uint32_t device, uint32_t slot, bool speed, bool width, uint8_t auto_cmd, uint32_t card_address, uint32_t data, uint32_t block_size, uint32_t block_count);
	bool ADMA2ReadWrite_with_block_gap(uint32_t card_address,  uint32_t blk_size, uint32_t blk_count, bool Speed, bool BusWidth, uint8_t autocmd);
	bool ADMA2ReadWrite_with_asyncabort(uint32_t card_address,  uint32_t blk_size, uint32_t blk_count, bool Speed, bool BusWidth, uint8_t autocmd);
	void ADMAFillTable_for_write(uint32_t block_count,uint8_t adma_type);
	bool ADMAFillTable_for_read(uint32_t block_count,uint8_t adma_type);
	void FillBufferADMA();
	void FillBufferSDMA();
	void ClearBuffer();
	bool enable_device(uint32_t device, bool status);
};

//EnableMMIOBusMaster
//ADMA2ReadWrite_with_block_gap

class SDLogger
{
public:
    static coreutil::Logger& log() { return mLogger; }

private:
    static coreutil::Logger mLogger;

    SDLogger();
};

#define SD_LOG_DEBUG() CORE_LOG_DEBUG(SDLogger::log())
#define SD_LOG_INFO()  CORE_LOG_INFO(SDLogger::log())
#define SD_LOG_WARN()  CORE_LOG_WARN(SDLogger::log())
#define SD_LOG_ERROR() CORE_LOG_ERROR(SDLogger::log())
void sd_logmsg(const char *lpOutputString, ...);
#endif
