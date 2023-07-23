
#include "sdcard_test.hpp"
#include "host_types.h"
#include "sd_wrapper.h"

#define INT_MESSAGE_ADDRESS 0xFEE0F00C
#define IOAPIC_IOREGSEL		0xFEC00000
#define IOAPIC_IOWINREG		0xFEC00010
#define MSI_32 false
#define MSI_64 true
#define MSI_ENABLE  true
#define MSI_DISABLE false
#define INT_ENABLE	true
#define INT_DISABLE	false

void intx_enable(uint32_t device, bool status)
{
uint32_t command_reg = read_pcicfg(device, SDHC_PCI_OR_MMIO_COMMAND , DWORD);
reg32 temp_value;
temp_value.reg_val = command_reg;
temp_value.bit.b10 = (status == true ? 0 : 1);
write_pcicfg(device, SDHC_PCI_OR_MMIO_COMMAND , DWORD , temp_value.reg_val);
}

void msi_enable(uint32_t device,bool msi_status)
{
uint32_t sd_control_reg = read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
reg32 temp_value;
temp_value.reg_val = sd_control_reg;
temp_value.bit.b1 = msi_status;
write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD , temp_value.reg_val);
}
void setup_msi(uint32_t device , bool msi_type , uint32_t vector_index)
{
	env_device_interface env_dev;
	env::System* device1 = env_dev.get_env_system();
	env::Map* memptr1;
	memptr1 = device1->pmap((void*)IOAPIC_IOREGSEL,32);
	env::System* device2 = env_dev.get_env_system();
	//env::Map* memptr2;
	//memptr2 = device2->pmap((void*)IOAPIC_IOWINREG,32);
	uint32_t *ioregsel = (uint32_t *)memptr1->getVirtualAddress();
	//uint32_t *iowinreg = (uint32_t*)(memptr2->getVirtualAddress());
	//uint32_t *iowinreg = (uint32_t*)memptr1->getVirtualAddress() + 0x10;
	uint32_t *iowinreg = (uint32_t*)(ioregsel+ 0x10);
	uint32_t msi_cap_header;
	uint32_t sd_control_reg = read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
	reg32 temp_value;
	
	
	if(false == msi_type) //32bit MSI
	{
		temp_value.reg_val = sd_control_reg;
		temp_value.bit.b4 = msi_type;
		write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD , temp_value.reg_val);
		
		
		write_pcicfg(device, SDHC_PCI_MSIADDR , DWORD , INT_MESSAGE_ADDRESS);
		//Extract the current vector for SD from the IOAPIC
		*ioregsel = vector_index;
		write_pcicfg(device, SDHC_PCI_MSIUADDR , DWORD , ((*iowinreg & 0xFF) | 0xC900));
	
	}
	else//64 bit MSI
	{
		temp_value.reg_val = sd_control_reg;
		temp_value.bit.b4 = msi_type;
		write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD , temp_value.reg_val);

		write_pcicfg(device, SDHC_PCI_MSIADDR  , DWORD , INT_MESSAGE_ADDRESS);
		write_pcicfg(device, SDHC_PCI_MSIUADDR , DWORD , 0);		
		//Extract the current vector for SD from the IOAPIC
		*ioregsel = vector_index;
		write_pcicfg(device, SDHC_PCI_MSIDATA , DWORD , ((*iowinreg & 0xFF) | 0xC900));
	}
	
	msi_cap_header = read_pcicfg(device, SDHC_PCI_MSICAPHEADER , DWORD);
	temp_value.reg_val = msi_cap_header;
	temp_value.bit.b16 = 1;
	write_pcicfg(device, SDHC_PCI_MSICAPHEADER , DWORD , temp_value.reg_val);	
}




class INTx_MSI_interrutp : public SDCardTest
{
  private:

