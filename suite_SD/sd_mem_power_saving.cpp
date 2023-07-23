
#include "sdcard_test.hpp"
#include "host_types.h"
#include "sd_wrapper.h"

class MemPowerSaving : public SDCardTest
{
  private:

  public:
	uint32_t misc_reg_base;
	uint32_t save_register_misc;
	uint32_t svae_sd_pci_register;
	bool restore_pci;
	bool restore_misc;
    MemPowerSaving(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~MemPowerSaving()
    {
    }

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun()
	{
		    SDCardTest::PreRun();
			//save_restore(true);
			restore_misc = false;
			restore_pci  = false;
	}

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun()
	{
		    SDCardTest::PostRun();
			//save_restore(false);
	}
	
	void enable_sdmempower_saving(bool state)
	{
		env_device_interface env_dev;
		env::System* device1 = env_dev.get_env_system();
		reg32 temp_value;
		env::Map* memptr1;
		
		memptr1 = device1->pmap((void*)(misc_reg_base + 0x68),1);
		uint32_t *misc_reg0x68 = (uint32_t*)memptr1->getAddress();
		
		restore_misc = true;
		save_register_misc = *misc_reg0x68;
		
		temp_value.reg_val = *misc_reg0x68;
		temp_value.bit.b18 = state;
		*misc_reg0x68      = temp_value.reg_val;
	}
	
	void enable_shutdown_deepsleep_mode(uint32_t device)
	{
		reg32 temp_value;
		svae_sd_pci_register = (uint32_t)read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
		temp_value.reg_val   = svae_sd_pci_register;
		restore_pci = true;

		temp_value.bit.b10 = 1;
		temp_value.bit.b11 = 1;
		write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD , temp_value.reg_val);
	}
	
	
	void save_restore(uint32_t device)
	{
		env_device_interface env_dev;
		env::System* device1 = env_dev.get_env_system();
		env::Map* memptr1;
		memptr1 = device1->pmap((void*)(misc_reg_base + 0x68),1);
		uint32_t *misc_reg0x68 = (uint32_t*)memptr1->getAddress();
	
		if(restore_pci) 
		{
			write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD , svae_sd_pci_register);
		}
		if(restore_misc) 
		{
			*misc_reg0x68 = save_register_misc;
		}
	}
	
	
    virtual ts::Test::Result Main()
    {
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0 , error = 0x00;
		//_init_status init_status;
		_function function;
		uint16_t speed_mode = HIGH_SPEED;
		uint32_t SD_control_register_save = 0x00;
		_r1 status;
		uint32_t card_address = 0x5000, blk_size = 512, blk_count = 100;
		uint8_t autocmd = 0x01;
		uint8_t drive_name = Parameter<uint8_t>("drive",0, coreutil::FmtFlags(std::ios::hex));
		uint8_t *source = (uint8_t*)"log.xml";
		uint8_t destination[5] = "/tmp";
		uint8_t *dest = &destination[0];
		destination[0] = drive_name;
		char command[20];
		//sprintf(command,"xcopy %s %s /y",source,dest);
		std::ostringstream cmd_str;
		#ifdef WIN32
		cmd_str<<"xcopy "<<(char*)source<<" "<<(char*)dest<<endl;
		#else
		cmd_str<<"cp -R"<<(char*)source<<" "<<(char*)dest<<endl;
		#endif
		misc_reg_base = ((port_read(0x24)) & 0xFFFFE000) + 0xE00;

		SD_LOG_DEBUG() << "In SD Memory Power Saving Test.... " << std::endl;
		SD_LOG_DEBUG() << "PM_Register Base :" << std::hex << misc_reg_base << std::endl;
		
		for(device = 0; device < Maxdevices; device++)
		{
			Maxslots = get_total_slot(device);
			for(slot = 0; slot < Maxslots; slot++)
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
			
				enable_shutdown_deepsleep_mode(device);
				enable_sdmempower_saving(false);	
				
				/********************* Clock Gating Enable and Data Transfer ****************************/
				
				uint32_t test_count = 0x00;
				for(test_count = 0 ; test_count < 3 ; test_count++)
				{
					env::SleepMs(500);
					SDCardTest::enable_device(device, true);
					env::SleepMs(500);
					if(system(cmd_str.str().c_str()))
					//if(system(command))
					{
						SD_LOG_DEBUG() << "Data Transfer failed When Mem Power saving Enabled"<< std::endl;
						save_restore(device);
						return ts::Test::Fail;
					}
					env::SleepMs(5000);
				}
				
				
				enable_sdmempower_saving(true);	

				for(test_count = 0 ; test_count < 3 ; test_count++)
				{
					env::SleepMs(500);
					SDCardTest::enable_device(device, true);
					env::SleepMs(500);
					if(system(command))
					{
						SD_LOG_DEBUG() << "Data Transfer failed When Mem Power saving Disabled"<< std::endl;
						save_restore(device);
						return ts::Test::Fail;
					}
					env::SleepMs(5000);
				}
				
				SD_LOG_DEBUG() << "Memory Power Saving Test Passed"<< std::endl;
				SDCardTest::enable_device(device, false);
				
				env::SleepMs(100);
			}
			save_restore(device);
		}
		return ts::Test::Pass;	
	}
};

TServerTestInstance(MemPowerSaving, MemPowerSaving);


//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
