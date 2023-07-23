#include "sdcard_test.hpp"


static uint8_t LockCard[18] = {0};

class SDCompliance_1_6_3Test : public SDCardTest
{
  private:

  public:
    SDCompliance_1_6_3Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_1_6_3Test()
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
		//uint8_t Devices = 0, Slots = 0;
		uint8_t device = 0, slot = 0;
		char password[32] = "0123456789ABCDEF";
		_r1 card_status;
		uint32_t error = 0x00;
		
		uint16_t speed_mode = Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));
		uint16_t type = Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex));
		//Devices = Parameter<uint16_t>("device",1, coreutil::FmtFlags(std::ios::dec)); 
		//Slots = Parameter<uint16_t>("slots",1, coreutil::FmtFlags(std::ios::dec));
		strcpy(password, (const char *)Parameter<std::string>("passcode", " ").c_str());
		printf("Password: %s\n", password);

		//Initialization
		bool voltage_switch_enabled =false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			printf("Initialization Failed....\n");
			SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
			return ts::Test::Fail;
		}
		printf("Initialization Complete....\n");
		
		error = 0x00;
		if(!get_card_status( &card_status, &error ))
		{
			printf("Failed in get_card_status function\n");
			return ts::Test::Fail; 
		}
		
		if(card_status.bit.card_is_locked)
		{
			printf("\n");
	
			//Unlock the card
			LockCard[0] = 0x2;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("Failed in lock_unlock function\n");
				return ts::Test::Fail; 
			}
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				printf("Failed in get_card_status function\n");
				return ts::Test::Fail; 
			}
			//printf("Card Status: 0x%x\n", card_status);
			printf("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
			printf("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
		#if 1
			if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
			{
				printf("Failed to reset the password\n");
				SD_LOG_DEBUG() << "Failed to reset the password" << std::endl;
				if(card_status.bit.card_is_locked)
				{
					printf("Card is still locked\n");
					SD_LOG_DEBUG() << "Card is still locked" << std::endl;
				}
				
				TestResult = ts::Test::Fail; 
			}
		#endif
		}
		#if 0
		else
		{
			printf("Card is not locked\n");
			return ts::Test::Fail; 
		}
		SD_LOG_DEBUG() << "Card unlock: Completed"<< std::endl;
		#endif
		
		//ACMD6 and CMD6
		_function function;
		_r1 status;
		//Get Card Supported Mode
		if(!get_supported_functions(device , slot, true, false, &function, &status))
		{
			SD_LOG_DEBUG() << "get_supported_functions failed"<< std::endl;
			TestResult = ts::Test::Fail;
		}
		
		//Check Requested mode is supported by the card
		if(!(speed_mode & function.group1))
		{
			SD_LOG_DEBUG() << "Selected Speed Mode is not supported by the Card" << std::endl;
			return ts::Test::Fail;			
		}
		
		//Change the speed mode
		if(1 == type)
		{
			if(!set_normal_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
			{
				SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
				TestResult = ts::Test::Fail;
			}
			SD_LOG_DEBUG() << "Set Normal speed mode: Completed"<< std::endl;
		}
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 1.6-3 test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCompliance_1_6_3Test, SDCompliance_1_6_3Test);
