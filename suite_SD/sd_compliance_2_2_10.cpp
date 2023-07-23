#include "sdcard_test.hpp"

static uint8_t LockCard[18] = {0};

class SDCompliance_2_2_10Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_2_10Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_2_10Test()
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
		//char password[32] = "0123456789ABCDEF";
		_r1 card_status;
		uint32_t error = 0x00;
		bool Speed = false;
		bool BusWidth = false;
		uint8_t autocmd = 0x0, Write_buffer[512] = {0}, Read_buffer[512] = {0};
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 0;
		
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
		
		//1.	Write data to card
		//2.	Set the TWP(bit-12) using CMD27(PROGRAM_CSD)
		//3.	Set the write protection bit of the address group using CMD28(SET_WRITE_PROT)
		//4.	Get the status of write protection using CMD30(SEND_WRITE_PROT)
		//5.	Lock the card
		//6.	Perform forced erase
		//7.	Ensure secured data is not erased

		//Write data to card
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
		
		error = 0x00;
		if(!get_card_status(&card_status, &error ))
		{
			printf("Failed in get_card_status function\n");
			return ts::Test::Fail; 
		}
		if((!card_status.bit.card_is_locked))
		{
			printf("\n");
			//Lock the card
			LockCard[0] = 0x5;
			LockCard[1] = 0x3;
			LockCard[2] = 'x';
			LockCard[3] = 'y';
			LockCard[4] = 'z';
			
			if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
			{
				printf("Failed in lock_unlock function\n");
				return ts::Test::Fail; 
			}
			error = 0x00;
			if(!get_card_status( &card_status, &error ))
			{
				printf("Failed in get_card_status function\n");
				return ts::Test::Fail; 
			}
			//printf("Card Status: 0x%x\n", card_status);
			printf("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
			printf("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
			if((card_status.bit.lock_unlock_fail) || (!card_status.bit.card_is_locked))
			{
				printf("Failed to Set the password\n");
				SD_LOG_DEBUG() << "Failed to Set the password" << std::endl;
				if(!card_status.bit.card_is_locked)
				{
					printf("Card is still unlocked\n");
					SD_LOG_DEBUG() << "Card is still unlocked" << std::endl;
				}
				TestResult = ts::Test::Fail; 
			}
			
			//Perform forced erase
			//if(card_status.bit.card_is_locked)
			{
				printf("\n");
				printf("----Card is locked ----\n");
				LockCard[0] = 0x08;
				if(!lock_unlock(device, slot, LockCard, false, false, &card_status))
				{
					printf("Failed in force_unlock function\n");
					return ts::Test::Fail;
				}
				error = 0x00;
				if(!get_card_status(  &card_status, &error ))
				{
					printf("Failed in get_card_status function\n");
					return ts::Test::Fail;
				}
				//printf("Card Status: 0x%x\n", card_status);
				printf("Card Error status: 0x%x\n", card_status.bit.lock_unlock_fail);
				printf("Card Lock status: 0x%x\n", card_status.bit.card_is_locked);
				if((card_status.bit.lock_unlock_fail) || (card_status.bit.card_is_locked))
				{
					printf("Failed to force erase\n");
					TestResult = ts::Test::Fail;
				}
			}
			
			//Ensure secured data is not erased
			{
			}
		}

		if(ts::Test::Pass == TestResult)
			SD_LOG_DEBUG() << "SD Card Compliance 2.2-10 test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCompliance_2_2_10Test, SDCompliance_2_2_10Test);