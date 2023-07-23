#include "sdcard_test.hpp"

static uint8_t LockCard[18] = {0};

class SDCompliance_2_1_23Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_23Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_23Test()
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
		_r1 card_status;
		uint32_t error = 0x00;
		bool Speed = false;
		bool BusWidth = false;
		uint8_t autocmd = 0x1, Write_buffer[512] = {0}, Read_buffer[512] = {0};
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 10;
		
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
		
		//Get the card status
		if(!get_card_status( &card_status, &error ))
		{
			printf("Failed for get card status....\n");
			SD_LOG_DEBUG() << "Failed for get card status...." << std::endl;
			return ts::Test::Fail; 
		}
		
		//If OUT_OF_RANGE or ADDRESS_ERROR or BLOCK_LEN_ERROR or CARD_ECC_FAILED or WP_VIOLATION or CC_ERROR or ERROR is set
		if(card_status.bit.out_of_range || card_status.bit.address_error || card_status.bit.block_len_error || card_status.bit.card_ecc_failed || card_status.bit.wp_violation || card_status.bit.cc_error || card_status.bit.error)
		{
			//Multi block Write
			//Writing the data
			for( uint32_t j=0; j<blk_size; j++)
			{
				Write_buffer[j] = j+1;
			}
			if(!pio_write( device, slot, Speed, BusWidth, autocmd, card_address, Write_buffer, blk_size, blk_count))
			{
				printf("Failed to read the data....\n");
				SD_LOG_DEBUG() << "Failed to read the data...." << std::endl;
				return ts::Test::Fail;
			}
			
			//Multi block Read
			//Reading the data
			if(!pio_read( device, slot, Speed, BusWidth, autocmd, card_address, Read_buffer, blk_size, blk_count))
			{
				printf("Failed to read the data....\n");
				SD_LOG_DEBUG() << "Failed to read the data...." << std::endl;
				return ts::Test::Fail;
			}
			
			//Get the card status
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				printf("Failed for get card status after multi read block....\n");
				SD_LOG_DEBUG() << "Failed for get card status after multi read block...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.out_of_range)
			{
				printf("Failed to recover for out of range error....\n");
				SD_LOG_DEBUG() << "Failed to recover for out of range error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.address_error)
			{
				printf("Failed to recover for address error....\n");
				SD_LOG_DEBUG() << "Failed to recover for address error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.block_len_error)
			{
				printf("Failed to recover for block length error....\n");
				SD_LOG_DEBUG() << "Failed to recover for block length error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.card_ecc_failed)
			{
				printf("Failed to recover for card ecc failed error....\n");
				SD_LOG_DEBUG() << "Failed to recover for card ecc failed error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.wp_violation)
			{
				printf("Failed to recover for write protect violation error....\n");
				SD_LOG_DEBUG() << "Failed to recover for write protect violation error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.cc_error)
			{
				printf("Failed to recover for CC error....\n");
				SD_LOG_DEBUG() << "Failed to recover for CC error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			if(card_status.bit.error)
			{
				printf("Failed to recover for error....\n");
				SD_LOG_DEBUG() << "Failed to recover for error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
		}
		//If ERASE_SEQ_ERROR or ERASE_PARAM is set
		else if(card_status.bit.erase_seq_error || card_status.bit.erase_param)
		{
			//uint32_t card_start_address = 0x8000;
			uint32_t card_end_address = 0;
			
			card_end_address = (card_address + blk_size);
			//Erase sequence
			erase_start_add(card_address, &card_status, &error);
			erase_end_add(card_end_address, &card_status, &error);
			erase_cmd(&card_status, &error);
			
			//Get the card status
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				printf("Failed for get card status after erase sequence....\n");
				SD_LOG_DEBUG() << "Failed for get card status after erase sequence...." << std::endl;
				return ts::Test::Fail; 
			}
			if(card_status.bit.erase_seq_error)
			{
				printf("Failed to recover for erase sequence error....\n");
				SD_LOG_DEBUG() << "Failed to recover for erase sequence error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
			else if(card_status.bit.erase_param)
			{
				printf("Failed to recover for erase parameter error....\n");
				SD_LOG_DEBUG() << "Failed to recover for erase parameter error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
		}
		//If LOCK_UNLOCK_FAILED is set
		else if(card_status.bit.lock_unlock_fail)
		{
			printf("\n");
			//Lock with password
			LockCard[0] = 0x5;
			LockCard[1] = 0x2;
			LockCard[2] = 'a';
			LockCard[3] = 'b';
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("set passwd check lock_unlock function\n");
				return ts::Test::Fail; 
			}
			
			printf("\n");
			//Unlock with password
			LockCard[0] = 0x2;
			LockCard[1] = 0x2;
			LockCard[2] = 'a';
			LockCard[3] = 'b';
			if(!lock_unlock(device, slot, LockCard, true, false, &card_status))
			{
				printf("set passwd check lock_unlock function\n");
				return ts::Test::Fail; 
			}
			
			//Get the card status
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				printf("Failed for get card status after lock unlock....\n");
				SD_LOG_DEBUG() << "Failed for get card status after lock unlock...." << std::endl;
				return ts::Test::Fail; 
			}
			if(card_status.bit.lock_unlock_fail)
			{
				printf("Failed to recover for lock/unlock failed error....\n");
				SD_LOG_DEBUG() << "Failed to recover for lock/unlock failed error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
		}
		//If CSD_OVERWRITE is set
		else if(card_status.bit.csd_overwrite)
		{
			//Get CSD (CMD9)
			_r2 ptr_csd;
			uint32_t error = 0x00;
			if(!get_csd( &ptr_csd, &error))
			{
				printf("Get CSD Failed for device %d and slot %d\n", device, slot);
				SD_LOG_DEBUG() << "Get CSD Failed for device" << device << " and slot" << slot << std::endl;
				return ts::Test::Fail;
			}
		
			//Get the card status
			error = 0x00;
			if(!get_card_status(&card_status, &error ))
			{
				printf("Failed for get card status after ....\n");
				SD_LOG_DEBUG() << "Failed for get card status after ...." << std::endl;
				return ts::Test::Fail; 
			}
			if(card_status.bit.csd_overwrite)
			{
				printf("Failed to recover for CSD overwrite error....\n");
				SD_LOG_DEBUG() << "Failed to recover for CSD overwrite error...." << std::endl;
				TestResult = ts::Test::Fail; 
			}
		}
		
		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.1-23 test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCompliance_2_1_23Test, SDCompliance_2_1_23Test);