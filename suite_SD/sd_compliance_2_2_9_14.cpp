#include "sdcard_test.hpp"

static uint8_t LockCard[18] = {0};

class SDCompliance_2_2_9_14Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_2_9_14Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_2_9_14Test()
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
		
		//Devices = Parameter<uint16_t>("device",1, coreutil::FmtFlags(std::ios::dec)); 
		//Slots = Parameter<uint16_t>("slots",1, coreutil::FmtFlags(std::ios::dec));
		strcpy(password, (const char *)Parameter<std::string>("passcode", " ").c_str());
		printf("Password: %s\n", password);
		
		//Initialization
		//_init_status init_status;
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
		//Set password and lock the card
		if((!card_status.bit.card_is_locked) || (card_status.bit.lock_unlock_fail))
		{
			printf("\n");
			//Set the password the card
			LockCard[0] = 0x1;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
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
			
			//Set the password the card
			LockCard[0] = 0x4;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
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
			if(!card_status.bit.card_is_locked)
			{
				printf("Failed to lock the card\n");
				TestResult = ts::Test::Fail; 
			}
			else if(card_status.bit.lock_unlock_fail)
			{
				printf("lock_unlock_fail error set\n");
				TestResult = ts::Test::Fail; 
			}
	
		#if 0
			//Clear password and Unlock the card
			LockCard[0] = 0x2;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
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
		#endif
		#if 0
			if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
			{
				printf("Failed to Reset the password\n");
				if(card_status.bit.card_is_locked)
					printf("Card is still locked\n");
				
				return ts::Test::Fail; 
			}
		#endif
		}
		#if 0
		else
		{
			if(card_status.bit.lock_unlock_fail)
				printf("Lock/Unlock Error already exist\n");
			else
				printf("Card is locked\n");
			
			return ts::Test::Fail; 
		}
		#endif
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.2-9/2.2-14 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_2_2_9_14Test, SDCompliance_2_2_9_14Test);