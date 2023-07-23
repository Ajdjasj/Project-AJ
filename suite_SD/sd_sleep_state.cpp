

#include "sdcard_test.hpp"
#include "host_types.h"
#include "sd_wrapper.h"
#include "core_logger.h"
#include "resource_manager.h"

#define S1_STATE 0x01
#define S3_STATE 0x02
#define S4_STATE 0x03

class SleepStatesTest : public SDCardTest
{
  private:

  public:
    SleepStatesTest(const TServerTestFactory &Factory) : SDCardTest(Factory),m_lg("SleepState")
    {
    }

  protected:

    coreutil::Logger    m_lg;
    Target*             m_target_device ;
    Resource_manager*   m_resource_mgr;
    ~SleepStatesTest()
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
	
	bool enter_sleep_state(uint32_t sleep_state)
	{
		char command[50];
		if(sleep_state == S1_STATE)
			sprintf(command,"pwrtest /sleep /c:1 /d:35 /p:40 /t:y /s:1");
		if(sleep_state == S3_STATE)
			sprintf(command,"pwrtest /sleep /c:1 /d:35 /p:40 /t:y /s:3");
		if(sleep_state == S4_STATE)
			sprintf(command,"pwrtest /sleep /c:1 /d:35 /p:40 /t:y /s:4");
		
		if(system(command))
		{
			sd_logmsg("Data Transfer failed When ALink Clock Gating Enabled");
			
		}	
		return true;
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
		uint8_t drive_name = Parameter<uint8_t>("drive",0, coreutil::FmtFlags(std::ios::hex));
		uint8_t *source = (uint8_t*)"log.xml";
		uint8_t destination[5] = "C:\\";
		uint8_t *dest = &destination[0];
		destination[0] = drive_name;
		char command[30];
		SD_LOG_DEBUG() << "Driver Name " << std::dec << drive_name;
		//sprintf(command,"xcopy %s %s /y",source,dest);
		
		for(device = 0; device < Maxdevices; device++)
		{
			Maxslots = get_total_slot(device);
			for(slot = 0; slot < Maxslots; slot++)
			{
				//SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				//Detect Card
				if(!detect_card(device, slot))
				{
				//	SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					return ts::Test::Fail;
				}
				//SD_LOG_DEBUG() << "Card detected"<< std::endl;
				SD_LOG_DEBUG()<< "Card detected"<< std::endl;

#if 0				
				SDCardTest::enable_device(device, true);
				env::SleepMs(1000);
			
				SD_LOG_DEBUG() <<"Entering to Sleep State S3" << std::endl;
				enter_sleep_state(S4_STATE);	
				env::SleepMs(10000);				
				SDCardTest::enable_device(device, false);
				env::SleepMs(1000);
				/********************* Clock Gating Enable and Data Transfer ****************************/
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;				
				if(!SDCardTest::Initalization(device, slot, true))
				{
					SD_LOG_DEBUG() << "Initalization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed" << std::endl;
				if(!SDCardTest::pio_data_transfer(device, slot, (speed_mode == 1 ? false : true), true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed"<< std::endl;
					return ts::Test::Fail;
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;

				//Read/Write Data and Compare the data
				if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, (speed_mode == 1 ? false : true), true, autocmd))
				{
					:("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
					//return ts::Test::Fail;
				}
				sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);	

				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;
	
				CORE_LOG_DEBUG (m_lg) << "Entering to Sleep State S4" << std::endl;
				SDCardTest::enable_device(device, true);
				env::SleepMs(1000);
				env::SleepMs(1000);
				enter_sleep_state(S3_STATE);
				env::SleepMs(10000);
				SDCardTest::enable_device(device, false);
				env::SleepMs(1000);
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
				if(!SDCardTest::pio_data_transfer(device, slot, (speed_mode == 1 ? false : true), true , autocmd , card_address , 0xDEADBEEF , blk_size , blk_count ))
				{
					SD_LOG_DEBUG() << "PIO Data transfer Failed"<< std::endl;
					return ts::Test::Fail;
				}	
				SD_LOG_DEBUG() << "PIO Data transfer Completed" << std::endl;
				

				//Read/Write Data and Compare the data
				if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, (speed_mode == 1 ? false : true), true, autocmd))
				{
					sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
					return ts::Test::Fail;
				}
				sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);	

				//ADMA Data transfer 500Kb
				if(!SDCardTest::ADMA2ReadWrite(device, slot, card_address, data_size_500k, (speed_mode == 1 ? false : true) /*Speed*/, true /*Width*/, autocmd,0x02))
				{
					SD_LOG_DEBUG() << "ADMA read/write failed"<<std::endl; 
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "ADMA Read/Write data_size_500k Completed" << std::endl;
							
				env::SleepMs(500);
			

				
				env::SleepMs(5000);
				SDCardTest::enable_device(device, true);
#endif
				env::SleepMs(1000);
				env::SleepMs(1000);				
				sd_logmsg("Entering to Sleep State S4\n");
				SD_LOG_DEBUG()<<"Entering to Sleep State S4" << std::endl;
				enter_sleep_state(S4_STATE);	
				env::SleepMs(10000);	

				if(system(command))
				{
					//SD_LOG_DEBUG() << "Data Transfer failed When ALink Clock Gating Enabled"<< std::endl;
					SD_LOG_DEBUG()<< "Data Transfer failed When ALink Clock Gating Enabled"<< std::endl;
					return ts::Test::Fail;
				}
				env::SleepMs(1000);	



				
				sd_logmsg("Entering to Sleep State S3\n");
				SD_LOG_DEBUG()<<"Entering to Sleep State S3\n"<<std::endl;
				enter_sleep_state(S3_STATE);	
				env::SleepMs(1000);				

				if(system(command))
				{
				//	SD_LOG_DEBUG() << "Data Transfer failed When ALink Clock Gating Enabled"<< std::endl;
					SD_LOG_DEBUG()<<"Data Transfer failed When ALink Clock Gating Enabled"<< std::endl;
					return ts::Test::Fail;
				}
				env::SleepMs(5000);				
				
			}
		}
		return ts::Test::Pass;	
	}
};

TServerTestInstance(SleepStatesTest, SleepStatesTest);
