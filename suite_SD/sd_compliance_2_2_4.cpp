#include "sdcard_test.hpp"

class SDCompliance_2_2_4Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_2_4Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_2_4Test()
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
		bool speed = false, width = false;
		uint8_t buffer[0x40];
		_r1 status;
		
		uint32_t argument = Parameter<uint32_t>("argument",0x00000001, coreutil::FmtFlags(std::ios::hex));
		//uint16_t speed_mode = Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));
		//uint16_t type = Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex));
		
		SD_LOG_DEBUG() << "argument" <<std::hex << argument<< std::endl;
		SD_LOG_DEBUG() << "argument" <<std::hex << argument<< std::endl;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled=false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			
			SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
			return ts::Test::Fail;
		}
		
		SD_LOG_DEBUG() << "Initialization Complete...." << std::endl;
		
		if(!issue_switch_function(device, slot, speed, width, argument, &status, &buffer[0] ))
		{
			
			SD_LOG_DEBUG() << "issue_switch_function Failed...." << std::endl;
			TestResult = ts::Test::Fail;
		}
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.2-4 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_2_2_4Test, SDCompliance_2_2_4Test);