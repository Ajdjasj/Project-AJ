
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCardFifoTest : public SDCardTest
{
  private:

  public:
    SDCardFifoTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardFifoTest()
    {
    }

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun()
	{
		    SDCardTest::PreRun();
	}

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun()
	{
		    SDCardTest::PostRun();
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		uint8_t Devices = 0, Slots = 0;
		bool Speed = false, BusWidth = true;
		uint8_t autocmd = 0x1;
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 4;
		uint32_t SDdebugReg = 0, FifoValue = 0, InputFIFObit = 11, Data = 0xdeadbeaf;
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("Card detected....\n");
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					sd_logmsg("Initialization Failed....\n");
					return ts::Test::Fail;
				}
				sd_logmsg("Initialization Complete....\n");
				
				//PCI read
				SDdebugReg = read_pcicfg(device, SDHC_PCI_DEBUG, DWORD);
				sd_logmsg("PCI SD Debug register: 0x%x\n", SDdebugReg);
				for( int i=0; i<2; i++ )
				{
					//PCI write
				#if 1
					FifoValue = read_pcicfg(device, SDHC_PCI_DEBUG, DWORD);
					FifoValue = FifoValue ^ (1 << InputFIFObit);
					write_pcicfg(device, SDHC_PCI_DEBUG, DWORD, FifoValue);
				#endif
					
					//Multiple block PIO Read/Write test
					if(!pio_data_transfer(device, slot, Speed, BusWidth, autocmd, card_address, Data, blk_size, blk_count))
					{
						write_pcicfg(device, SDHC_PCI_DEBUG, DWORD, SDdebugReg);
						sd_logmsg("SDHC/SDXC Read/Write Failed for Multiple block....\n");
						return ts::Test::Fail;
					}
					
					SD_LOG_DEBUG() << "SDHC/SDXC PIO Read/Write Passed for Multiple block...." << std::endl;
				
					//Multiple block SDMA Read/Write test
					if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, Speed, BusWidth, autocmd))
					{
						write_pcicfg(device, SDHC_PCI_DEBUG, DWORD, SDdebugReg);
						sd_logmsg("SDMA Multiple block read/write failed\n");
						return ts::Test::Fail;
					}
					
					SD_LOG_DEBUG() << "SDMA Multiple block read/write complete for device "<< device << " and slot " << slot << std::endl;
					
					
					//Multiple block PIO Read/Write test
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_30k,true, true, autocmd,0x02))
					{
						write_pcicfg(device, SDHC_PCI_DEBUG, DWORD, SDdebugReg);
						SD_LOG_DEBUG() << "ADMA read/write failed : "<<std::endl; 
						return ts::Test::Fail;
					}
					write_pcicfg(device, SDHC_PCI_DEBUG, DWORD, SDdebugReg);
					SD_LOG_DEBUG() << "ADMA Multiple block read/write complete for device "<< device << " and slot " << slot << std::endl;
				}
			}
		}
		SD_LOG_DEBUG() << "SDMA Input FIFO test Successful" << std::endl;
        return TestResult; 
    }
};


TServerTestInstance(SDCardFifoTest, SDCardFifoTest);

