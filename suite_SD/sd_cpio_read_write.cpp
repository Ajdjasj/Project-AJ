
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCardSdscPioReadWriteTest : public SDCardTest
{
  private:

  public:
    SDCardSdscPioReadWriteTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardSdscPioReadWriteTest()
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
		bool Speed = false;
		bool BusWidth[2] = {true, false};
		uint8_t autocmd = 0x0, Write_buffer[2048] = {0}, Read_buffer[2048] = {0};
		uint32_t card_address = 0x8000, blk_size_read = 0,blk_size_write = 0,remaining_blk_size_read = 0, remaining_blk_size_write = 0, blk_count = 0, Data = 0xdeadbeaf;
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail; 
				}
				printf("Card detected....\n");
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "Initialization Failed...." << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Initialization Complete...." << std::endl;
				
				if(init_status.card.sdhc_sdxc)
				{
					SD_LOG_DEBUG() << "Please insert the SDSC card for this test" << std::endl;
					return ts::Test::Fail;
				}
				
				//Get the Partial Read and Write option
				_r2 csd;
				uint32_t error = 0x00;
				if(get_csd(device, slot, &csd, &error))
				{
					uint8_t ret;
					if(csd.write_bl_partial())
						PartialWrite = true;
					
					if(csd.read_bl_partial())
						PartialRead = true;
				}
				
				//Partial block Read/Write test
				if(PartialWrite || PartialRead)
				{
						//Write the data
						if(PartialWrite)
						{
							blk_size_write = 400;
							remaining_blk_size_write = 112;
						}
						else
						{
							blk_size_write = 512;
							remaining_blk_size_write = 0;
						}
						
						//Reading the data
						if(PartialRead)
						{
							blk_size_read = 400;
							remaining_blk_size_read = 112;
						}
						else
						{
							blk_size_read = 512;
							remaining_blk_size_read = 0;
						}
							
						for( int j=0; j<blk_size; j++)
						{
							Write_buffer[j] = j+1;
						}
						if(!pio_write(device, slot, Speed, false, autocmd, card_address, Write_buffer, blk_size_write, blk_count))
						{
							SD_LOG_DEBUG() << "PIO Write Failed" << std::endl;
							return ts::Test::Fail; 
						}
						if(remaining_blk_size_write)
						{
							if(!pio_write(device, slot, Speed, false, autocmd, (card_address + blk_size_write), Write_buffer, remaining_blk_size_write, blk_count))
							{
								SD_LOG_DEBUG() << "PIO write Failed" << std::endl;
								return ts::Test::Fail; 
							}
						}

							
						if(!pio_read(device, slot, Speed, BusWidth[i], autocmd, card_address, Read_buffer, blk_size_read, blk_count))
						{
							SD_LOG_DEBUG() << "PIO Read Failed" << std::endl;
							return ts::Test::Fail; 
						}
						
						for(int j=0; j < blk_size_read; j++)
						{
							if(Write_buffer[j] != Read_buffer[j])
							{
								SD_LOG_DEBUG() << "SDSC PIO Partial block Read/Write test Failed for data: " << j << std::endl;
								return ts::Test::Fail; 
							}
							
						}
						
						if(remaining_blk_size_read)
						{
							if(!pio_read(device, slot, Speed, BusWidth[i], autocmd, (card_address + blk_size), Read_buffer, remaining_blk_size_read, blk_count))
							{
								printf("SDSC Failed to partial read....\n");
								return ts::Test::Fail; 
							}
							printf("SDSC PIO Partial block read complete for %d....\n", i+1);
							for(int j=0; j < remaining_blk_size_read; j++)
							{
								if(Write_buffer[blk_size + j] != Read_buffer[j])
								{
									SD_LOG_DEBUG() << "SDSC PIO Partial block Read/Write test Failed for data: " << j << std::endl;
									return ts::Test::Fail; 
								}
								
							}
						}
					}
				}
				else
				{
					printf("SDCard does not support partial Read/Write for device %d and slot %d....\n", device, slot);
					SD_LOG_DEBUG() << "SDCard does not support partial Read/Write" << std::endl;
				}
				SD_LOG_DEBUG() << "SDSC PIO Partial block Read/Write test Passed" << std::endl;
				
				
				//Single block Read/Write test;
				blk_size = 512;
				blk_count = 0;
				Data = 0xdeadbeaf;
				
				for(int i=0; i<2; i++)
				{
					if(!pio_data_transfer(device, slot, Speed, BusWidth[i], autocmd, card_address, Data, blk_size, blk_count))
					{
						printf("SDHC/SDXC Read/Write Failed for Multiple block....\n");
						return ts::Test::Fail; 
					}
				}
				SD_LOG_DEBUG() << "SDSC PIO Single block Read/Write test Passed" << std::endl;
				
				
				//Multiple block Read/Write test
				autocmd = 0x1;
				blk_size = 512;
				blk_count = 4;
				
				for(int i=0; i<2; i++)
				{
					if(!pio_data_transfer(device, slot, Speed, BusWidth[i], autocmd, card_address, Data, blk_size, blk_count))
					{
						printf("SDHC/SDXC Read/Write Failed for Multiple block....\n");
						return ts::Test::Fail; 
					}
				}
				SD_LOG_DEBUG() << "SDSC PIO Multiple block Read/Write test Passed" << std::endl;
			}
		}
		
		SD_LOG_DEBUG() << "SDSC PIO Read/Write test Successful" << std::endl;
		return TestResult; 
    }
};

TServerTestInstance(SDCardSdscPioReadWriteTest, SDCardSdscPioReadWriteTest);
