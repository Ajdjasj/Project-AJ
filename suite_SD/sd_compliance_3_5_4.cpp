#include "sdcard_test.hpp"

static uint8_t LockCard[18] = {0};

class SDCompliance_3_5_4Test : public SDCardTest
{
  private:

  public:
    SDCompliance_3_5_4Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_3_5_4Test()
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

		/*
		1. Initialize the host and card
		2. check lock/unlock status and unlock the card
		3. Set DDR50 speed mode
		4. send CMD42 with even block length
		5. or xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

		*/
        
		Result TestResult = ts::Test::Pass;
		//uint8_t Devices = 0, Slots = 0;
		uint8_t device = 0, slot = 0;
		char password[32] = "0123456789ABCDEF";
		//uint8_t buswidth = 0x02;

		_function function;
		_r1 card_status;
		//uint8_t autocmd = 0x01;
		//uint32_t card_address = 0x2000; //, blk_size = 512, blk_count = 100;

		
		uint32_t error = 0x00;
		
		//uint16_t speed_mode = Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));
		//uint16_t type = Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); //If type == 1 it is normal mode, if == 2 it is UHS mode
		//Devices = Parameter<uint16_t>("device",1, coreutil::FmtFlags(std::ios::dec)); 
		//Slots = Parameter<uint16_t>("slots",1, coreutil::FmtFlags(std::ios::dec));
		//strcpy(password, (const char *)Parameter<std::string>("passcode", " ").c_str());


		uint16_t speed_mode = 0x10;
		uint16_t type = 1;
		//Devices = 1;
		//Slots = 1;
		//strcpy(password, (const char *)Parameter<std::string>("passcode", " ").c_str());


		printf("Password: %s\n", password);
		
		//Initialization
		bool voltage_switch_enabled =true;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
			return ts::Test::Fail;
		}
		SD_LOG_DEBUG() << "Initialization Complete....\n" << std::endl;
		//
		#if 1		
		//Set the password the card
			LockCard[0] = 0x5;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			//if(!lock_unlock(device, slot, LockCard, true, buswidth, &card_status))
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
			{
				SD_LOG_DEBUG() <<"Failed in lock_unlock function\n"<< std::endl;
				return ts::Test::Fail; 
			}
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				SD_LOG_DEBUG() << "Failed in get_card_status function\n"<< std::endl;
				return ts::Test::Fail; 
			}
			//printf("Card Status: 0x%x\n", card_status);
			//printf("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
			//printf("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);

			SD_LOG_DEBUG() <<"Card Status:0x " << std::endl;
			SD_LOG_DEBUG() <<"Card Error status:0x "<<std::hex<< card_status.bit.lock_unlock_fail << std::endl;
			SD_LOG_DEBUG() <<"Card Lock status:0x"<<std::hex << card_status.bit.card_is_locked<< std::endl;
			
			if((card_status.bit.lock_unlock_fail) || (!card_status.bit.card_is_locked))
			{
				//printf("Failed to Set the password\n");
				SD_LOG_DEBUG() << "Failed to Set the password" << std::endl;
				if(!card_status.bit.card_is_locked)
				{
					//printf("Card is still unlocked\n");
					SD_LOG_DEBUG() << "Card is still unlocked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
			}
	#if 0
			if(!card_status.bit.card_is_locked)
			{
				SD_LOG_DEBUG() << "Failed to lock the card\n"<< std::endl;
				TestResult = ts::Test::Fail; 
			}
			else if(card_status.bit.lock_unlock_fail)
			{
				SD_LOG_DEBUG() << "lock_unlock_fail error set\n"<< std::endl;
				TestResult = ts::Test::Fail; 
			}

	#endif

#endif 

#if 1
		//**************************************************
		//3. check lock/unlock status and unlock the card
		if(card_status.bit.card_is_locked)
		{
			//SD_LOG_DEBUG() <<"\n"<< std::endl;
	
			SD_LOG_DEBUG() <<"card before unlock...."<< std::endl;
			//Unlock the card
			LockCard[0] = 0x2;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			
			//lock_unlock(uint32_t device, uint32_t slot,uint8_t *lock_card_data,bool speed, bool bus_width ,_r1 *status)
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
			{
				SD_LOG_DEBUG() <<"Failed in lock_unlock function\n"<< std::endl;
				return ts::Test::Fail; 
			}
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				SD_LOG_DEBUG() <<"Failed in get_card_status function\n"<< std::endl;
				return ts::Test::Fail; 
			}
			SD_LOG_DEBUG() <<"Card Status: " << std::endl;
			SD_LOG_DEBUG() <<"Card Error status:0x%x "<<std::hex<< card_status.bit.lock_unlock_fail << std::endl;
			SD_LOG_DEBUG() <<"Card Lock status:0x%x"<<std::hex << card_status.bit.card_is_locked<< std::endl;
		
		#if 1
			if((card_status.bit.lock_unlock_fail) /* || (card_status.bit.card_is_locked)*/)
			{
				
				SD_LOG_DEBUG() << "Failed to reset the password" << std::endl;
				if(card_status.bit.card_is_locked)
				{
					
					SD_LOG_DEBUG() << "Card is still locked" << std::endl;
				}
				else
				{
					SD_LOG_DEBUG() << "Card is  unlocked" << std::endl;
				}
				
				return ts::Test::Fail; 
			}

		#endif
	}
