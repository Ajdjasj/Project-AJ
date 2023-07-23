#include "sdcard_test.hpp"


class SDCompliance_2_1_9Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_9Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_9Test()
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
		//uint8_t autocmd = 0x0, Write_buffer[512] = {0}, Read_buffer[512] = {0};
		uint32_t card_address = 0x8000, blk_size = 512; //, blk_count = 0;
		//uint32_t card_start_address = 0x8000;
		 uint32_t card_end_address = 0;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled =false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			SD_LOG_DEBUG() << "Initialization Failed" << std::endl;
			return ts::Test::Fail;
		}
		printf("Initialization Complete\n");
		
		//Erase the Data
		_r1 status;
		_scr SCR;
		if(!get_scr(device, slot, &SCR, &status, Speed, BusWidth))
		{
			printf("Failed to get the SCR....\n");
			return ts::Test::Fail;
		}
		
		uint8_t EraseValue = 0xFF;
		if(!SCR.data_stst_after_erase())
			EraseValue = 0x0;
		printf("Erase value: 0x%x\n", EraseValue);
		SD_LOG_DEBUG() << "Erase value: 0x%x" << EraseValue << std::endl;
		if(1 == status.bit.erase_seq_error)
		{
			printf("Before - Erase sequence error\n");
			SD_LOG_DEBUG() << "Before - Erase sequence error" << std::endl;
		}
		else
		{
			printf("Before - No Erase sequence error\n");
			SD_LOG_DEBUG() << "Before - Np Erase sequence error" << std::endl;
		}
		
		//Erase Sequence
		uint32_t error = 0x00;
		card_end_address = (card_address + blk_size);
		//Erase sequence
		erase_start_add(card_address, &status, &error);
		erase_end_add(card_end_address, &status, &error);
		erase_cmd(&status, &error);
		
		//Get SCR to check sequence error
		if(!get_scr(device, slot, &SCR, &status, Speed, BusWidth))
		{
			printf("Failed to get the status....\n");
			SD_LOG_DEBUG() << "Failed to get the status...." << std::endl;
			TestResult = ts::Test::Fail;
		}

		if(status.bit.erase_seq_error)
		{
			printf("Failed: Erase sequence error is set %d\n", status.bit.erase_seq_error);
			SD_LOG_DEBUG() << "Failed: Erase sequence error is set" << status.bit.erase_seq_error << std::endl;
			TestResult = ts::Test::Fail;
		}
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-9 test Successful" << std::endl;
		return TestResult; 
    }
};


TServerTestInstance(SDCompliance_2_1_9Test, SDCompliance_2_1_9Test);