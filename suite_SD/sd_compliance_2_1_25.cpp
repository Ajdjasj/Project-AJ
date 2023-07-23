#include "sdcard_test.hpp"


class SDCompliance_2_1_25Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_25Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_25Test()
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
		bool Speed = false;
		bool BusWidth = false;
		uint8_t autocmd = 0x1,  Read_buffer[512*100] = {0};
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 100;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled =false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			SD_LOG_DEBUG() << "In 2.1-25 test:  Initialization Failed" << std::endl;
			return ts::Test::Fail;
		}
		SD_LOG_DEBUG() << "In 2.1-25 test:  Initialization Complete\n"<< std::endl;
		
		//Multi block Read
		//Reading the data
		if(!pio_read( device, slot, Speed, BusWidth, autocmd, card_address, Read_buffer, blk_size, blk_count))
		{
			SD_LOG_DEBUG() <<" In 2.1-25 test:  Failed to read the data....\n"<< std::endl;
			return ts::Test::Fail;
		}
		SD_LOG_DEBUG() << "PIO multi block read complete for device %d "<< device << "and slot %d....\n"<< slot<< std::endl;
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-36 test Successful" << std::endl;
		return TestResult; 
    }
};


TServerTestInstance(SDCompliance_2_1_25Test, SDCompliance_2_1_25Test);