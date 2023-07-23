
#include "sdcard_test.hpp"
#include "host_types.h"


class SDTuningProcedureTest : public SDCardTest
{
  private:
	uint32_t cap1;
	bool restore;
  public:
    SDTuningProcedureTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDTuningProcedureTest()
    {
    }

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun()
	{
		restore = false;
		SDCardTest::PreRun();
	}

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun()
	{
		    SDCardTest::PostRun();
	}
	void enable_sdr50_tuning(uint32_t device)
	{
		uint32_t capability = read_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD);
		write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD,(capability | (1 << 13)));		
	}
	void save_capability(uint32_t device)
	{
		restore = true;
		cap1 = read_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD);
	}
	void restore_capability(uint32_t device)
	{
		if(restore == true)
		write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD,cap1);
	}

	
    virtual ts::Test::Result Main()
    {
		uint32_t MaxDevices = get_total_pcidevice(), MaxSlots = 0;
		uint32_t device = 0, slot = 0 , error = 0x00;
		//_init_status init_status;
		uint16_t type 		= Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); 
		uint16_t mode = SDR_50;
		_function function;
		_r1 status;
		uint8_t autocmd = 0x01;
		uint32_t card_address = 0x2000, blk_size = 512, blk_count = 500;

		
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
					save_capability(device);
					
					enable_sdr50_tuning(device);
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
					restore_capability(device);
					return ts::Test::Fail;
				}
					
				//Check Requested mode is supported by the card
				if(!(mode & function.group1))
				{
					SD_LOG_DEBUG() << "Card does not support the requested mode" <<std::endl;
					restore_capability(device);
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
				
				if(!tuning_procedure( device, slot))
				{
					SD_LOG_DEBUG() << "tuning procedure failed " <<std::endl;
					restore_capability(device);
					return ts::Test::Fail;
				}
				
				//Do PIO Data transfer
				if(!SDCardTest::pio_data_transfer(device, slot, true , true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed after Tuning procedure"<< std::endl;
					restore_capability(device);
					return ts::Test::Fail;
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed"<< std::endl;
				//ADMA Data transfer 
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_4mb, true/*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					restore_capability(device);
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_4mb Completed" << std::endl;

				//Read/Write Data and Compare the data
				if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, true, true, autocmd))
				{
					sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
					restore_capability(device);
					return ts::Test::Fail;
				}
				sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
				restore_capability(device);				
			}
		}
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDTuningProcedureTest, SDTuningProcedureTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
