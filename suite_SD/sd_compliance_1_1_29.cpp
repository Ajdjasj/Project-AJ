#include "sdcard_test.hpp"

class SDCompliance_1_1_29Test : public SDCardTest
{
  private:

  public:
    SDCompliance_1_1_29Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_1_1_29Test()
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
		SD_LOG_DEBUG() << "Initialization Complete....\n" << std::endl;
		uint32_t error = 0x00;
		_r2 csd;
		printf("Reading CSD...\n");
		if(!get_csd( &csd, &error))
		{
			SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		printf("CSD  = %16lX - %16lX\n",csd.get_lhs(),csd.get_rhs());

		uint32_t loop =0;
		uint8_t csd_array[18]; 
		for (loop = 0 ; loop < 8 ; loop++)
		{
			csd_array[loop] = ((csd.get_rhs() >> (loop * 8)) & (0xFF));
		}
		for (loop = 8 ; loop < 15 ; loop++)
		{
			csd_array[loop] = ((csd.get_lhs() >> ((loop - 8) * 8)) & (0xFF));
		}
		printf("\n");
		printf("Data \n");
		//csd_array[0] = (csd_array[0] & 0x10 ) == 0x10 ? (csd_array[0] & 0xEF) : (csd_array[0] | 0x10);
		//printf("Toggling tem_write_protection bit\n",csd_array[0]);
		for (loop = 0 ; loop < 15 ; loop++)
		{
				printf("%X ", csd_array[15-loop]);
		}	
		printf("\n");
		
		_r1 status;
		printf("Programing CSD...\n");
		if(!set_csd_1_1_29( device,  slot,&csd_array[0],true, true ,&status))
		{
			printf("Program CSD failed\n");
		}
		
		printf("Reading CSD...\n");
		if(!get_csd( &csd, &error))
		{
			SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		//printf("CSD  = %16X - %16X\n",csd.get_lhs(),csd.get_rhs());
		
		SD_LOG_DEBUG() << "SD Card Compliance 1.1-29 test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCompliance_1_1_29Test, SDCompliance_1_1_29Test);