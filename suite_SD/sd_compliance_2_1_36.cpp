#include "sdcard_test.hpp"


class SDCompliance_2_1_36Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_36Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_36Test()
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
		uint8_t autocmd = 0x0, Write_buffer[512] = {0}, Read_buffer[512] = {0};
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 0;
		//uint32_t card_start_address = 0x8000;
		//uint32_t card_end_address = 0;
		
		//Initialization
		//_init_status init_status;
		bool voltage_switch_enabled =false;
		if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
		{
			SD_LOG_DEBUG() << "Initialization Failed" << std::endl;
			return ts::Test::Fail;
		}
		printf("Initialization Complete\n");
		
	#if 1
		//Single block Read/Write test
		//Write the data
		for( uint32_t j=0; j<blk_size; j++)
		{
			Write_buffer[j] = j+1;
		}
		if(!pio_write( device, slot, Speed, BusWidth, autocmd, card_address, Write_buffer, blk_size, blk_count))
		{
			printf("Failed to write the data....\n");
			return ts::Test::Fail;
		}
		printf("PIO single block write complete for device %d and slot %d....\n", device, slot);
		
		//Reading the data
		if(!pio_read( device, slot, Speed, BusWidth, autocmd, card_address, Read_buffer, blk_size, blk_count))
		{
			printf("Failed to read the data....\n");
			return ts::Test::Fail;
		}
		printf("PIO single block read complete for device %d and slot %d....\n", device, slot);
		for(uint32_t j=0; j < blk_size; j++)
		{
			if(Write_buffer[j] != Read_buffer[j])
			{
				SD_LOG_DEBUG() << "SDCard PIO Single block Read/Write test Failed for data: %d" << j << std::endl;
				return ts::Test::Fail;
			}
			
		}
	#endif
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-36 test Successful" << std::endl;
		return TestResult; 
    }
};


TServerTestInstance(SDCompliance_2_1_36Test, SDCompliance_2_1_36Test);