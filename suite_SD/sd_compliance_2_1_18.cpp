#include "sdcard_test.hpp"

class SDCompliance_2_1_18Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_18Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_18Test()
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
		//uint16_t rca_value;
		uint32_t error = 0x00;
		_r1 status;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled =false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			//printf("In 2.1-18 test: Failed Initialization....\n");
			SD_LOG_DEBUG() << "In 2.1-18 test:  Failed Initialization...." << std::endl;
			return ts::Test::Fail;
		}
		//printf(" In 2.1-18 test: Initialization Complete....\n");
		SD_LOG_DEBUG() << "In 2.1-18 test: Initialization Complete...." << std::endl;
		
	#if 0
		//get_rca(&rca_value, device, slot, &error);
		//Get RCA (CMD3)
		if(!get_rca(&rca_value, device, slot, &error))
		{
			//printf("Failed to get RCA....\n");
			SD_LOG_DEBUG() << "Failed to get RCA...." << std::endl;
			return ts::Test::Fail;
		}
		SD_LOG_DEBUG() << "RCA value: " << rca_value << std::endl;
		
		//if(0 == rca_value)
		{
			//Get RCA other than ZERO
			error = 0x0;
			//get_rca(&rca_value, device, slot, &error);
			if(!get_rca(&rca_value, device, slot, &error))
			{
				//printf("Failed to get RCA....\n");
				SD_LOG_DEBUG() << "Failed to get RCA...." << std::endl;
				return ts::Test::Fail;
			}
			SD_LOG_DEBUG() << "RCA value: " << rca_value << std::endl;
		}
	#endif

#if 1
		//select the card
		//select_card(device, slot, &status, &error);
		if(!select_card( &status, &error))
		{
			
			//printf("Failed to get RCA....\n");
			SD_LOG_DEBUG() << "Failed to get RCA...." << std::endl;
			return ts::Test::Fail;
		}
		//Get card status
		if(!get_card_status( &status, &error))
		{
			//printf("Failed to get card status....\n");
			SD_LOG_DEBUG() << "Failed to get card status...." << std::endl;
			return ts::Test::Fail;
		}
		if(tran != status.bit.current_state)
		{
			//printf("After Select the state is not transfer mode: 0x%xss\n", status.bit.current_state);
			SD_LOG_DEBUG() << "After Select the state is not transfer mode: " << status.bit.current_state << std::endl;
			TestResult = ts::Test::Fail;
		}
		//printf("After Select the state is transfer mode\n");
		SD_LOG_DEBUG() << "After Select the state is transfer mode" << std::endl;

#endif
		//De-select the card
		//deselect_card(device, slot, &status, &error);
		if(!deselect_card( &status, &error))
		{
			//printf("Failed to get RCA....\n");
			SD_LOG_DEBUG() << "Failed to get RCA...." << std::endl;
			return ts::Test::Fail;
		}
		//Get card status
		if(!get_card_status( &status, &error))
		{
			//printf("Failed to get card status....\n");
			SD_LOG_DEBUG() << "Failed to get card status...." << std::endl;
			return ts::Test::Fail;
		}
		if(stby != status.bit.current_state)
		{
			//printf("After Deselect the state is not standby mode: 0x%xss\n", status.bit.current_state);
			SD_LOG_DEBUG() << "After Deselect the state is not standby mode: " << status.bit.current_state << std::endl;
			TestResult = ts::Test::Fail;
		}
		//printf("After Deselect the state is standby mode\n");
		SD_LOG_DEBUG() << "After Deselect state the is standby mode" << std::endl;
	
	#if 0
		//Get RCA( Issue CMD3 again)
		error = 0x0;
		if(!get_rca(&rca_value, device, slot, &error))
		{
			//printf("Failed to get RCA....\n");
			SD_LOG_DEBUG() << "Failed to get RCA...." << std::endl;
			return ts::Test::Fail;
		}
		//printf("RCA value: \n", rca_value);
		SD_LOG_DEBUG() << "RCA value: " << rca_value << std::endl;
	#endif
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-18 test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCompliance_2_1_18Test, SDCompliance_2_1_18Test);