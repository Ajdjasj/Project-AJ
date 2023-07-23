#include "sdcard_test.hpp"

class SDCompliance_2_1_1Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_1Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_1Test()
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
		
		//Memort Capacity = BlockNR * BlockLength
		//BlockNR = (C_SIZE+1) * MULT
		//MULT = 2pow(C_SIZE_MULT + 2)  where C_SIZE_MULT should be less 8
		//BlockLength = 2pow(READ_BL_LENGTH)  where READ_BL_LENGTH is less than 12
		double mult = (1 << ( ptr_csd.c_size_mult() + 2) );
		printf("mult: %f\n", mult);
		SD_LOG_DEBUG() << "mult: " << mult << std::endl;
		double BlockNR = ( ptr_csd.c_size() + 1 ) * mult;
		printf("BlockNR: %f\n", BlockNR);
		SD_LOG_DEBUG() << "BlockNR: " << BlockNR << std::endl;
		double BlockLength = (1 << ptr_csd.read_bl_length() );
		printf("BlockLength: %f\n", BlockLength);
		SD_LOG_DEBUG() << "BlockLength: " << BlockLength << std::endl;
		double MemCapacity = BlockNR * BlockLength;
		printf("Memory Capacity: 0x%lf\n", MemCapacity);
		SD_LOG_DEBUG() << "Memory Capacity: " << MemCapacity << std::endl;
		
		//Get SCR
		bool speed = false, width = false;
		_r1 status;
		_scr ptr_scr;
		if(!get_scr(device, slot, &ptr_scr, &status, speed, width))
		{
			SD_LOG_DEBUG() << "Get SCR Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		
		printf("SD spec: 0x%x\n", ptr_scr.sd_spec());
		SD_LOG_DEBUG() << "SD spec: " << ptr_scr.sd_spec() << std::endl;
		printf("SD spec3: 0x%x\n", ptr_scr.sd_spec3());
		SD_LOG_DEBUG() << "SD spec3: " << ptr_scr.sd_spec3() << std::endl;
		
		if(MemCapacity < 3.9)
		{
			printf("Size is not 4GB\n");
			SD_LOG_DEBUG() << "Size is not 4GB"  << std::endl;
			TestResult = ts::Test::Fail;
		}
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-1 test Successful" << std::endl;
		return TestResult; 
    }
	
};


TServerTestInstance(SDCompliance_2_1_1Test, SDCompliance_2_1_1Test);