
#include "sdcard_test.hpp"
#include "host_types.h"

bool pio_with_blockgap_n_abort(uint32_t device , uint32_t slot ,bool speed, bool width, uint8_t auto_cmd ,uint32_t card_address,uint32_t data, uint32_t block_size, uint32_t block_count) 
{ 
	uint32_t size = (block_size * (block_count == 0 ? 1 : block_count)), index = 0x00, data_index = 0x00; 
	uint8_t *write_buffer, *read_buffer; 
	write_buffer    = new uint8_t[size]; 
	read_buffer     = new uint8_t[size]; 
	memset(write_buffer, 0 , size); 
	memset(read_buffer , 0 , size); 
	uint8_t array[5] = {((uint8_t)(data & 0x000000FF)), ((uint8_t)((data & 0x0000FF00) >> 8)) , ((uint8_t)((data & 0x00FF0000) >> 16)) , ((uint8_t)((data & 0xFF000000) >> 24))}; 
	sd_logmsg("Index : %x\n",(uint16_t)size); 
	 
	for(index = 0 ; index < size ; index++) 
	{ 
		write_buffer[index] = array[data_index]; 
		if(++data_index == 4) 
		data_index = 0; 
	} 
	 
	//writing the data       
	if(!pio_write_with_block_gap( device, slot, speed, width, auto_cmd, card_address, write_buffer, block_size, block_count)) 
	{
		//sd_logmsg("Failed in pio_write\n");
		SD_LOG_DEBUG()<<"Failed in pio_write"<<endl;
		return false; 
	}
	//Reading the data 
	//if(!pio_read_with_block_gap( device, slot, speed, width, auto_cmd, card_address, read_buffer, block_size, block_count)) 
	if(!pio_read( device, slot, speed, width, auto_cmd, card_address, read_buffer, block_size, block_count)) 
	{
		//sd_logmsg("Failed in pio_read\n");
		SD_LOG_DEBUG()<<"Failed in pio_read"<<endl;
		return false; 
	}

	 for(index = 0 ; index < size ; index++) 
	 { 
		 if(write_buffer[index] != read_buffer[index]) 
			 return false; 
	 }    
	 return true; 
}


class SDAbortTransactionTest : public SDCardTest
{
  private:

  public:
    SDAbortTransactionTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDAbortTransactionTest()
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
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0;
		_function function;
		_r1 status;
		uint8_t autocmd = 0x01;
		uint32_t card_address = 0x2000, blk_size = 512, blk_count = 100;
		//_init_status init_status;
		
		uint16_t type 				= Parameter<uint16_t>("cardtype"	,0, coreutil::FmtFlags(std::ios::hex)); //If type == 1 it is normal mode, if == 2 it is UHS mode
		uint16_t speed_mode 		= Parameter<uint16_t>("mode"		,0, coreutil::FmtFlags(std::ios::hex)); //If 1= DS/SDR12 , 2=HS/SDR25 , 4=SDR50 , 8=SDR104
		uint16_t abort_type 		= Parameter<uint16_t>("aborttype"	,0, coreutil::FmtFlags(std::ios::hex)); //If 1=Asynchronus Abort, 2=Synchronus abort
		
		bool voltage_switch = false;
		if(type == 2)
			voltage_switch = true;
		SD_LOG_DEBUG()<< "------<<SDAbortTransactionTest>>--- -----" << std::endl;
		
