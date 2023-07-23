#include "sdcard_test.hpp"

class SDCompliance_2_1_6Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_6Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_6Test()
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
		
		//Get CSD (CMD9)
		_r2 ptr_csd;
		uint32_t error = 0x00;
		if(!get_csd( &ptr_csd, &error))
		{
			SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		printf("Get CSD Passed....\n");
		SD_LOG_DEBUG() << "Get CSD Passed...." << std::endl;
		
		printf("Card Command Classes supported: 0x%x\n", ptr_csd.ccc());
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-6 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_2_1_6Test, SDCompliance_2_1_6Test);