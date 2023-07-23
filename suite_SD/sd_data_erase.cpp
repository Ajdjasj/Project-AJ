
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCardDataEraseTest : public SDCardTest
{
  private:

  public:
    SDCardDataEraseTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardDataEraseTest()
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
		    SDCardTest::PostRun();
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		uint8_t Devices = 0, Slots = 0;
		bool PartialRead = false, PartialWrite = false;
		bool Speed = true;
		bool BusWidth = true;
		uint8_t autocmd = 0x0, Write_buffer[512] = {0}, Read_buffer[512] = {0};
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 0;
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() <<"Card detected...." <<std::endl;
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "Initialization Failed" << std::endl;
					return ts::Test::Fail;
				}
					SD_LOG_DEBUG() << "Initialization Completed" << std::endl;
				
				
				//Single block Read/Write test
				//Write the data
				for( int j=0; j<blk_size; j++)
				{
					Write_buffer[j] = j+1;
				}
				if(!pio_write( device, slot, Speed, BusWidth, autocmd, card_address, Write_buffer, blk_size, blk_count))
				{
					//sd_logmsg("Failed to write the data....\n");
					SD_LOG_DEBUG() << "Failed to write the data..." << std::endl;
					return ts::Test::Fail;
				}
				//sd_logmsg("PIO single block write complete for device %d and slot %d....\n", device, slot);
				SD_LOG_DEBUG()<<"PIO single block write complete for device "<<std::dec<<device << "and slot"<< std::dec <<slot <<std::endl;
				
				//Reading the data
				if(!pio_read( device, slot, Speed, BusWidth, autocmd, card_address, Read_buffer, blk_size, blk_count))
				{
					//sd_logmsg("Failed to read the data....\n");
					SD_LOG_DEBUG() << "Failed to read the data..." << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG()<<"PIO single block read complete for device "<<std::dec<<device <<"and slot"<< std::dec<<slot <<std::endl;
				//sd_logmsg("PIO single block read complete for device %d and slot %d....\n", device, slot);
				for(int j=0; j < blk_size; j++)
				{
					if(Write_buffer[j] != Read_buffer[j])
					{
						SD_LOG_DEBUG() << "SDCard PIO Single block Read/Write test Failed for data: "<<std::dec << j << std::endl;
						return ts::Test::Fail;
					}
					
				}
				
				//Erase the Data
				_r1 status;
				_scr SCR;
				if(!get_scr(device, slot, &SCR, &status, Speed, BusWidth))
				{
					//sd_logmsg("Failed to get the SCR....\n");
					SD_LOG_DEBUG() << "Failed to get the SCR..." << std::endl;
					return ts::Test::Fail;
				}
				#if 1
				uint8_t EraseValue = 0xFF;
				if(!SCR.data_stst_after_erase())
					EraseValue = 0x0;
				SD_LOG_DEBUG() << "Erase values: 0x%x" << EraseValue << std::endl;
				#endif
				if(!erase_data(device, slot, card_address, (card_address + blk_size), &status))
				{
					//sd_logmsg("Failed to erase the data....\n");
					SD_LOG_DEBUG() << "Failed to erase the data..." << std::endl;
					return ts::Test::Fail;
				}
				//Reading the data
				if(!pio_read( device, slot, Speed, BusWidth, autocmd, card_address, Read_buffer, blk_size, blk_count))
				{
					//sd_logmsg("Failed to read the data after data erase....\n");
					SD_LOG_DEBUG() << "Failed to read the data after data erase ..." << std::endl;
					return ts::Test::Fail;
				}
				//sd_logmsg("PIO single block read complete for device %d and slot %d....\n", device, slot);
				SD_LOG_DEBUG()<<"PIO single block read complete for device "<<std::dec<<device << "and slot"<< std::dec <<slot <<std::endl;
				for(int j=0; j < blk_size; j++)
				{
					if(EraseValue != Read_buffer[j])
					{
						//sd_logmsg("\nSDCard data erase test Failed for data: 0X%X at offset %d\n", Read_buffer[j], j);
						SD_LOG_DEBUG() << "SDCard data erase test Failed for data: " << std::dec<<Read_buffer[j] << " at offset " << j << std::endl;
						return ts::Test::Fail;
					}
				}
				SD_LOG_DEBUG() << "SDCard PIO Single block Read/Write test Passed for device " << device << " and slot" << slot << std::endl;
			}
		}
		
		SD_LOG_DEBUG() << "SDCard Data erase test Successful" << std::endl;
		return TestResult; 
    }
};

TServerTestInstance(SDCardDataEraseTest, SDCardDataEraseTest);
