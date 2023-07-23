
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"

static uint8_t LockCard[18] = {0};

class SDCardForceEraseTest : public SDCardTest
{
  private:

  public:
    SDCardForceEraseTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardForceEraseTest()
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
		int device=0; 
		int slot=0;
		
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
					SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("Initialization Complete....\n");
				
				//Get the current status
				_r1 card_status;
				uint32_t error = 0x00;
				if(!get_card_status( &card_status, &error ))
				{
					SD_LOG_DEBUG() << "Failed to get card status" << std::endl;
					return ts::Test::Fail;
				}
				if(card_status.bit.current_state != stby)
				{
					sd_logmsg("Card is not in standby state\n");
					return ts::Test::Fail;
				}
				
				sd_logmsg("\nCard Status: 0x%x\n", card_status);
				
				#if 1
				if(!card_status.bit.card_is_locked)
				{
					sd_logmsg("\n");
					//Set the password and lock card
					LockCard[0] = 0x5;
					LockCard[1] = 0x2;
					LockCard[2] = 'm';
					LockCard[3] = 'a';
					if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
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
				}
				#endif
				
				//Force Erase
				if(card_status.bit.card_is_locked)
				{
					sd_logmsg("\n");
					sd_logmsg("----Card is locked ----\n");
					LockCard[0] = 0x08;
					if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
					{
						sd_logmsg("Failed in force_unlock function\n");
						return ts::Test::Fail;
					}
					error = 0x00;
					if(!get_card_status(  &card_status, &error ))
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
				}
				else
				{
					sd_logmsg("Force Erase will work on locked card\n");
				}
		#if SD_PCI
			}
		}
		#endif
		
		SD_LOG_DEBUG() << "SDCard Force Erase Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardForceEraseTest, SDCardForceEraseTest);
