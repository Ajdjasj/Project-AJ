
#include "sdcard_test.hpp"
#include "host_types.h"


class SDTuningModeTest : public SDCardTest
{
  private:
	uint32_t cap1;
  public:
    SDTuningModeTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDTuningModeTest()
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

	void save_capability(uint32_t device)
	{
		cap1 = read_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD);
	}
	void restore_capability(uint32_t device)
	{
		write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD,cap1);
	}

	void enable_tuning_mode(uint32_t device , uint32_t tuning_mode)
	{
		uint32_t capability = read_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD);
		write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD,(capability | ((tuning_mode - 1) << 14)));
	}
	
	void enable_sdr50_tuning(uint32_t device)
	{
		uint32_t capability = read_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD);
		write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD,(capability | (1 << 13)));		
	}
	
    virtual ts::Test::Result Main()
    {
		uint32_t MaxDevices = get_total_pcidevice(), MaxSlots = 0;
		uint32_t device = 0, slot = 0 , error = 0x00;
		//_init_status init_status;
		uint16_t mode = SDR_50;
		_function function;
		_r1 status;
		uint8_t autocmd = 0x01;
		uint32_t card_address = 0x5000, blk_size = 512, blk_count = 500;
		uint32_t total_data_size  = Parameter<uint16_t>("datasize"  ,0 , coreutil::FmtFlags(std::ios::dec));  // multiple of 1mb
		uint32_t total_trans_loop = Parameter<uint16_t>("transloop" ,0 , coreutil::FmtFlags(std::ios::dec));  // multiple of 1mb
		uint32_t tuning_mode	  = Parameter<uint16_t>("tuningmode",0 , coreutil::FmtFlags(std::ios::dec));
		uint32_t type			  = Parameter<uint16_t>("speedmode"	,0 , coreutil::FmtFlags(std::ios::dec));
		
		if(total_data_size < 1)
		{
			SD_LOG_DEBUG() << "Wrong Data size selected by the user from build.xml , size should be >= 1(multiple of mb)"<< std::endl;
			return ts::Test::Fail;
		}
		if(total_trans_loop < 10)
		{
			SD_LOG_DEBUG() << "Wrong Data transfer loop selected by the user from build.xml , loop count should be minimum 10"<< std::endl;
			return ts::Test::Fail;
		}
		if(type == 1)
			mode = SDR_50;
		else if(type == 2)
			mode = SDR_104;
		else
		{
			SD_LOG_DEBUG() << "Wrong Type selected by the user from build.xml , type should be 1 SDR50 tuning and 2 SDR104 tuning"<< std::endl;
			return ts::Test::Fail;
		}
	
		for(device = 0; device < MaxDevices; device++)
		{
			MaxSlots = get_total_slot(device);
			for(slot = 0; slot < MaxSlots; slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				//Detect Card
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				if(!SDCardTest::Initalization(device, slot, true))
				{
					SD_LOG_DEBUG() << "Initalization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed" << std::endl;
	
				if(init_status.host.version < HOST_SPEC_VERSION_3_00)
				{
					SD_LOG_DEBUG() << "Host does not support Tuning, Host Spec Verion is less than version3.0" <<std::endl;
					return ts::Test::Fail;	
				}	
				
				//Change the speed mode
				if(mode == SDR_50)//SDR 50 Tuning
				{
					if(init_status.host.sdr50 == false)
					{
						SD_LOG_DEBUG() << "Host does not support SDR50 mode" <<std::endl;
						return ts::Test::Fail;					
					}
				}
				else
				{
					if(init_status.host.sdr104 == false)
					{
						SD_LOG_DEBUG() << "Host does not support SDR104 mode" <<std::endl;
						return ts::Test::Fail;					
					}
				
				}
				
				//Get Card Supported Mode
				if(!get_supported_functions(device , slot, false, false, &function, &status))
				{
					SD_LOG_DEBUG() << "get_supported_functions failed"<< std::endl;
					return ts::Test::Fail;
				}
					
				//Check Requested mode is supported by the card
				if(!(mode & function.group1))
				{
					SD_LOG_DEBUG() << "Card does not support the requested mode" <<std::endl;
					return ts::Test::Fail;			
				}
					
						_reg sd_register;
						uint32_t clk_multiplier = 0x00;
						
						SET_SD_REGISTER_READ(&sd_register,HOST_CAPABILITIES_1,device,slot);
						read_host_reg(&sd_register);
						clk_multiplier = ((sd_register.sd_reg.value & 0x00FF0000) >> 16);
						if(0x00 == clk_multiplier)
						{
							if(!set_UHS_mode( device,  slot,  mode, DIVIDED_CLK_SELECTION_MODE))
							{
								SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
								return ts::Test::Fail;
							}						
						}
						else
						{
							if(!set_UHS_mode( device,  slot,  mode, PROGRAMABLE_CLK_SELECTION_MODE))
							{
								SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
								return ts::Test::Fail;
							}
						}	
						SD_LOG_DEBUG() << "Set UHS speed mode Completed"<< std::endl;

				uint32_t data_trns_count = 0x01;
				uint32_t int_status      = 0x00;
				reg32 intrrupt_status;
				uint32_t re_tuning_int_count = 0x00;
				save_capability(device);
				enable_tuning_mode(device , tuning_mode);
				if(mode == SDR_50)//SDR 50 Tuning
					enable_sdr50_tuning(device);
				
				if(!tuning_procedure( device, slot))
				{
					SD_LOG_DEBUG() << "tuning procedure failed " <<std::endl;
					restore_capability(device);
					return ts::Test::Fail;
				}

				while(data_trns_count <= total_trans_loop)
				{
					if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, (data_size)(total_data_size * data_size_1mb),true/*Speed*/, true /*Width*/, autocmd,0x02))
					{
						SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
						restore_capability(device);
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "ADMA Read/Write  "<< (total_data_size) << "mb Completed, Data transfer loop count = " << data_trns_count << std::endl;
					data_trns_count++;
					intrrupt_status.reg_val = get_int_status(device , slot);
					
					if(tuning_mode == 2)// Mode 2
					{
						if(intrrupt_status.bit.b15 != 1)
						{
							if(intrrupt_status.bit.b12 == 1)
							{
								re_tuning_int_count++;
								if(!tuning_procedure( device, slot))
								{
									SD_LOG_DEBUG() << "tuning procedure failed " <<std::endl;
									restore_capability(device);
									return ts::Test::Fail;
								}
							}
						}
						else					
						{
							restore_capability(device);
							SD_LOG_DEBUG() << "Errot Interrupt Generated , Errot Int Status "<< std::hex << ((intrrupt_status.reg_val & 0xffff0000) >> 16) << std::endl; 
							return ts::Test::Fail;
						}
					}
					else if(tuning_mode == 1)// Mode 1
					{
						if(intrrupt_status.bit.b15 != 1)
						{
							if(!tuning_procedure( device, slot))
							{
								SD_LOG_DEBUG() << "tuning procedure failed " <<std::endl;
								restore_capability(device);
								return ts::Test::Fail;
							}
						}
						else					
						{
							SD_LOG_DEBUG() << "Errot Interrupt Generated , Errot Int Status "<< std::hex << ((intrrupt_status.reg_val & 0xffff0000) >> 16) << std::endl; 
							restore_capability(device);
							return ts::Test::Fail;
						}
					}					
				}
				if(tuning_mode == 2)// Mode 2
					SD_LOG_DEBUG() << "Total Re-Tuning Interrupt from the Host : "<< std::dec << re_tuning_int_count <<std::endl;
			}
			restore_capability(device);
		}

		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDTuningModeTest, SDTuningModeTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
