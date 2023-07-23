
#include "sdcard_test.hpp"
#define MAX_FBR  0x07
#define FBR_SIZE 0x11

class SDIOInitTest : public SDCardTest
{
  private:

  public:
    SDIOInitTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDIOInitTest()
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
		uint8_t Maxdevice = 0, Maxslot = 0;
		uint16_t testtype = Parameter<uint16_t>("testid",0, coreutil::FmtFlags(std::ios::hex));
		//uint16_t speed_mode = Parameter<uint16_t>("mode",0, coreutil::FmtFlags(std::ios::hex));
		uint32_t register_index = 0, max_fbr_index = 0x00;
		uint32_t fbr_count = 0x01;
		uint8_t CCCR_read_buffer[25];
		_r5 io_read;
		io_read.reg_val = 0x00;
		uint32_t error = 0x00;
		_r1 status;
		
		Maxdevice = get_total_pcidevice();
		for(int device=0; device<Maxdevice; device++)
		{
			Maxslot = get_total_slot(device);
			for(int slot=0; slot<Maxslot; slot++)
			{
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SD card detect Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}

				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "SD Card Init test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				uint16_t sdio_rca = 0x00;
				get_rca(&sdio_rca, device,slot,&error);
				sd_logmsg("SDIO rca = 0x%X\n",sdio_rca);
				
				if(testtype == 1)//SDIO Init test
				{
					if(init_status.result.sdio_init != true)
						return ts::Test::Fail;
				}
				else if(testtype == 2) //Direct and Extended read operation
				{
					select_card(device , slot , &status , &error);
					if(!extended_io_byte_read(device, slot , 0/*Function Number*/, 0x00 /*Start Offset*/, true /* fixe_or_inc_address */, false/* block_mode*/, 23 ,&CCCR_read_buffer[0], &io_read,&error))
					{
						sd_logmsg("IO extended Read failed\n");
						return ts::Test::Fail;
					}

					for(register_index = 0 ; register_index < 23 ; register_index++)
					{				
						if(!direct_io_read(0x00,register_index, &io_read , &error))
						{						
							sd_logmsg("CCCR direct read failed\n");
							return ts::Test::Fail;
						}
						else if((io_read.bit.flag & 0x07) == 0x00)
						{
							if(io_read.bit.data != CCCR_read_buffer[register_index])
							{
								sd_logmsg("Direct and Extended read operation failed\n");
								sd_logmsg("Data red using Extended read - Offset 0x%X - Data : %X\n", register_index,CCCR_read_buffer[register_index]);
								sd_logmsg("Data red using Direct read   - Offset 0x%X - Data : %X\n", register_index,io_read.bit.data);
								return ts::Test::Fail;
							}
							else
							{
								sd_logmsg("Data red using Extended read - Offset 0x%X - Data : %X\n", register_index,CCCR_read_buffer[register_index]);
								sd_logmsg("Data red using Direct read   - Offset 0x%X - Data : %X\n", register_index,io_read.bit.data);							
							}
						}	
						else
						{
							sd_logmsg("CCCR direct read failed read response flag : %X \n",io_read.bit.flag);
							return ts::Test::Fail;							
						}
					}
				}
				else if(testtype == 3) //Direct and Extended Write operation
				{
					select_card(device , slot , &status , &error);
					if(!direct_io_write(true , 0x00 , 0x10, 0x5A , &io_read ,&error))
					{
						sd_logmsg("Direct IO Write Failed Offset 0x10\n");
						return ts::Test::Fail;
					}
					
					if(io_read.bit.data != 0x5A)
					{
						sd_logmsg("Direct IO RAW value failed Offset 0x10, RAW : 0x%X Written value : 0x5A\n",io_read.bit.data);
						return ts::Test::Fail;
					}
					
					if(!direct_io_read(0x00,0x10, &io_read , &error))
					{						
						sd_logmsg("Direct IO Read operation failed Offset 0x10\n");
						return ts::Test::Fail;
					}	
					
					if(io_read.bit.data != 0x5A)
					{
						sd_logmsg("Direct IO Write failed Offset 0x10, Red value : 0x%X Written value : 0x5A\n",io_read.bit.data);
						return ts::Test::Fail;
					}
					sd_logmsg("Direct IO Write Succesfull Offset 0x10, Red value : 0x%X Written value : 0x5A\n",io_read.bit.data);

					for(register_index = 0 ; register_index < 10 ; register_index++)
					{
						CCCR_read_buffer[register_index] = register_index;
					}
					if(!extended_io_byte_write(device, slot , 0/*Function Number*/, 0x10 /*Start Offset*/, false /* fixe_or_inc_address */, false/* block_mode*/, 10 ,&CCCR_read_buffer[0], &io_read,&error))
					{
						sd_logmsg("IO extended Write failed\n");
						return ts::Test::Fail;
					}
					
					if(!direct_io_read(0x00,0x10, &io_read , &error))
					{						
						sd_logmsg("Direct IO Read operation failed Offset 0x10\n");
						return ts::Test::Fail;
					}	
					
					if(io_read.bit.data != CCCR_read_buffer[register_index - 1])
					{
						sd_logmsg("Extended IO Write failed Offset 0x10, Red value : 0x%X Written value : 0x%X\n",io_read.bit.data,(register_index - 1));
						return ts::Test::Fail;
					}
					else
					{
						sd_logmsg("Last written value using Extended write - Offset 0x%X - Data : %X\n", 0x10,CCCR_read_buffer[register_index - 1]);
						sd_logmsg("Data red using Direct read              - Offset 0x%X - Data : %X\n", 0x10,io_read.bit.data);							
					}
	
				}	
				else if(testtype == 4) //Read CCCR and FBR 
				{
					select_card(device , slot , &status , &error);
					sd_logmsg("--------------Reading the CCCR------------------\n");
					for(register_index = 0x00 ; register_index <= 0x16 ; register_index++)
					{
						if(!direct_io_read(0x00,register_index, &io_read,&error))
						{						
							sd_logmsg("CCCR direct read failed\n");
							return ts::Test::Fail;
						}	
						else if((io_read.bit.flag & 0x07) == 0x00)
						{
							sd_logmsg("Offset 0x%X - Data : %X\n",register_index,io_read.bit.data);					
						}	
						else
						{
							sd_logmsg("CCCR direct read failed read response flag : %X \n",io_read.bit.flag);
							return ts::Test::Fail;							
						}
					}
					
					while(fbr_count <= MAX_FBR)
					{
						register_index = fbr_count * 0x100;
						max_fbr_index  = FBR_SIZE + register_index;
						sd_logmsg("--------------Reading the FBR %d------------------\n",fbr_count);
						for( ; register_index <= max_fbr_index ; register_index++)
						{
							if(!direct_io_read(0x00,register_index, &io_read,&error))
							{						
								sd_logmsg("FBR%d direct read failed\n",fbr_count);
								return ts::Test::Fail;
							}	
							else
							{
								sd_logmsg("Offset 0x%X - Data : %X\n",register_index,io_read.bit.data);					
							}	
						}
						fbr_count++;
					}	
				}	
				else
				{
				}
			}	
		}
		return ts::Test::Pass;
    }
	
};

TServerTestInstance(SDIOInitTest, SDIOInitTest);

