
#include "sdcard_test.hpp"
#include "host_types.h"


class SDDeviceClassRWTest: public SDCardTest
{
  private:

  public:
  uint32_t SD_device_class_id;
  uint32_t SD_Control;
    SDDeviceClassRWTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDDeviceClassRWTest()
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
	
	ts::Test::Result test_failed(uint32_t device)
	{
		//Restore the register on test failure
		write_pcicfg(device, SDHC_PCI_REVISION_ID , DWORD ,SD_device_class_id);
		write_pcicfg(device, SDHC_PCI_SDCONTROL   , DWORD ,SD_Control);
		return ts::Test::Fail;
	}
	
    virtual ts::Test::Result Main()
    {
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0 , error = 0x00;
		Result TestResult = ts::Test::Pass;
		uint32_t SD_device_class_id_register = 0x00;
		uint32_t SD_Control_register = 0x00;
		uint32_t current_class_code = 0x00, new_class_code = 0x00, current_sub_class = 0x00,new_sub_class = 0x00;
		
		
		
		//_init_status init_status;
		//_function function;
		uint16_t speed_mode = HIGH_SPEED;
		uint32_t SD_control_register = 0x00;
		//_r1 status;
		uint32_t card_address = 0x2000, blk_size = 512, blk_count = 100;
		uint8_t autocmd = 0x01;
	
		for(device = 0; device < Maxdevices; device++)
		{
			SD_device_class_id_register = read_pcicfg(device	, SDHC_PCI_REVISION_ID  , DWORD);
			SD_device_class_id 			= SD_device_class_id_register;
			
			SD_Control_register			= read_pcicfg(device	, SDHC_PCI_SDCONTROL 	, DWORD);
			SD_Control					= SD_Control_register;		
			
			
			//Enable SD class code Backdoor access
			SD_LOG_DEBUG() << "Enabling the BackDoor Device class access" << std::endl;
			write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD ,(SD_Control_register | (1 << 6)) );
			
			
			//Change class Code
			current_class_code 	= (SD_device_class_id_register & 0xFF000000);
			new_class_code 		= (~(current_class_code) & 0xFF000000);
			//Change Sub class 
			current_sub_class 	= (SD_device_class_id_register & 0x00FF0000);
			new_sub_class		= (~(current_sub_class) & 0x00FF0000);	
	
			write_pcicfg(device, SDHC_PCI_REVISION_ID , DWORD ,((((SD_device_class_id_register & 0x00FFFFFF) | new_class_code ) & 0xFF00FFFF) | new_sub_class));
		

			//sd_logmsg("((SD_device_class_id_register & 0x00FFFFFF) | new_class_code )) = %X",((((SD_device_class_id_register & 0x00FFFFFF) | new_class_code ) & 0xFF00FFFF) | new_sub_class));

			current_sub_class	= ((read_pcicfg(device	, SDHC_PCI_REVISION_ID  , DWORD)) & 0x00FF0000);
			current_class_code	= ((read_pcicfg(device	, SDHC_PCI_REVISION_ID  , DWORD)) & 0xFF000000);
			
			
			if(current_class_code != new_class_code )
			{
				SD_LOG_DEBUG() << "BackDoor Write to Class Code Failed  "<< "Class Code Written = "<< std::hex << (new_class_code >> 24)  << ", Class Code Red = "<< std::hex << (current_class_code >> 24) << std::endl;
				return test_failed(device);			
			}
			
			if(current_sub_class != new_sub_class )
			{
				SD_LOG_DEBUG() << "BackDoor Write to Sub Class Failed  "<< "Sub Class Written = "<< std::hex << (new_sub_class >> 16)  << ", Sub Class Red = "<< std::hex << (current_sub_class >> 16)   << std::endl;
				return test_failed(device);		
			}
			
			
			//Write the original Value Back
			SD_LOG_DEBUG() << "Re storing the Original Device class and Subclass value...." << std::endl;
			write_pcicfg(device, SDHC_PCI_REVISION_ID , DWORD ,SD_device_class_id);
			if(SD_device_class_id != (read_pcicfg(device	, SDHC_PCI_REVISION_ID  , DWORD)))
			{
				SD_LOG_DEBUG() << "Re store Failed" << std::endl;
				return test_failed(device);
			}
			//Disable SD class code Backdoor access
			SD_LOG_DEBUG() << "Disabling the BackDoor Device class access" << std::endl;
			write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD ,(SD_Control_register & (~(1 << 6))) );
			
			SD_device_class_id_register = read_pcicfg(device	, SDHC_PCI_REVISION_ID  , DWORD);
			
			SD_LOG_DEBUG() << "Try to do BackDoor access to Device class" << std::endl;
			write_pcicfg(device, SDHC_PCI_REVISION_ID , DWORD ,(SD_device_class_id_register & 0x0000FFFF));
			if((SD_device_class_id_register != read_pcicfg(device	, SDHC_PCI_REVISION_ID  , DWORD)))
			{
				SD_LOG_DEBUG() << "Backdoor access is allowed even after disabling " << std::endl;
				return test_failed(device);
			}
			test_failed(device);
		}
			
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDDeviceClassRWTest, SDDeviceClassRWTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
