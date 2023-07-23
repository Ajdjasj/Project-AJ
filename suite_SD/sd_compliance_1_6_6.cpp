#include "sdcard_test.hpp"

static uint8_t LockCard[18] = {0};

class SDCompliance_1_6_6Test : public SDCardTest
{
  private:

  public:
    SDCompliance_1_6_6Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_1_6_6Test()
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
		
		//Get the card status
		_r1 card_status;
		uint32_t error = 0x00;
		if(!get_card_status( &card_status, &error ))
		{
			printf("Failed for get card status....\n");
			return ts::Test::Fail; 
		}
		
		//1.	Lock with password
		//2.	Unlock with password
		//3.	Lock with password
		//4.	Change the password
		//5.	Unlock with changed password
		if(!card_status.bit.card_is_locked)
		{
			printf("\n");
			//Lock with password
			LockCard[0] = 0x5;
			LockCard[1] = 0x2;
			LockCard[2] = 'a';
			LockCard[3] = 'b';
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("set passwd check lock_unlock function\n");
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
			if((card_status.bit.lock_unlock_fail) || (!card_status.bit.card_is_locked))
			{
				printf("Failed to Set the password\n");
				SD_LOG_DEBUG() << "Failed to Set the password" << std::endl;
				if(!card_status.bit.card_is_locked)
				{
					printf("Card is still unlocked\n");
					SD_LOG_DEBUG() << "Card is still unlocked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
			}
			
			printf("\n");
			//Unlock with password
			LockCard[0] = 0x2;
			LockCard[1] = 0x2;
			LockCard[2] = 'a';
			LockCard[3] = 'b';
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("set passwd check lock_unlock function\n");
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
			if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
			{
				printf("Failed to clear the password\n");
				SD_LOG_DEBUG() << "Failed to clear the password" << std::endl;
				if(card_status.bit.card_is_locked)
				{
					printf("Card is still locked\n");
					SD_LOG_DEBUG() << "Card is still locked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
			}
			
			printf("\n");
			//Lock with password
			LockCard[0] = 0x5;
			LockCard[1] = 0x2;
			LockCard[2] = 'a';
			LockCard[3] = 'b';
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("set passwd check lock_unlock function\n");
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
			if((card_status.bit.lock_unlock_fail) || (!card_status.bit.card_is_locked))
			{
				printf("Failed to Set the password\n");
				SD_LOG_DEBUG() << "Failed to Set the password" << std::endl;
				if(!card_status.bit.card_is_locked)
				{
					printf("Card is still unlocked\n");
					SD_LOG_DEBUG() << "Card is still unlocked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
			}
			
			//Change the password
			LockCard[0] = 0x5;
			LockCard[1] = 0x5;
			LockCard[2] = 'a';
			LockCard[3] = 'b';
			LockCard[4] = 'x';
			LockCard[5] = 'y';
			LockCard[6] = 'z';
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
			if((card_status.bit.lock_unlock_fail) || (!card_status.bit.card_is_locked))
			{
				printf("Failed to Change the password\n");
				SD_LOG_DEBUG() << "Failed to Change the password" << std::endl;
				if(!card_status.bit.card_is_locked)
				{
					printf("Card is still unlocked\n");
					SD_LOG_DEBUG() << "Card is still unlocked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
				
				//Clear the changed password
				if(!card_status.bit.card_is_locked)
				{
					//Clear password
					LockCard[0] = 0x2;
					LockCard[1] = 0x2;
					LockCard[2] = 'a';
					LockCard[3] = 'b';
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
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						printf("Clear password Failed\n");
						TestResult = ts::Test::Fail; 
					}
				}
				TestResult = ts::Test::Fail; 
			}
		
			//Unlock with changed password
			LockCard[0] = 0x2;
			LockCard[1] = 0x3;
			LockCard[2] = 'x';
			LockCard[3] = 'y';
			LockCard[4] = 'z';
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("Failed in lock_unlock function\n");
				return ts::Test::Fail; 
			}
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				printf("Failed in get_card_status function\n");
				TestResult = ts::Test::Fail; 
			}
			//printf("Card Status: 0x%x\n", card_status);
			printf("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
			printf("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
			if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
			{
				printf("Failed to clear the password\n");
				SD_LOG_DEBUG() << "Failed to clear the password" << std::endl;
				if(card_status.bit.card_is_locked)
				{
					printf("Card is still locked\n");
					SD_LOG_DEBUG() << "Card is still locked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
			}
		}
		else
		{
			printf("Card is locked\n");
			TestResult = ts::Test::Fail; 
		}
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 1.6-6 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_1_6_6Test, SDCompliance_1_6_6Test);
