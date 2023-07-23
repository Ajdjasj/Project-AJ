
#include "sdcard_test.hpp"
#include "host_types.h"


class SDFeedBackClockTest : public SDCardTest
{
  private:

  public:
	uint32_t Restore_SD_control_register;
	bool restore;
    SDFeedBackClockTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDFeedBackClockTest()
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
	
	ts::Test::Result test_failed(uint32_t device)
	{
		if(restore == true)
		{
		//Restore the register on test failure
		write_pcicfg(device, SDHC_PCI_SDCONTROL   , DWORD ,Restore_SD_control_register);
		}
		return ts::Test::Fail;
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
		uint32_t card_address = 0x2000, blk_size = 512, blk_count = 100;
		uint8_t autocmd = 0x01;
	
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

				//Check High mode is supported by the Host
				if(!init_status.host.hs)
				{
					SD_LOG_DEBUG() << "High Speed Mode is not supported by the Host"<< std::endl;
					return ts::Test::Fail;					
				}
				
				if(!get_supported_functions(device , slot, false, false, &function, &status))
				{
					SD_LOG_DEBUG() << "get_supported_functions failed"<< std::endl;
					return ts::Test::Fail;
				}
				//Check High mode is supported by the card
				if(!(speed_mode & function.group1))
				{
					SD_LOG_DEBUG() << "High Speed Mode is not supported by the Card" << std::endl;
					return ts::Test::Fail;			
				}
				//Change Speed Mode to HS
				if(!set_normal_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
				{
					SD_LOG_DEBUG() << "set High speed mode failed "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "set High speed mode Completed"<< std::endl;
				
				//Save FeedBack Clock Option 
				Restore_SD_control_register = read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
				SD_control_register_save	= Restore_SD_control_register;
				//Enable Feedback Clock Option
				write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD ,(SD_control_register_save | (1 << 23)) );
				restore = true;
				if((SD_control_register_save | (1 << 23)) == (read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD)))
					SD_LOG_DEBUG() << "FeedBack Clock Enabled" << std::endl;
				else
				{
					SD_LOG_DEBUG() << "FeedBack Clock Enable failed" << std::endl;
					return test_failed(device);
				}
				//Do PIO, SDMA, ADMA data transfer
				if(!SDCardTest::pio_data_transfer(device, slot, true, true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed in High Speed Mode"<< std::endl;
					return test_failed(device);
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;				
			
				
				
				//Disable Feedback Clock Option
				
				write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD ,(SD_control_register_save & (!(1 << 23)) ) );
				if((SD_control_register_save & (!(1 << 23))) == (read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD)))
					SD_LOG_DEBUG() << "FeedBack Clock Disabled" << std::endl;
				else
				{
					SD_LOG_DEBUG() << "FeedBack Clock Disable failed" << std::endl;
					return test_failed(device);
				}			
				
				
				//Do PIO, SDMA, ADMA data transfer
				if(!SDCardTest::pio_data_transfer(device, slot, true, true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed in High Speed Mode"<< std::endl;
					return test_failed(device);
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;				

			}
			test_failed(device);
		}
		
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDFeedBackClockTest, SDFeedBackClockTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