  public:
	uint32_t command_reg,sd_control_reg,msi_cap;
    INTx_MSI_interrutp(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~INTx_MSI_interrutp()
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
	
	void save_register(uint32_t device)
	{
		command_reg 	= read_pcicfg(device, SDHC_PCI_OR_MMIO_COMMAND   , DWORD);
		sd_control_reg 	= read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
		msi_cap			= read_pcicfg(device, SDHC_PCI_MSICAPHEADER , DWORD);
	}
	
	ts::Test::Result test_failed(uint32_t device)
	{
		//Restore the register on test failure
		write_pcicfg(device, SDHC_PCI_OR_MMIO_COMMAND    	, DWORD ,command_reg);
		write_pcicfg(device, SDHC_PCI_SDCONTROL    	, DWORD ,sd_control_reg);
		write_pcicfg(device, SDHC_PCI_MSICAPHEADER 	, DWORD ,msi_cap);
		
		return ts::Test::Fail;
	}


    virtual ts::Test::Result Main()
    {
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0 , error = 0x00;
		//_init_status init_status;
		//_function function;
		uint16_t speed_mode = HIGH_SPEED;
		uint32_t SD_control_register_save = 0x00;
		//_r1 status;
		uint32_t card_address = 0x5000, blk_size = 512, blk_count = 100;
		uint8_t autocmd = 0x01;
		uint32_t test_type = Parameter<uint32_t>("type",0, coreutil::FmtFlags(std::ios::hex));
		uint8_t *source = (uint8_t*)"/media/log.xml";
		uint8_t destination[5] = "/tmp";
		uint8_t *dest = &destination[0];
		//char command[20];
		//sprintf(command,"xcopy %s %s /y",source,dest);
		std::ostringstream cmd_str;
                #ifdef WIN32
		uint8_t drive_name = Parameter<uint8_t>("drive",0, coreutil::FmtFlags(std::ios::hex));
		destination[0] = drive_name;
                cmd_str<<"xcopy "<<(char*)source<<" "<<(char*)dest<<endl;
                #else
                cmd_str<<"cp -R "<<(char*)source<<" "<<(char*)dest<<endl;
                #endif


		uint32_t vector_index = Parameter<uint32_t>("vector",0, coreutil::FmtFlags(std::ios::hex));

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
#if 0				
				//Initialization
				if(!SDCardTest::Initalization(device, slot, true))
				{
					SD_LOG_DEBUG() << "Initalization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed" << std::endl;
#endif				
				save_register(device);
				
				/********************* INTx Interrutp Enable and Data Transfer ****************************/
			if(1 == test_type)
			{
#if 0
				intx_enable(device,INT_ENABLE);
				msi_enable(device,MSI_DISABLE);	

				//Do Data TRansfer PIO/ADMA/SDMA
				if(!SDCardTest::pio_data_transfer(device, slot, true, true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed"<< std::endl;
					return test_failed(device);
				}	
				SD_LOG_DEBUG() << "50Kb PIO Data transfer Completed" << std::endl;
				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k,true, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return test_failed(device);
				}
				SD_LOG_DEBUG() << "500Kb ADMA Read/Write data_size_500k Completed" << std::endl;				
#endif				

				env::SleepMs(500);
				//SDCardTest::enable_device(device, true);
				env::SleepMs(500);
				save_register(device);
				intx_enable(device,INT_ENABLE);
				msi_enable(device,MSI_DISABLE);				
				//if(system(command))
				SD_LOG_DEBUG()<<"command to execute="<<cmd_str.str().c_str()<<endl;
				if(system(cmd_str.str().c_str()))	
				//if(system("cp -R /media/log.xml /tmp"))	
				{
					SD_LOG_DEBUG() << "Data Transfer failed in INTx"<< std::endl;
					return test_failed(device);
				}
				SD_LOG_DEBUG() << "INTx interrupt test passed"<< std::endl;
				//SDCardTest::enable_device(device, false);
				
				env::SleepMs(100);
			}
				/********************* MSI 32 Interrutp Enable and Data Transfer ****************************/
			if(2 == test_type)
			{
#if 0
				intx_enable(device,INT_DISABLE);
				msi_enable(device,MSI_ENABLE);	//enable MSI 32			
				setup_msi(device ,MSI_32 , vector_index);
				//Do Data TRansfer PIO/ADMA/SDMA

				card_address = 0x6000;
				if(!SDCardTest::pio_data_transfer(device, slot, true, true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed in High Speed Mode"<< std::endl;
					return test_failed(device);
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k,true, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return test_failed(device);
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;				
#endif				
				env::SleepMs(500);
				//SDCardTest::enable_device(device, true);
				env::SleepMs(500);
				intx_enable(device,INT_DISABLE);
				msi_enable(device,MSI_ENABLE);	//enable MSI 32			
				setup_msi(device ,MSI_32,vector_index);				
			


				if(system(cmd_str.str().c_str()))	
				//if(system(command))
				{
					SD_LOG_DEBUG() << "Data transfer failed in MSI32"<< std::endl;
					return test_failed(device);
				}
				SD_LOG_DEBUG() << "MSI32 interrupt test passed"<< std::endl;
				//SDCardTest::enable_device(device, false);
				env::SleepMs(100);
				//Initialization
			}
				/********************* MSI 64 Interrutp Enable and Data Transfer ****************************/
			if(3 == test_type)
			{
				intx_enable(device,INT_DISABLE);
				msi_enable(device,MSI_ENABLE);	//enable MSI 64			
				setup_msi(device ,MSI_64 , vector_index);
				//Do Data TRansfer PIO/ADMA/SDMA			
#if 0
				card_address = 0x7000;
				if(!SDCardTest::pio_data_transfer(device, slot, true, true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed in High Speed Mode"<< std::endl;
					return test_failed(device);
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k,true, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return test_failed(device);
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;				
#endif
				env::SleepMs(500);
				//SDCardTest::enable_device(device, true);
				env::SleepMs(500);
				intx_enable(device,INT_DISABLE);
				msi_enable(device,MSI_ENABLE);	//enable MSI 64			
				setup_msi(device ,MSI_64,vector_index);				
				//if(system(command))
				if(system(cmd_str.str().c_str()))	
				{
					SD_LOG_DEBUG() << "Data transfer failed in MSI64"<< std::endl;
					return test_failed(device);
				}
				SD_LOG_DEBUG() << "MSI64 interrupt test passed"<< std::endl;
				//SDCardTest::enable_device(device, false);
				env::SleepMs(100);
			}

			}
			test_failed(device);
		}
		return ts::Test::Pass;	
	}
};

TServerTestInstance(INTx_MSI_interrutp, INTx_MSI_interrutp);
