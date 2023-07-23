#include "sdcard_test.hpp"

static uint8_t LockCard[18] = {0};

class SDCompliance_3_4_16Test : public SDCardTest
{
  private:

  public:
    SDCompliance_3_4_16Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_3_4_16Test()
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
		printf("Please Capture the trace and press Enter to exit...\n");
		getchar();
		
		SDCardTest::PostRun();
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		uint8_t device = 0, slot = 0;
		uint16_t speed_mode = 1;
		uint16_t type 		= 2;
		uint8_t autocmd = 0x01;
		uint32_t card_address = 0x2000;// blk_size = 512, blk_count = 100;
		//_function function;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled =true;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			printf("Initialization Failed....\n");
			SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
			return ts::Test::Fail;
		}
		printf("Initialization Complete....\n");
		
		//Get the card status
		_r1 card_status;
		uint32_t error = 0x00;
		if(!get_card_status( &card_status, &error ))
		{
			printf("Failed for get card status....\n");
			return ts::Test::Fail; 
		}

#if 0
		//Get Card Supported Mode
		if(!get_supported_functions(device , slot, true, true, &function, &card_status))
		{
			SD_LOG_DEBUG() << "get_supported_functions failed"<< std::endl;
			TestResult =  ts::Test::Fail;
		}
		
		//Check Requested mode is supported by the card
		if(!(speed_mode & function.group1))
		{
			SD_LOG_DEBUG() << "Selected Speed Mode is not supported by the Card" << std::endl;
			return  ts::Test::Fail;			
		}
#endif		
		if(2 == type)
		{
			if(voltage_switch_enabled == true)
			{
				_reg sd_register;
				uint32_t clk_multiplier = 0x00;
				
				SET_SD_REGISTER_READ(&sd_register,HOST_CAPABILITIES_1,device,slot);
				read_host_reg(&sd_register);
				clk_multiplier = ((sd_register.sd_reg.value & 0x00FF0000) >> 16);
				if(0x00 == clk_multiplier)
				{
					if(!set_UHS_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
					{
						SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}						
				}
				else
				{
					if(!set_UHS_mode( device,  slot,  speed_mode, PROGRAMABLE_CLK_SELECTION_MODE))
					{
						SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}
				}	
				SD_LOG_DEBUG() << "Set UHS speed mode Completed"<< std::endl;
			}
			else
			{
				SD_LOG_DEBUG() << "UHS mode : Not possible in the current Configuration"<< std::endl;
				if(init_status.host.voltage_switch == false)
					SD_LOG_DEBUG() << "Host doesnot support Voltage switch for switching the mode to UHS"<<std::endl;
				if(init_status.card.voltage_switch == false)
					SD_LOG_DEBUG() << "Card doesnot support Voltage switch for switching the mode to UHS"<<std::endl;
				TestResult =  ts::Test::Fail;	
			}
		}
		else
		{
			SD_LOG_DEBUG() << "Wrong Type selected by the user from build.xml , type should be 1 for Normal modes and 2 for UHS modes"<< std::endl;
			TestResult =  ts::Test::Fail;
		}
		
		
		//ADMA Data transfer 500Kb
		if(!SDCardTest::ADMA2Write_3_4_16(device, slot, card_address, data_size_500k, true /*Speed*/, true /*Width*/, autocmd,0x02, LockCard))
		{
			SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
			TestResult =  ts::Test::Fail;
		}
		SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;
		



		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 3.4-16 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_3_4_16Test, SDCompliance_3_4_16Test);