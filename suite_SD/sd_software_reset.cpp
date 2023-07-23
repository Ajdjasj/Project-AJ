
#include "sdcard_test.hpp"
//#include "sd_wrapper.h"
//#include "SDHostRegisters.hpp"

class SDSoftwareResetTest : public SDCardTest
{
  private:

  public:
    SDSoftwareResetTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDSoftwareResetTest()
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

	uint32_t GetRegFormat(uint8_t width, uint32_t RegValueReferance )
	{
		uint32_t ret = 0;
		
		if(width== 1)
		{
			ret = (uint8_t)(RegValueReferance & 0xFF);
			sd_logmsg("Ret: 0x%x\n", ret);
		}
		else if(width == 2)
		{
			ret = (uint16_t)(RegValueReferance & 0xFFFF);
			sd_logmsg("Ret: 0x%x\n", ret);
		}
		else if(width == 4)
		{
			ret = RegValueReferance & 0xFFFFFFFF;
			sd_logmsg("Ret: 0x%x\n", ret);
		}
		else
		{
			/*Do nothing To meet the coding standerd */
		}
		
		return ret;
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		uint8_t Devices = 0, Slots = 0;
		//_reg previous_value, written_value, present_value;
		uint32_t present_state = 0, previous_state = 0;
		_reg ReadData, WriteData;
		uint8_t ResetType[3] = {SW_ALL_LINE_RESET, SW_CMD_LINE_RESET, SW_DAT_LINE_RESET};
		uint16_t PresentValue[3] = {0x0, 0x1, 0xF06}, NormalInterruptStatusValue[3] = {0x0, 0x1, 0x3E};
		
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				
				SD_LOG_DEBUG() << "------------------- Device" << device << " , Slot" << slot << "-------------------"<< std::endl;
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed"<< std::endl;
					return ts::Test::Fail;
				}
				sd_logmsg("Card detected....\n");
				
				//Initialization
				if(!init_host(device, slot))
				{
					SD_LOG_DEBUG() << "SD host Init test Failed"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() <<"Initialization Complete"<< std::endl;;
				
				for(int k=2; k>= 0; k--)
				{	
					//sd_logmsg("Press Enter key\n");
					//getchar();
					
					#if 0
					
					if(!sw_host_reset(ResetType[k], device, slot))
					{
						SD_LOG_DEBUG() << "SDCard software reset for DAT line" << std::endl;
						return ts::Test::Fail;
					}
				
					//sd_logmsg("Press Enter key\n");
					//getchar();
					
					#endif
					
					//Software reset
					//Write Data into Bost Block Gap Control register
					previous_state = 0xF;
					SET_SD_REGISTER_WRITE(&WriteData,HOST_BLOCK_GAP_CONTROL,0,0,(uint8_t)previous_state);
					if(!write_host_reg(&WriteData))
						SD_LOG_DEBUG() << "write_host_reg failed" << std::endl;
					env::SleepMs(100);
					SET_SD_REGISTER_READ(&ReadData,HOST_BLOCK_GAP_CONTROL,0,0);
					if(!read_host_reg(&ReadData))
						SD_LOG_DEBUG() << "read_host_reg failed" << std::endl;
					present_state = ReadData.sd_reg.value;
					sd_logmsg("Read Width: 0x%x\n", ReadData.sd_reg.width);
					sd_logmsg("Register Offset			: 0x%x\n", ReadData.sd_reg.offset);
					sd_logmsg("Second register value	: 0x%x\n", ReadData.sd_reg.value);
					
					if(!sw_host_reset(ResetType[k], device, slot))
					{
						SD_LOG_DEBUG() << "S/W Reset Tes Failed" << std::endl;
						return ts::Test::Fail;
					}
				#if 0
					if(SW_DAT_LINE_RESET == ResetType[k]) //Only in SW reset for DAT line
					{
						SET_SD_REGISTER_READ(&ReadData,HOST_BUFFER_0,0,0);
						if(!read_host_reg(&ReadData))
						{
							SD_LOG_DEBUG() << "read_host_reg failed for HOST_BUFFER_0" << std::endl;
							return ts::Test::Fail;
						}
						if( 0 != ReadData.sd_reg.value )
						{
							sd_logmsg("HOST_BUFFER_0 is not reset: 0x%x\n",ReadData.sd_reg.value);
							return ts::Test::Fail;
						}	
						sd_logmsg("HOST_BUFFER_0 is not reset: 0x%x\n",ReadData.sd_reg.value);
						SET_SD_REGISTER_READ(&ReadData,HOST_BUFFER_1,0,0);
						if(!read_host_reg(&ReadData))
						{
							SD_LOG_DEBUG() << "read_host_reg failed for HOST_BUFFER_1" << std::endl;
							return ts::Test::Fail;
						}
						if( 0 != ReadData.sd_reg.value )
						{
							sd_logmsg("HOST_BUFFER_1 is not reset: 0x%x\n",ReadData.sd_reg.value);
							return ts::Test::Fail;
						}
						sd_logmsg("HOST_BUFFER_1 is not reset: 0x%x\n",ReadData.sd_reg.value);
					}
				#endif	
					SET_SD_REGISTER_READ(&ReadData,HOST_PRESENT_STATE,0,0);
					if(!read_host_reg(&ReadData))
					{
						SD_LOG_DEBUG() << "read_host_reg failed for HOST_PRESENT_STATE" << std::endl;
						return ts::Test::Fail;
					}
					if( 0 != (ReadData.sd_reg.value & PresentValue[k]) )
					{
						sd_logmsg("HOST_PRESENT_STATE is not reset: 0x%x\n",ReadData.sd_reg.value);
						return ts::Test::Fail;
					}
					
					if(SW_DAT_LINE_RESET == ResetType[k]) //Only for SW reset fo DAT line
					{
						SET_SD_REGISTER_READ(&ReadData,HOST_BLOCK_GAP_CONTROL,0,0);
						if(!read_host_reg(&ReadData))
						{
							SD_LOG_DEBUG() << "read_host_reg failed for HOST_BLOCK_GAP_CONTROL" << std::endl;
							return ts::Test::Fail;
						}
						if( 0 != (ReadData.sd_reg.value & 0x3) )
						{
							sd_logmsg("HOST_BLOCK_GAP_CONTROL is not reset: 0x%x\n",ReadData.sd_reg.value);
							return ts::Test::Fail;
						}
					}
					
					//Will verify this compare later
					#if 0
					SET_SD_REGISTER_READ(&ReadData,HOST_INT_ENABLE,0,0);
					if(!read_host_reg(&ReadData))
					{
						SD_LOG_DEBUG() << "read_host_reg failed for HOST_INT_ENABLE" << std::endl;
						return ts::Test::Fail;
					}
					if( 0 != (ReadData.sd_reg.value & NormalInterruptStatusValue[k]) )
					{
						sd_logmsg("HOST_INT_ENABLE is not reset: 0x%x\n",ReadData.sd_reg.value);
						return ts::Test::Fail;
					}
					#endif
					switch(k)
					{
						case 0:
							sd_logmsg("SW_ALL_LINE_RESET Reset completed\n");
						break;

						case 1:
							sd_logmsg("SW_CMD_LINE_RESET Reset completed\n");
						break;

						case 2:
							sd_logmsg("SW_DAT_LINE_RESET Reset completed\n");
						break;
					}
					
				}
			}
		}
		
		SD_LOG_DEBUG() << "SD card Software reset test Successful" << std::endl;
        return TestResult; 
    }
	
};

TServerTestInstance(SDSoftwareResetTest, SDSoftwareResetTest);
