
#include "sdcard_test.hpp"
#include "host_types.h"


class SDSpeedModeTest : public SDCardTest
{
  private:

  protected:

    coreutil::Logger    m_lg;
    Target*             m_target_device ;
    Resource_manager*   m_resource_mgr;
  public:
    //SDSpeedModeTest(const TServerTestFactory &Factory) : SDCardTest(Factory),m_lg("SDSpeedMode")
    SDSpeedModeTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDSpeedModeTest()
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
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0;
		_function function;
		_r1 status;
		uint8_t autocmd = 0x01;
		uint32_t card_address = 0x2000, blk_size = 512, blk_count = 100;
		//_init_status init_status;
 		SD_LOG_DEBUG() << "DIFFERNT SPEED MODE TEST BEFORE-------" << std::endl;
		uint16_t speed_mode = Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));
		uint16_t type 		= Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); //If type == 1 it is normal mode, if == 2 it is UHS mode
		bool voltage_switch = false;
 		SD_LOG_DEBUG() << "DIFFERNT SPEED MODE TEST-------" << std::endl;
		if(type == 2)
			voltage_switch = true;
		
		//SD_LOG_DEBUG() << "Selected Mode  : " << speed_mode << std::hex << std::endl;
		//SD_LOG_DEBUG()<< "Selected Type  : " << type << std::hex << std::endl;		
		
		for(device = 0; device < Maxdevices; device++)
		{
			Maxslots = get_total_slot(device);
			for(slot = 0; slot < Maxslots; slot++)
			{
				//SD_LOG_DEBUG()<< "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				SD_LOG_DEBUG()<< "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				//Detect Card
				if(!detect_card(device, slot))
				{
					//SD_LOG_DEBUG()<< "SDCard Detection Failed for SD device"<< std::endl;
					SD_LOG_DEBUG()<< "SDCard Detection Failed for SD device"<< std::endl;
					return ts::Test::Fail;
				}
				//SD_LOG_DEBUG()<< "Card detected"<< std::endl;
				SD_LOG_DEBUG()<< "Card detected"<< std::endl;
				
				//Initialization
				if(!SDCardTest::Initalization(device, slot,voltage_switch))
				{
					//SD_LOG_DEBUG()<< "Initalization Failed for SD device"<< std::endl;
					SD_LOG_DEBUG()<< "Initalization Failed for SD device"<< std::endl;
					return ts::Test::Fail;
				}
				//SD_LOG_DEBUG()<< "Init Completed"<< std::endl;
				SD_LOG_DEBUG()<< "Init Completed"<< std::endl;
				
				//Get Card Supported Mode
				if(!get_supported_functions(device , slot, false, false, &function, &status))
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
					
					if(!set_normal_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
					{
						//SD_LOG_DEBUG()<< "set speed mode failed"<< std::endl;
						SD_LOG_DEBUG()<< "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}
					//SD_LOG_DEBUG()<< "Set Normal speed mode : Completed"<< std::endl;
					SD_LOG_DEBUG()<< "Set Normal speed mode : Completed"<< std::endl;
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
						SD_LOG_DEBUG() << "Set UHS speed mode Completed"<< std::endl;
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
				//Do PIO Data transfer
#if 0
				if(!SDCardTest::pio_data_transfer(device, slot, (speed_mode == 1 ? false : true), true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					//SD_LOG_DEBUG()<< "PIO Data transfer Failed"<< std::endl;
					SD_LOG_DEBUG() << "PIO Data transfer Failed"<< std::endl;
					return ts::Test::Fail;
				}	
#endif
				//SD_LOG_DEBUG()<< "PIO Data transfer Completed" << std::endl;
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				

#if 0
				//Read/Write Data and Compare the data
				if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, (speed_mode == 1 ? false : true), true, autocmd))
				{
					sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
					//return ts::Test::Fail;
				}
				sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);	
#endif

				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					//SD_LOG_DEBUG()<< "ADMA read/write failed"<<std::endl; 
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return ts::Test::Fail;
				}
				//SD_LOG_DEBUG()<< "ADMA Read/Write data_size_500k Completed" << std::endl;
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;
				
				
			}
		}
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDSpeedModeTest, SDSpeedModeTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);

