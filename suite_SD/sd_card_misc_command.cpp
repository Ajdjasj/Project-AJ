
#include "sdcard_test.hpp"


class SDCardMiscCommandTest : public SDCardTest
{
  private:

  public:
    SDCardMiscCommandTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardMiscCommandTest()
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
        Result TestResult = ts::Test::Pass;
		uint8_t Devices = 0, Slots = 0;
		bool speed = false;
		bool width = true;
		uint32_t error;
		uint16_t rca;
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SD card detect Failed for device " << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "SD Card Init test Failed for device " << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("Initialization is complete....\n");
				SD_LOG_DEBUG() << "Initialization is complete...." << std::endl;
				
				//CMD9
				_r2 ptr_csd;
				if(!get_csd(device, slot, &ptr_csd, &error))
				{
					SD_LOG_DEBUG() << "SD Card CMD9 test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card CMD9 test Passed....\n");
				SD_LOG_DEBUG() << "SD Card CMD9 test Passed...." << std::endl;
				
				if(!get_rca(&rca, device, slot, &error))
				{
					SD_LOG_DEBUG() << "SD Card get RCA Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				//CMD10
				_r2 ptr_cid;
				if(!get_cid(&ptr_cid, rca, &error))
				{
					SD_LOG_DEBUG() << "SD Card CMD10 test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card CMD10 test Passed....\n");
				SD_LOG_DEBUG() << "SD Card CMD10 test Passed...." << std::endl;
				
				//CMD13
				_r1 status;
				_sd_status sd_status;
				if(!get_sd_status(device, slot, speed , width, &status , &sd_status))
				{
					SD_LOG_DEBUG() << "SD Card CMD13 test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card CMD13 test Passed....\n");
				SD_LOG_DEBUG() << "SD Card CMD13 test Passed...." << std::endl;
				
				//ACMD51
				_scr ptr_scr;
				if(!get_scr(device, slot, &ptr_scr, &status, speed, width))
				{
					SD_LOG_DEBUG() << "SD Card ACMD51 and CMD55 test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card ACMD51 and CMD55 test Passed....\n");
				SD_LOG_DEBUG() << "SD Card ACMD51 and CMD55 test Passed...." << std::endl;
				
				//CMD15
				if(!go_inactive(device, slot, &error))
				{
					SD_LOG_DEBUG() << "SD Card CMD15 test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card CMD15 test Passed....\n");
				SD_LOG_DEBUG() << "SD Card CMD15 test Passed...." << std::endl;
				
				if(get_sd_status(device, slot, speed , width, &status , &sd_status))
				{
					SD_LOG_DEBUG() << "SD Card is not in INACTIVE state for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card is in INACTIVE state. Please remove and insert card.\n");
				SD_LOG_DEBUG() << "SD Card is in INACTIVE state. Please remove and insert card." << std::endl;
			}
		}
		
		SD_LOG_DEBUG() << "SD Card Misc Command Test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardMiscCommandTest, SDCardMiscCommandTest);