#endif

#if 0
		//rrrr
			//**************************************************
		//3. check lock/unlock status and unlock the card
		if(card_status.bit.card_is_locked)
		{
			//SD_LOG_DEBUG() <<"\n"<< std::endl;
	
			SD_LOG_DEBUG() <<"card force erase..."<< std::endl;
			//Unlock the card
			LockCard[0] = 0x0;
			LockCard[1] = 16;
			//Copy the Password
			for( int i=2; i<18; i++)
			{
				LockCard[i] = password[i-2];
			}
			
			//lock_unlock(uint32_t device, uint32_t slot,uint8_t *lock_card_data,bool speed, bool bus_width ,_r1 *status)
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
			{
				SD_LOG_DEBUG() <<"Failed in lock_unlock function\n"<< std::endl;
				return ts::Test::Fail; 
			}
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				SD_LOG_DEBUG() <<"Failed in get_card_status function\n"<< std::endl;
				return ts::Test::Fail; 
			}
			SD_LOG_DEBUG() <<"Card Status: " << std::endl;
			SD_LOG_DEBUG() <<"Card Error status:0x%x "<<std::hex<< card_status.bit.lock_unlock_fail << std::endl;
			SD_LOG_DEBUG() <<"Card Lock status:0x%x"<<std::hex << card_status.bit.card_is_locked<< std::endl;
		
		
				
			
		if((card_status.bit.lock_unlock_fail) /* || (card_status.bit.card_is_locked)*/)
		{
				
			SD_LOG_DEBUG() << "Failed to reset the password" << std::endl;
				
			return ts::Test::Fail; 
		}

		if(card_status.bit.card_is_locked)
		{
					
			SD_LOG_DEBUG() << "Card is still locked" << std::endl;
		}
		else
		{
			SD_LOG_DEBUG() << "Card is  unlocked" << std::endl;
		}
				

		
	}

#endif
		//rrrrr
		
		//*********************************************
		//	2. Set DDR50 speed mode
		//Get Card Supported Mode
		if(!get_supported_functions(device , slot, false, false, &function, &card_status))
		{
			//SD_LOG_DEBUG()<< "get_supported_functions failed"<< std::endl;
			SD_LOG_DEBUG()<< "get_supported_functions failed"<< std::endl;
			return ts::Test::Fail;
		}
				
		//Check Requested mode is supported by the card
		if(!(speed_mode & function.group1))
		{
			//SD_LOG_DEBUG()<< "Selected Speed Mode is not supported by the Card" << std::endl;
			SD_LOG_DEBUG()<< "Selected Speed Mode is not supported by the Card" << std::endl;
			return ts::Test::Fail;			
		}
				
		//Change the speed mode
		if(1 == type)
		{
			SD_LOG_DEBUG() << "Volatage switch variable = " << init_status.result.voltage_switch << std::endl;
			if(init_status.result.voltage_switch == true)
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
						//SD_LOG_DEBUG()<< "set speed mode failed"<< std::endl;
						SD_LOG_DEBUG()<< "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}						
				}
				else
				{
					if(!set_UHS_mode( device,  slot,  speed_mode, PROGRAMABLE_CLK_SELECTION_MODE))
					{
						//SD_LOG_DEBUG()<< "set speed mode failed"<< std::endl;
						SD_LOG_DEBUG()<< "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}
				}	
				//SD_LOG_DEBUG()<< "Set UHS speed mode Completed"<< std::endl;
				SD_LOG_DEBUG() << "Set UHS speed mode: DDR50 Completed"<< std::endl;
			}
			else
			{
				//SD_LOG_DEBUG()<< "UHS mode : Not possible in the current Configuration"<< std::endl;
				SD_LOG_DEBUG() << "UHS mode : Not possible in the current Configuration"<< std::endl;
				if(init_status.host.voltage_switch == false)
					SD_LOG_DEBUG() << "Host doesnot support Voltage switch for switching the mode to UHS"<<std::endl;
				if(init_status.card.voltage_switch == false)
					//SD_LOG_DEBUG()<< "Card doesnot support Voltage switch for switching the mode to UHS"<<std::endl;
					SD_LOG_DEBUG() << "Card doesnot support Voltage switch for switching the mode to UHS"<<std::endl;
				return ts::Test::Fail;	
			}
		}
		else
		{
			//SD_LOG_DEBUG()<< "Wrong Type selected by the user from build.xml , type should be 1 for Normal modes and 2 for UHS modes"<< std::endl;
			SD_LOG_DEBUG()<< "Wrong Type selected by the user from build.xml , type should be 1 for Normal modes and 2 for UHS modes"<< std::endl;
			return ts::Test::Fail;
		}

		
		error = 0x00;
		if(!get_card_status(&card_status, &error ))
		{
			SD_LOG_DEBUG() <<"Failed in get_card_status function\n"<< std::endl;
			return ts::Test::Fail; 
		}

		SD_LOG_DEBUG() << "card_status.bit.card_is_locked = " << std::hex<< card_status.bit.card_is_locked << std::endl;

		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 3.4-16 test Successful" << std::endl;		
			
	return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_3_5_4Test, SDCompliance_3_5_4Test);