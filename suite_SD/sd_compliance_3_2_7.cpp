#include "sdcard_test.hpp"

class SDCompliance_3_2_7Test : public SDCardTest
{
  private:

  public:
    SDCompliance_3_2_7Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_3_2_7Test()
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
		
		//Get SD Status (ACMD13)
		bool speed = false, width = false;
		_r1 status;
		_sd_status sd_status;
		if(!get_sd_status( device, slot, speed , width, &status, &sd_status))
		{
			printf("Get SD Status Failed....\n");
			SD_LOG_DEBUG() << "Get SD Status Failed...." << std::endl;
			return ts::Test::Fail;
		}
		printf("Get SD Status Complete....\n");
		SD_LOG_DEBUG() << "Get SD Status Complete...." << std::endl;
		
		if(true == init_status.card.sdhc_sdxc)
			printf("Card is SDXC/SDHC\n");
		else
			printf("Card is SDSC\n");
		
		printf("Size of Protected Area: 0x%x\n", (uint32_t)sd_status.size_of_protected_area);
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 3.2-7 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_3_2_7Test, SDCompliance_3_2_7Test);