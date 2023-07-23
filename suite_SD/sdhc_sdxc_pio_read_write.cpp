
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCardSdhcSdxcPioReadWriteTest : public SDCardTest
{
  private:

  public:
    SDCardSdhcSdxcPioReadWriteTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardSdhcSdxcPioReadWriteTest()
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
		bool PartialRead = false, PartialWrite = false;
		bool Speed = false;
		bool BusWidth = false;
		uint8_t autocmd = 0x0;
		uint32_t card_address = 0x4000, blk_size = 512, blk_count = 0, Data = 0xbeafdead;
		uint16_t test_type = Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex));
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled = false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "Initialization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() <<"Initialization Complete"<< std::endl;
				
				//Single block Read/Write test
				if(1 == test_type)
				{
					if(!pio_data_transfer(device, slot, Speed, false, autocmd, card_address, Data, blk_size, blk_count))
					{
						SD_LOG_DEBUG() <<"Single block PIO Read/Write Failed,Bus width : 1Bit" << std::endl;
						return ts::Test::Fail; 
					}
					card_address = 0x5000;
					if(!pio_data_transfer(device, slot, Speed, true, autocmd, card_address, Data, blk_size, blk_count))
					{
						SD_LOG_DEBUG() <<"Single block PIO Read/Write Failed,Bus width : 4Bit" << std::endl;
						return ts::Test::Fail; 
					}
					SD_LOG_DEBUG() <<"Single block PIO Read/Write Completed"<< std::endl;
				}
				
				if(2 == test_type)
				{				
					//Multiple block Read/Write test
					autocmd = 0x1;
					blk_size = 512;
					blk_count = 100;
					card_address = 0x6000;
					Data = 0xdeadbeaf;
					if(!pio_data_transfer(device, slot, Speed, false, autocmd, card_address, Data, blk_size, blk_count))
					{
						SD_LOG_DEBUG() <<"Multi block PIO Read/Write Failed,Bus width : 1Bit" << std::endl;
						return ts::Test::Fail; 
					}
					card_address = 0x7000;
					if(!pio_data_transfer(device, slot, Speed, true, autocmd, card_address, Data, blk_size, blk_count))
					{
						SD_LOG_DEBUG() <<"Single block PIO Read/Write Failed,Bus width : 4Bit" << std::endl;
						return ts::Test::Fail; 
					}
					SD_LOG_DEBUG() <<"Multi block PIO Read/Write Completed"<< std::endl;		
				}
			}
		}
		return TestResult; 
    }
};

TServerTestInstance(SDCardSdhcSdxcPioReadWriteTest, SDCardSdhcSdxcPioReadWriteTest);
