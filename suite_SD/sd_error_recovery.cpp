
#include "sdcard_test.hpp"
#include "host_types.h"

class SDErrorRecoveryTest : public SDCardTest
{
  private:

  public:
    SDErrorRecoveryTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDErrorRecoveryTest()
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

	bool force_normal_int(uint32_t device, uint32_t slot, uint32_t error)
{
	if(!force_event(device, slot, (error << 16)))
	{
		return false;
	}
	return true;
}

bool force_auto_command(uint32_t device, uint32_t slot, uint32_t error)
{
	if(!force_event(device, slot, error))
	{
		return false;
	}
	return true;
}
#if 0
bool recover_autocmd_error()
{
}

bool inject_error()
{
}
#endif

bool do_data_transfer(uint32_t device , uint32_t slot , bool speed, uint32_t card_address, uint32_t blk_size, uint32_t blk_count)
{
	//Do PIO Data transfer
	if(!SDCardTest::pio_data_transfer(device, slot, speed , true , 0x01 , card_address , 0xDEADBEEF , blk_size , blk_count ))
	{
		SD_LOG_DEBUG() << "PIO Data transfer Failed"<< std::endl;
		return false;
	}	
	SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
	
    
	//Read/Write Data and Compare the data
	if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, speed , true, 0x01))
	{
		sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
		return false;
	}
	sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);	
    
    
	//ADMA Data transfer 500Kb
	if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_30k, speed /*Speed*/, true /*Width*/, 0x01,0x02))
	{
		SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
		return false;
	}
	SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;
	return true;
}
    virtual ts::Test::Result Main()
    {
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0;
		_function function;
		_r1 status;
		//uint8_t autocmd = 0x01;
		uint32_t card_address = 0x2000, blk_size = 512, blk_count = 10;
		//_init_status init_status;
		uint16_t speed_mode = Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));
		uint16_t type       = Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); //If type == 1 it is normal mode, if == 2 it is UHS mode
		uint32_t force_event = 0x00, shift = 0x00 , speed_selected = 0;
		uint8_t normal_int_event_name[10][40] = {{"CMD timeout Error"},{"CMD CRC Error"},{"CMD end bit Error"},{"Command Index Error"},{"Data time out Error"},{"Data CRC Error"},{"Data End Bit Error"}};
		//uint16_t autoCMDerror[10] = {
		
		bool voltage_switch = false;
		if(type == 2)
			voltage_switch = true;
		
		SD_LOG_DEBUG() << "Selected Mode  : " << speed_mode << std::hex << std::endl;
		SD_LOG_DEBUG() << "Selected Type  : " << type << std::hex << std::endl;		
		
		for(device = 0; device < Maxdevices; device++)
		{
			Maxslots = get_total_slot(device);
			for(slot = 0; slot < Maxslots; slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				//Detect Card
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed for SD device"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				if(!SDCardTest::Initalization(device, slot,voltage_switch))
				{
					SD_LOG_DEBUG() << "Initalization Failed for SD device"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed"<< std::endl;
				
				//Get Card Supported Mode
				if(!get_supported_functions(device , slot, false, false, &function, &status))
				{
					SD_LOG_DEBUG() << "get_supported_functions failed"<< std::endl;
					return ts::Test::Fail;
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
					if(speed_mode == 1)
						speed_selected = 1;
					if(!set_normal_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
					{
						SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "Set Normal speed mode : Completed"<< std::endl;
				}
				else if(2 == type)
				{
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
						return ts::Test::Fail;	
					}
				}
				else
				{
					SD_LOG_DEBUG() << "Wrong Type selected by the user from build.xml , type should be 1 for Normal modes and 2 for UHS modes"<< std::endl;
					return ts::Test::Fail;
				}

				if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
				{
					SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
					return ts::Test::Fail;
				}
	
	uint32_t error = 0x00;		
	/*Error injection : LESS_BLOCK_SIZE*/
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),LESS_BLOCK_SIZE, false,&error);
	env::SleepMs(500);
	sd_logmsg("LESS_BLOCK_SIZE\n");
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}


	/*Error injection : DATA_WIDTH_MISMATCH*/
	sd_logmsg("DATA_WIDTH_MISMATCH\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),DATA_WIDTH_MISMATCH, false,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}

	/*Error injection : AUTO_CMD23_WITHOUT_BLOCK_COUNT*/
	sd_logmsg("AUTO_CMD23_WITHOUT_BLOCK_COUNT\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),AUTO_CMD23_WITHOUT_BLOCK_COUNT, false,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}

	/*Error injection : WRONG_CMD_INDEX*/
	sd_logmsg("WRONG_CMD_INDEX\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),WRONG_CMD_INDEX, false,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}

		/*Error injection : CARD_ADDRESS_ERROR*/
	sd_logmsg("CARD_ADDRESS_ERROR\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),CARD_ADDRESS_ERROR, false,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}
		
		/*Error injection : CLOCK_ERROR*/
	sd_logmsg("CLOCK_ERROR\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),CLOCK_ERROR, false,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}
	
sd_logmsg("******************* Read Operation ************************\n");
	/*Error injection : LESS_BLOCK_SIZE*/
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),LESS_BLOCK_SIZE, true,&error);
	env::SleepMs(500);
	sd_logmsg("LESS_BLOCK_SIZE\n");
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}


	/*Error injection : DATA_WIDTH_MISMATCH*/
	sd_logmsg("DATA_WIDTH_MISMATCH\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),DATA_WIDTH_MISMATCH, true,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}

	/*Error injection : AUTO_CMD23_WITHOUT_BLOCK_COUNT*/
	sd_logmsg("AUTO_CMD23_WITHOUT_BLOCK_COUNT\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),AUTO_CMD23_WITHOUT_BLOCK_COUNT, true,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}

	/*Error injection : WRONG_CMD_INDEX*/
	sd_logmsg("WRONG_CMD_INDEX\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),WRONG_CMD_INDEX, true,&error);
	env::SleepMs(500);
	
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}

		/*Error injection : CARD_ADDRESS_ERROR*/
	sd_logmsg("CARD_ADDRESS_ERROR\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),CARD_ADDRESS_ERROR, true,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}
		
		/*Error injection : CLOCK_ERROR*/
	sd_logmsg("CLOCK_ERROR\n");
	error = 0x00;
	datatransfer_error_inject(device, slot,(speed_selected == 1 ? false : true),CLOCK_ERROR, true,&error);
	env::SleepMs(500);
	if(error == 0)
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);
	if(!error_interrupt_recovery( device, slot, error))
	{
		sd_logmsg("Normal error recovery Failed\n");
		return ts::Test::Fail;
	}	
	error = (get_int_status( device , slot) >> 16);
	sd_logmsg("Error Int : %X\n",error);	
	deselect_card(device, slot , &status ,&error);
	
	if(!do_data_transfer(device , slot , (speed_selected == 1 ? false : true), card_address, blk_size, blk_count))
	{
		SD_LOG_DEBUG() << "Data Transfer Failed"<< std::endl;
		return ts::Test::Fail;
	}



	
	#if 0			
				//Normal Error recovery
				while(force_event <= 0x40)
				{
					force_event = (1 << shift);
					shift++;
					force_normal_int( device,slot,force_event);
					sd_logmsg("forcing the Bus error : %S\n",normal_int_event_name[shift]);
					
					uint32_t error = (get_int_status( device , slot) >> 16);
					
					sd_logmsg("Error Int : %X\n",error);
					//bool recover_normal_error()
					if(!error_interrupt_recovery( device, slot, error))
					{
						sd_logmsg("Normal error recovery Failed\n");
						return ts::Test::Fail;
					}
					if(!do_data_transfer(device , slot , (speed_mode == 1 ? false : true), card_address, blk_size, blk_count))
					{
						sd_logmsg("Data Transfer Failed\n");
						return ts::Test::Fail;
					}
				}	
				SD_LOG_DEBUG() << "Nrmal Error Recovery passed"<< std::endl;
				
			
			//Auto command Error recovery
				while(force_event <= 0x80)
				{
					force_event = (1 << shift);
					shift++;
					if((force_event != 0x20) && (force_event != 0x40))
					{
						force_auto_command(force_event);
						sd_logmsg("forcing the Auto command error : %S\n",normal_int_event_name[shift][]);
					
						//bool recover_normal_error()
						if(!recover_autocmd_error())
						{
							sd_logmsg("Auto Command error recovery Failed\n");
							return ts::Test::Fail;
						}
						if(!do_data_transfer(device , slot , (speed_mode == 1 ? false : true), card_address, blk_size, blk_count))
						{
							sd_logmsg("Data Transfer Failed\n");
							return ts::Test::Fail;
						}
					}
				}
#endif				
			}
		}
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDErrorRecoveryTest, SDErrorRecoveryTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
