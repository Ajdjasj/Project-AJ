
#include "sdcard_test.hpp"
#include "host_types.h"


class SDPresetvalueTest : public SDCardTest
{
  private:

  public:
    SDPresetvalueTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDPresetvalueTest()
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
		uint32_t MaxDevices = get_total_pcidevice(), MaxSlots = 0;
		uint32_t Device = 0, Slot = 0 , error = 0x00;
		//_init_status init_status;
		uint16_t type 							= Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); 
		uint16_t mode 							= Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));		
		uint32_t preset_register 				= 0x60;
		bool voltage_switch 					= false;
		_reg clk_ctrl;					
		_reg preset;			  					
		_reg host_ctrl2 ;						
		_r1 status;
		_function function;
		uint16_t SDCLK_Frq_select 				= 0x00;		
		uint16_t UPPER_BITS_SDCLK_Frq_select	= 0x00;
		uint16_t SDCLK_Gen_select 				= 0x00;
		uint16_t DRIVER_Strength				= 0x00;
		
		uint16_t preset_Frq_select 				= 0x00;
        uint16_t preset_BITS_SDCLK_Frq_select	= 0x00;
		uint16_t preset_Gen_select 				= 0x00;
		uint16_t preset_Strength				= 0x00;
		uint32_t card_address = 0x3000, blk_size = 512, blk_count = 100;
		uint8_t autocmd = 0x01;
		uint32_t speed_mode = 0;
		
		if(2 == type)
			voltage_switch = true;
		
		
		
		for(Device = 0; Device < MaxDevices; Device++)
		{
			MaxSlots = get_total_slot(Device);
			for(Slot = 0; Slot < MaxSlots; Slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD Device : " << Device << " , SD Slot : "<< Slot <<" ---------------------" << std::endl;
				//Detect Card
				if(!detect_card(Device, Slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				if(!SDCardTest::Initalization(Device, Slot,voltage_switch))
				{
					SD_LOG_DEBUG() << "Initalization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed" << std::endl;
	
				if(2 == type)
				{
					if(init_status.host.version < HOST_SPEC_VERSION_3_00)
					{
						SD_LOG_DEBUG() << "Host does not support this mode, Host Spec Verion is less than version3.0" <<std::endl;
						return ts::Test::Fail;	
					}	
					SET_SD_REGISTER_READ(&host_ctrl2,HOST_HOST_CONTROL_2,Device,Slot);
					if(!read_host_reg(&host_ctrl2))
						return ts::Test::Fail;

					DRIVER_Strength = ((host_ctrl2.sd_reg.value & 0x0030) >> 4);
					//Change the speed mode
					if(mode == SDR_12)
						preset_register = HOST_PRESET_SDR12;	
					else if(mode == SDR_25)	
					{	
						speed_mode = 1;
						preset_register = HOST_PRESET_SDR25;
					}
					else if(mode == SDR_50)//SDR 50 Tuning
					{
						preset_register = HOST_PRESET_SDR50;
						if(init_status.host.sdr50 == false)
						{
							SD_LOG_DEBUG() << "Host does not support SDR50 mode" <<std::endl;
							return ts::Test::Fail;					
						}
					}
					else if(mode == SDR_104)
					{
						preset_register = HOST_PRESET_SDR104;
						if(init_status.host.sdr104 == false)
						{
							SD_LOG_DEBUG() << "Host does not support SDR104 mode" <<std::endl;
							return ts::Test::Fail;					
						}
					}
					else
					{
						SD_LOG_DEBUG() << "Wrong 'Mode' selected by the user from build.xml"<< std::endl;
						return ts::Test::Fail;					
					}
				}
				else if(1 == type)
				{
					preset_register = HOST_PRESET_DEFAILT;
					if(mode == HIGH_SPEED)
					{
						preset_register = HOST_PRESET_HIGHSPEED;
						if(init_status.host.hs == false)
						{
							SD_LOG_DEBUG() << "Host does not support SDR104 mode" <<std::endl;
							return ts::Test::Fail;					
						}
					
					}				

				}	
				else
				{
					SD_LOG_DEBUG() << "Wrong 'Type' selected by the user from build.xml , type should be 1 normal mode and 2 for UHS mode"<< std::endl;
					return ts::Test::Fail;				
				}
				//Get Card Supported Mode
				if(!get_supported_functions(Device , Slot, false, false, &function, &status))
				{
					SD_LOG_DEBUG() << "get_supported_functions failed "<< std::endl;
					return ts::Test::Fail;
				}
				
				//Check Requested mode is supported by the card
				if(!(mode & function.group1))
				{
					SD_LOG_DEBUG() << "Card does not support the requested mode" <<std::endl;
					return ts::Test::Fail;			
				}

				if(1 == type)
				{				
					if(!set_normal_mode( Device, Slot ,  mode , PRESET_CLK_SELECTION_MODE))
					{
						SD_LOG_DEBUG() << "set Normal mode failed "  <<std::endl;
						return ts::Test::Fail;
					}
				}
				
				if(2 == type)
				{				
					if(!set_UHS_mode( Device, Slot ,  mode , PRESET_CLK_SELECTION_MODE))
					{
						SD_LOG_DEBUG() << "set UHS mode failed "  <<std::endl;
						return ts::Test::Fail;
					}
					
				}
				SD_LOG_DEBUG() << "Mode Switch Success"  <<std::endl;
				
				env::SleepMs(10);
				SET_SD_REGISTER_READ(&preset,preset_register,Device,Slot);
				if(!read_host_reg(&preset))
					return ts::Test::Fail;	
				
				SET_SD_REGISTER_READ(&clk_ctrl,HOST_CLOCK_CONTROL,Device,Slot);
				if(!read_host_reg(&clk_ctrl))
					return ts::Test::Fail;
					
				SDCLK_Frq_select 				= (uint16_t)((clk_ctrl.sd_reg.value & 0xFF00) >> 8);	
				UPPER_BITS_SDCLK_Frq_select		= (uint16_t)((clk_ctrl.sd_reg.value & 0x00C0) >> 6);	
				SDCLK_Gen_select 				= (uint16_t)((clk_ctrl.sd_reg.value & 0x0020) >> 5);	
				
				preset_Frq_select 				= (uint16_t)(preset.sd_reg.value   & 0x00FF);
				preset_BITS_SDCLK_Frq_select	= (uint16_t)((preset.sd_reg.value  & 0x0300) >> 8);
				preset_Gen_select 				= (uint16_t)((preset.sd_reg.value  & 0x0400) >> 10);
				preset_Strength					= (uint16_t)((preset.sd_reg.value  & 0xC000) >> 14);
					
				if(SDCLK_Frq_select != preset_Frq_select)	
				{
					SD_LOG_DEBUG() << "Preset CLK Loading Failed " <<std::endl;
					SD_LOG_DEBUG() << "Clock Ctrl Register Frequency Select   : " << std::hex << SDCLK_Frq_select  <<std::endl;
					SD_LOG_DEBUG() << "Preset Value Register Frequency Select : " << std::hex << preset_Frq_select <<std::endl;
					return ts::Test::Fail;					
				}	

				if(init_status.host.version < HOST_SPEC_VERSION_3_00)
				{
					if(SDCLK_Gen_select != 0)	
					{
						SD_LOG_DEBUG() << "Preset CLK Loading Failed " <<std::endl;
						SD_LOG_DEBUG() << "Clock Ctrl Register Clock Generator Select : " << std::hex << SDCLK_Gen_select << " , But the value should be 0"<<std::endl;
						return ts::Test::Fail;
					}

					if(UPPER_BITS_SDCLK_Frq_select != 0)	
					{
						SD_LOG_DEBUG() << "Preset CLK Loading Failed " <<std::endl;
						SD_LOG_DEBUG() << "Clock Ctrl Register Clock Upper bits of SDCLK : " << std::hex << UPPER_BITS_SDCLK_Frq_select << " , But the value should be 0"<<std::endl;
						return ts::Test::Fail;
					}
				}
				else
				{
					if(SDCLK_Gen_select != preset_Gen_select)	
					{
						SD_LOG_DEBUG() << "Preset CLK Loading Failed " <<std::endl;
						SD_LOG_DEBUG() << "Clock Ctrl Register Clock Generator Select   : " << std::hex << SDCLK_Gen_select  <<  std::endl;
						SD_LOG_DEBUG() << "Preset Value Register Clock Generator Select : " << std::hex << preset_Gen_select <<  std::endl;
						return ts::Test::Fail;
					}

					if(UPPER_BITS_SDCLK_Frq_select != preset_BITS_SDCLK_Frq_select)	
					{
						SD_LOG_DEBUG() << "Preset CLK Loading Failed " <<std::endl;
						SD_LOG_DEBUG() << "Clock Ctrl Register Upper bits of SDCLK   : " << std::hex << UPPER_BITS_SDCLK_Frq_select  <<std::endl;
						SD_LOG_DEBUG() << "Preset Value Register Upper bits of SDCLK : " << std::hex << preset_BITS_SDCLK_Frq_select <<std::endl;
						return ts::Test::Fail;
					}
					
					if(preset_Strength != DRIVER_Strength)	
					{
						SD_LOG_DEBUG() << "Preset CLK Loading Failed " <<std::endl;
						SD_LOG_DEBUG() << "Clock Ctrl Register Driver Strength    : " << std::hex << DRIVER_Strength << std::endl;
						SD_LOG_DEBUG() << "Preset Value Register Driver Strength  : " << std::hex << preset_Strength << std::endl;
						return ts::Test::Fail;
					}				
				
				}
				
				
								//Do PIO Data transfer
				if(!SDCardTest::pio_data_transfer(Device, Slot, (speed_mode == 1 ? false : true), true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed"<< std::endl;
					return ts::Test::Fail;
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				

				//Read/Write Data and Compare the data
				if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, (speed_mode == 1 ? false : true), true, autocmd))
				{
					sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
					//return ts::Test::Fail;
				}
				sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);	


				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(Device, Slot, card_address, data_size_500k, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;
				
			}
		}
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDPresetvalueTest, SDPresetvalueTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
