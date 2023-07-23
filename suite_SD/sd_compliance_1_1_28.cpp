#include "sdcard_test.hpp"

class SDCompliance_1_1_28Test : public SDCardTest
{
  private:

  public:
    SDCompliance_1_1_28Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_1_1_28Test()
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
			//printf("Initialization Failed....\n");
			SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
			return ts::Test::Fail;
		}
		//printf("Initialization Complete....\n");
		SD_LOG_DEBUG() << "Initialization Complete....\n" << std::endl;
		uint32_t error = 0x00;
		_r2 csd;
		SD_LOG_DEBUG() << " First time....> Reading CSD...\n"<< std::endl;
		if(!get_csd( &csd, &error))
		{
			SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		SD_LOG_DEBUG() << "CSD LHS:: "<< std::hex<< csd.get_lhs()<< endl;
		SD_LOG_DEBUG() <<"CSD RHS:: "<< std::hex <<csd.get_rhs()<< std::endl;
		
		printf( "using pritf----> CSD LHS:: 0x%16lx", csd.get_lhs() );
		printf( "using pritf---->  CSD RHS:: 0x%16lx", csd.get_rhs() );

		uint32_t loop =0;
		uint8_t csd_array[18]; 
		
		SD_LOG_DEBUG() <<"copying CSD register LHS and RHS values into .... char csd_array[15],  byte by byte.. "<<std::endl;
		for (loop = 0 ; loop < 8 ; loop++)
		{
			csd_array[loop] = ((csd.get_rhs() >> (loop * 8)) & (0xFF));
		}
		for (loop = 8 ; loop < 15 ; loop++)
		{
			csd_array[loop] = ((csd.get_lhs() >> ((loop - 8) * 8)) & (0xFF));
		}
		
		SD_LOG_DEBUG() << "CSD Data, after storing in csd_array[15] ..."<< std::endl;
		for (loop = 0 ; loop < 15 ; loop++)
		{
				SD_LOG_DEBUG() << " csd_array[ " << loop << " ]:: "<< std::hex<< csd_array[14-loop]<< std::endl;
		}	
		
		SD_LOG_DEBUG() << "Toggling csd_array[0] byte.... bit:7"<< std::endl;
		csd_array[0] = (csd_array[0] & 0x10 ) == 0x10 ? (csd_array[0] & 0xEF) : (csd_array[0] | 0x10);

		
		
		SD_LOG_DEBUG() << " After Toggling csd_array[0] byte.... bit:7, the csd_array[15] ..."<< std::endl;
		for (loop = 0 ; loop < 15 ; loop++)
		{
				SD_LOG_DEBUG() << " csd_array[ " << loop << " ]:: "<< std::hex<< csd_array[14-loop]<< std::endl;
		}
		
				
		
		//SD_LOG_DEBUG() << "Toggling tem_write_protection bit\n"<<csd_array[0]<< std::endl;
		
		_r1 status;
		SD_LOG_DEBUG() << "Programing CSD...\n"<< std::endl;
		if(!set_csd_1_1_29( device,  slot,&csd_array[0],false, false ,&status))
		{
			SD_LOG_DEBUG() << "Program CSD failed\n"<< std::endl;
		}
		
		SD_LOG_DEBUG() << " Second time Reading CSD..."<< std::endl;
		if(!get_csd( &csd, &error))
		{
			SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		
		//SD_LOG_DEBUG() << "CSD  = %llX - %llX\n"<<std::hex << csd.get_lhs()<< std::hex<< csd.get_rhs()<< std::endl;
		SD_LOG_DEBUG() << "CSD LHS:: "<< std::hex<< csd.get_lhs()<< endl;
		SD_LOG_DEBUG() <<"CSD RHS:: "<< std::hex <<csd.get_rhs()<< std::endl;
		
		printf( "using pritf----> CSD LHS:: 0x%16lx", csd.get_lhs() );
		printf( "using pritf---->  CSD RHS:: 0x%16lx", csd.get_rhs() );
		
		for (loop = 0 ; loop < 8 ; loop++)
		{
			csd_array[loop] = ((csd.get_rhs() >> (loop * 8)) & (0xFF));
		}
		for (loop = 8 ; loop < 15 ; loop++)
		{
			csd_array[loop] = ((csd.get_lhs() >> ((loop - 8) * 8)) & (0xFF));
		}
		
		SD_LOG_DEBUG() << "CSD Data, after storing in csd_array[15], 2nd time ..."<< std::endl;
		for (loop = 0 ; loop < 15 ; loop++)
		{
				SD_LOG_DEBUG() << " csd_array[ " << loop << " ]:: "<< std::hex<< csd_array[14-loop]<< std::endl;
		}	
		
		SD_LOG_DEBUG() << "Toggling csd_array[0] byte.... bit:7"<< std::endl;
		
		
		
		//SD_LOG_DEBUG() << "Toggling tem_write_protection bit\n"<< csd_array[0]<< std::endl;
		csd_array[0] = (csd_array[0] & 0x10 ) == 0x10 ? (csd_array[0] & 0xEF) : (csd_array[0] | 0x10);
		
		
		SD_LOG_DEBUG() << " After Toggling csd_array[0] byte.... bit:7, the csd_array[15] ..."<< std::endl;
		for (loop = 0 ; loop < 15 ; loop++)
		{
			SD_LOG_DEBUG() << " csd_array[ " << loop << " ]:: "<< std::hex<< csd_array[14-loop]<< std::endl;
		}
		
		
		
		SD_LOG_DEBUG() << "\n"<< std::endl;
		
		
		
		
		
		SD_LOG_DEBUG() << "2nd time ..Programing CSD...\n"<< std::endl;
		if(!set_csd_1_1_29( device,  slot,&csd_array[0],false, false ,&status))
		{
			SD_LOG_DEBUG() << "Program CSD failed\n"<< std::endl;
		}
		
		
		
		
		SD_LOG_DEBUG() << " Third time Reading CSD..."<< std::endl;
		if(!get_csd( &csd, &error))
		{
			SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
			return ts::Test::Fail;
		}
		
		//SD_LOG_DEBUG() << "CSD  = %llX - %llX\n"<<std::hex << csd.get_lhs()<< std::hex<< csd.get_rhs()<< std::endl;
		SD_LOG_DEBUG() << "CSD LHS:: "<< std::hex<< csd.get_lhs()<< endl;
		SD_LOG_DEBUG() <<"CSD RHS:: "<< std::hex <<csd.get_rhs()<< std::endl;
		
		printf( "using pritf----> CSD LHS:: 0x%16lx", csd.get_lhs() );
		printf( "using pritf---->  CSD RHS:: 0x%16lx", csd.get_rhs() );
		
					
		SD_LOG_DEBUG() << "SD Card Compliance 1.1-28 test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCompliance_1_1_28Test, SDCompliance_1_1_28Test);