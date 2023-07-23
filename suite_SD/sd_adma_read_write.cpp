
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCardAdmaReadWriteTest : public SDCardTest
{
  private:

  public:
    SDCardAdmaReadWriteTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardAdmaReadWriteTest()
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
		uint8_t autocmd = 0x1;
		uint32_t card_address = 0x00;
		Devices = get_total_pcidevice();
		uint16_t type 		= Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); 
		
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
				SD_LOG_DEBUG() << "Card detected...." << std::endl;
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled = false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "Initialization Failed"<<std::endl;
					return ts::Test::Fail;
				}
					SD_LOG_DEBUG() << "Initialization Complete"<<std::endl;
				
				if(2 == type)
				{
					card_address = 0x5000;
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_30k, false /*Speed*/, false /*Width*/, autocmd,0x02/*32 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 1bit , ADMA select = 32bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
						SD_LOG_DEBUG() << "ADMA read/write Success, Width = 1bit , ADMA select = 32bit address ADMA2"<<std::endl; 
					
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_30k, false /*Speed*/, true /*Width*/, autocmd,0x02/*32 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 4bit , ADMA select = 32bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "ADMA read/write Completed , Width = 4bit , ADMA select = 32bit address ADMA2" << std::endl;
					
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_30k, false /*Speed*/, false /*Width*/, autocmd,0x03/*64 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 1bit , ADMA select = 64bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "ADMA read/write Completed , Width = 1bit , ADMA select = 64bit address ADMA2" << std::endl;
					
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_30k, false /*Speed*/, true /*Width*/, autocmd,0x03/*64 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 4bit , ADMA select = 64bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG()<< "ADMA read/write Completed , Width = 4bit , ADMA select = 64bit address ADMA2" << std::endl;
				}
				else if(1 == type)
				{
					card_address = 0x3000;
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_singleblock, false /*Speed*/, false /*Width*/, 0x00/*Auto commnd not required*/,0x02/*32 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 1bit , ADMA select = 32bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
				 	SD_LOG_DEBUG()<<"ADMA read/write Completed , Width = 1bit , ADMA select = 32bit address ADMA2" <<std::endl;
					
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_singleblock, false /*Speed*/, true /*Width*/, 0x00/*Auto commnd not required*/,0x02/*32 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 4bit , ADMA select = 32bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG()<<"ADMA read/write Completed , Width = 4bit , ADMA select = 32bit address ADMA2" <<std::endl;
					
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_singleblock, false /*Speed*/, false /*Width*/, 0x00/*Auto commnd not required*/,0x03/*64 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 1bit , ADMA select = 64bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "ADMA read/write Completed , Width = 1bit , ADMA select = 64bit address ADMA2" << std::endl;
					
					if(!ADMA2ReadWrite(device, slot, card_address, data_size_singleblock, false /*Speed*/, true /*Width*/, 0x00/*Auto commnd not required*/,0x03/*64 bit ADMA*/))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed , Width = 4bit , ADMA select = 64bit address ADMA2"<<std::endl; 
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() <<"ADMA read/write Completed , Width = 4bit , ADMA select = 64bit address ADMA2" << std::endl;

				}
			}
		}
		
	 	SD_LOG_DEBUG() << "ADMA Read/Write test Completed" << std::endl;
		return TestResult; 
	}
};

TServerTestInstance(SDCardAdmaReadWriteTest, SDCardAdmaReadWriteTest);
