
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"

static uint8_t LockCard[18] = {0};

class SDCardLockUnlockTest : public SDCardTest
{
  private:

  public:
    SDCardLockUnlockTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardLockUnlockTest()
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
		uint8_t device=0;
		uint8_t slot=0;
		
		#if SD_PCI
		uint8_t Devices = 0, Slots = 0;
		
		Devices = get_total_pcidevice();
		for( device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for( slot=0; slot<Slots; slot++)
			{
		#endif
			
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Insert test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail; 
				}
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					sd_logmsg("Initialization Failed....\n");
					SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("Initialization Complete....\n");
				
				//Get the current status
				_r1 card_status;
				uint32_t error = 0x00;
				if(!get_card_status( &card_status, &error ))
				{
					sd_logmsg("Failed for get card status....\n");
					return ts::Test::Fail; 
				}
				if(card_status.bit.current_state != stby)
				{
					sd_logmsg("Card is not in standby state\n");
					return ts::Test::Fail; 
				}
				sd_logmsg("\nCard Status: 0x%x\n", card_status);
				
				//First Test
				//Set password and clear passwrd test
				if((!card_status.bit.card_is_locked) || (card_status.bit.lock_unlock_fail))
				{
					sd_logmsg("\n");
					//Set the password the card
					LockCard[0] = 0x1;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Failed to Set the password\n");
						return ts::Test::Fail; 
					}
			
					//Clear password and Unlock the card
					LockCard[0] = 0x2;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Failed to Reset the password\n");
						return ts::Test::Fail; 
					}
				}
				else
				{
					if(card_status.bit.lock_unlock_fail)
						sd_logmsg("Lock/Unlock Error already exist\n");
					else
						sd_logmsg("Card is locked\n");
					
					return ts::Test::Fail; 
				}
				sd_logmsg("First test Passed\n");
			
			
				//Second Test
				//Clear password even though no password set previously
				if(!card_status.bit.card_is_locked)
				{
					sd_logmsg("\n");
					//Clear password and Unlock the card
					LockCard[0] = 0x2;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Failed to update the lock/unlock_failed error\n");
						return ts::Test::Fail; 
					}
				}
				else
				{
					sd_logmsg("Card is locked\n");
					return ts::Test::Fail; 
				}
				sd_logmsg("Second test Passed\n");
				
				
				//Third Test
				//Set the password, change the password and clear the password
				if(!card_status.bit.card_is_locked)
				{
					sd_logmsg("\n");
					//Set the password
					LockCard[0] = 0x1;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("set passwd check lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Set password Failed\n");
						return ts::Test::Fail; 
					}
					
					//Change the password
					LockCard[0] = 0x1;
					LockCard[1] = 0x5;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					LockCard[4] = 'm';
					LockCard[5] = 's';
					LockCard[6] = 'm';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Change password Failed\n");
							
						//Clear the changed password
						if(!card_status.bit.card_is_locked)
						{
							//Clear password
							LockCard[0] = 0x2;
							LockCard[1] = 0x2;
							LockCard[2] = 'm';
							LockCard[3] = 'a';
							if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
							{
								sd_logmsg("Failed in lock_unlock function\n");
								return ts::Test::Fail; 
							}
							error = 0x00;
							if(!get_card_status( &card_status, &error ))
							{
								sd_logmsg("Failed in get_card_status function\n");
								return ts::Test::Fail; 
							}
							sd_logmsg("Card Status: 0x%x\n", card_status);
							sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
							sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
							if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
							{
								sd_logmsg("Clear password Failed\n");
								return ts::Test::Fail; 
							}
						}
						return ts::Test::Fail; 
					}
				
					//Clear the changed password
					LockCard[0] = 0x2;
					LockCard[1] = 0x3;
					LockCard[2] = 'm';
					LockCard[3] = 's';
					LockCard[4] = 'm';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Clear password Failed\n");
						return ts::Test::Fail; 
					}
				}
				else
				{
					sd_logmsg("Card is locked\n");
					return ts::Test::Fail; 
				}
				sd_logmsg("Third test Passed\n");
				
				
				//Fourth Test
				//Set the password and lock the card then 
				//change the password and clear the password
				if(!card_status.bit.card_is_locked)
				{
					sd_logmsg("\n");
					//Set the password and lock card
					LockCard[0] = 0x5;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((!card_status.bit.card_is_locked) || card_status.bit.lock_unlock_fail)
					{
						sd_logmsg("Failed to Set the password and lock card\n");
						if(!card_status.bit.card_is_locked)
							return ts::Test::Fail; 
					}
					
					//Change the password
					LockCard[0] = 0x1;
					LockCard[1] = 0x5;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					LockCard[4] = 'm';
					LockCard[5] = 's';
					LockCard[6] = 'm';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Change password Failed\n");
							
						//Clear the changed password
						if(!card_status.bit.card_is_locked)
						{
							//Clear password
							LockCard[0] = 0x2;
							LockCard[1] = 0x2;
							LockCard[2] = 'm';
							LockCard[3] = 'a';
							if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
							{
								sd_logmsg("Failed in lock_unlock function\n");
								return ts::Test::Fail; 
							}
									
							error = 0x00;
							if(!get_card_status( &card_status, &error ))
							{
								sd_logmsg("Failed in get_card_status function\n");
								return ts::Test::Fail; 
							}
							sd_logmsg("Card Status: 0x%x\n", card_status);
							sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
							sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
							if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
							{
								sd_logmsg("Clear password Failed\n");
								return ts::Test::Fail; 
							}
						}
						return ts::Test::Fail; 
					}
					
					//Unlock and clear the pasword
					LockCard[0] = 0x2;
					LockCard[1] = 0x3;
					LockCard[2] = 'm';
					LockCard[3] = 's';
					LockCard[4] = 'm';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if(card_status.bit.card_is_locked || card_status.bit.lock_unlock_fail)
					{
						sd_logmsg("Failed to Unlock and clear the pasword\n");
						return ts::Test::Fail; 
					}
				}
				else
				{
					sd_logmsg("Card is locked\n");
					return ts::Test::Fail; 
				}
				sd_logmsg("Fourth test Passed\n");
				
			
				//Fifth Test
				//Send lock/unlock mode as 0x0 in the following condition,
				//1. Unlcok and password exist
				//2. Unlcok and password exist and 
				//3. Unlcok and password clear
				if(!card_status.bit.card_is_locked)
				{
					sd_logmsg("\n");
					//Set the password
					LockCard[0] = 0x5;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if(!card_status.bit.card_is_locked)
					{
						sd_logmsg("Set password Failed\n");
						return ts::Test::Fail; 
					}
					
					//Send lock/unlock mode as 0x0 in lcok and password exist
					LockCard[0] = 0x0;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if(card_status.bit.card_is_locked)
					{
						sd_logmsg("Fifth1: Failed to update lock/unlock mode as 0x0\n");
						TestResult = ts::Test::Fail;
					}
					
					//Send lock/unlick mode as 0x0 in Unlcok and password exist
					LockCard[0] = 0x0;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if( ( card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
					{
						sd_logmsg("Fifth2: Failed to update lock/unlock mode as 0x0\n");
						return ts::Test::Fail; 
					}
					
					//Clear the password
					LockCard[0] = 0x2;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status(&card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if(card_status.bit.lock_unlock_fail)
					{
						sd_logmsg("Clear password Failed\n");
						return ts::Test::Fail; 
					}
					
					//Send lock/unlick mode as 0x0 in Unlcok and password clear
					LockCard[0] = 0x0;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, true, true, &card_status))
					{
						sd_logmsg("Failed in lock_unlock function\n");
						return ts::Test::Fail; 
					}
					error = 0x00;
					if(!get_card_status( &card_status, &error ))
					{
						sd_logmsg("Failed in get_card_status function\n");
						return ts::Test::Fail; 
					}
					sd_logmsg("Card Status: 0x%x\n", card_status);
					sd_logmsg("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
					sd_logmsg("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
					if( (card_status.bit.lock_unlock_fail)|| (card_status.bit.card_is_locked))
					{
						sd_logmsg("Fifth3: Failed to update lock/unlock mode as 0x0\n");
						return ts::Test::Fail; 
					}
				}
				else
				{
					sd_logmsg("Card is locked\n");
					return ts::Test::Fail; 
				}
				sd_logmsg("Fifth test Passed\n");
		#if SD_PCI		
			}
		}
		#endif
		
		SD_LOG_DEBUG() << "SDCard Lock/Unlock Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardLockUnlockTest, SDCardLockUnlockTest);
