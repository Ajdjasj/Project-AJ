#include "sdcard_test.hpp"

class SDCompliance_4_1_5Test : public SDCardTest
{
  private:

  public:
    SDCompliance_4_1_5Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_4_1_5Test()
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
		printf("Please Capture the trace and press Enter to exit...\n");
		getchar();
		
		SDCardTest::PostRun();
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		uint8_t device = 0, slot = 0;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled =false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			printf("Initialization Failed....\n");
			SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
			return ts::Test::Fail;
		}
		printf("Initialization Complete....\n");
		SD_LOG_DEBUG() << "Initialization Complete...." << std::endl;
		
		//Get SCR
		bool speed = false, width = false;
		_r1 status;
		_scr ptr_scr;
		if(!get_scr(device, slot, &ptr_scr, &status, speed, width))
		{
			SD_LOG_DEBUG() << "Get SCR Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		printf("Get SCR Passed....\n");
		
		printf("SD spec: 0x%x\n", ptr_scr.sd_spec());
		SD_LOG_DEBUG() << "SD spec: " << ptr_scr.sd_spec() << std::endl;
		printf("SD spec3: 0x%x\n", ptr_scr.sd_spec3());
		SD_LOG_DEBUG() << "SD spec3: " << ptr_scr.sd_spec3() << std::endl;
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 4.1-5 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_4_1_5Test, SDCompliance_4_1_5Test);