		for(device = 0; device < Maxdevices; device++)
		{
			Maxslots = get_total_slot(device);
			for(slot = 0; slot < Maxslots; slot++)
			{
				
				SD_LOG_DEBUG()<< "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				
				//Detect Card
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed for SD device "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				if(!SDCardTest::Initalization(device, slot,voltage_switch))
				{
					SD_LOG_DEBUG() << "Initalization Failed for SD device"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed"<< std::endl;
				
				//Get Card Supported Mode
				if(!get_supported_functions(device , slot, false, false, &function, &status))
				{
					SD_LOG_DEBUG() << "get_supported_functions failed"<< std::endl;
					return ts::Test::Fail;
				}
				
				//Check Requested mode is supported by the card
				if(!(speed_mode & function.group1))
				{
					SD_LOG_DEBUG() << "Selected Speed Mode is not supported by the Card"<< std::endl;
					return ts::Test::Fail;			
				}
				
				//Change the speed mode
				if(1 == type)
				{
					if(!set_normal_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
					{
						SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "set Normal speed mode Completed"<< std::endl;
				}
				else if(2 == type)
				{
					if(init_status.result.voltage_switch == true)
					{
						_reg sd_register;
						uint32_t clk_multiplier = 0x00;
						
						SET_SD_REGISTER_READ(&sd_register,HOST_CAPABILITIES_1,device,slot);
						read_host_reg(&sd_register);
						clk_multiplier = ((sd_register.sd_reg.value & 0x00FF0000) >> 16);
						if(0x00 == clk_multiplier)
						{
							if(!set_UHS_mode( device,  slot,  speed_mode, DIVIDED_CLK_SELECTION_MODE))
							{
								SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
								return ts::Test::Fail;
							}						
						}
						else
						{
							if(!set_UHS_mode( device,  slot,  speed_mode, PROGRAMABLE_CLK_SELECTION_MODE))
							{
								SD_LOG_DEBUG() << "set speed mode failed"<< std::endl;
								return ts::Test::Fail;
							}
						}	
						SD_LOG_DEBUG() << "Set UHS speed mode Completed"<< std::endl;
					}
					else
					{
						SD_LOG_DEBUG() << "UHS mode Not possible in the current Configuration"<< std::endl;
						if(init_status.host.voltage_switch == false)
							SD_LOG_DEBUG() << "Host doesnot support Voltage switch to 1.8V for switching the mode to UHS"<<std::endl;
						if(init_status.card.voltage_switch == false)
							SD_LOG_DEBUG() << "Card doesnot support Voltage switch to 1.8V for switching the mode to UHS"<<std::endl;
						return ts::Test::Fail;	
					}
				}
				else
				{
					SD_LOG_DEBUG() << "Wrong Type selected by the user from build.xml , type should be 1 for Normal modes and 2 for UHS modes"<< std::endl;
					return ts::Test::Fail;
				}
				//Do PIO Data transfer
				if(2 == abort_type)
				{
					if(!pio_with_blockgap_n_abort( device ,  slot ,(speed_mode == 1 ? false : true), true , autocmd , card_address , 0x5A5A5A5A , blk_size , blk_count) )
					{
						SD_LOG_DEBUG() << "PIO Data transfer Failed with Block Gap And Abort"<< std::endl;
						return ts::Test::Fail;					
					}
					SD_LOG_DEBUG() << "PIO Data transfer Completed by using block Gap Int and Abort Command..."<< std::endl;
					//blk_count = 10;
					if(!SDCardTest::ADMA2ReadWrite_with_block_gap(card_address, 0xDEADBEEF, blk_size, 10000, true, true, 0x00) )
					{
						SD_LOG_DEBUG() << "Failed To get Block Gap"<< std::endl;
						return ts::Test::Fail;					
					}
					SD_LOG_DEBUG() << "DMA transfer Completed..."<< std::endl;
				}	
				else if(1 == abort_type)
				{
					blk_count = 100;
					if(!SDCardTest::ADMA2ReadWrite_with_asyncabort(card_address, 0xDEADBEEF, blk_size, 10000, true, true, 0x01) )
					{
						SD_LOG_DEBUG() << "Asynchronus Abort Failed during ADMA"<< std::endl;
						return ts::Test::Fail;					
					}
					SD_LOG_DEBUG() << "Asynchronus Abort Success"<< std::endl;
					
				}
			}
		}
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDAbortTransactionTest, SDAbortTransactionTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
