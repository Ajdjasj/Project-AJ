
#include "sdcard_test.hpp"


class SDForceEventTest : public SDCardTest
{
  private:

  public:
    SDForceEventTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDForceEventTest()
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
				sd_logmsg("Card detected....\n");
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					sd_logmsg("Host Initialization Failed....\n");
					return ts::Test::Fail;
				}
				sd_logmsg("Host Initialization Complete....\n");
				
				//Force Event
				for( int k=0; k<=9; k++ )
				{
					uint32_t int_status = 0, force = 0, Get_status = 0;
					
					//Enable Error Interrupt Status
					int_status = ((1<<k) << 16);
					sd_logmsg("Enable Error Interrupt Status: 0x%x\n", int_status);
					if(!int_status_enable(device, slot, int_status))
					{
						sd_logmsg("Failed for %d\n", k);
						return ts::Test::Fail;
					}
					
					//Enable Force Event Error Interrupt status
					force = int_status;
					sd_logmsg("Enable Force Event Error Interrupt Status: 0x%x\n", force);
					if(!force_event(device, slot, force))
					{
						sd_logmsg("Failed for %d\n", k);
						return ts::Test::Fail;
					}
					
					//Get the Error Interrupt Status bit
					Get_status = get_int_status(device, slot);
					sd_logmsg("Get Error Interrupt Status bit: 0x%x\n", Get_status);
					if(0x8000 & Get_status)
					{
						if(force != (((1<<k) << 16) & Get_status))
						{
							sd_logmsg("Failed for %d\n", k);
							return ts::Test::Fail;
						}
					}
					else
					{
						sd_logmsg("Failed to update Error Interrupt Status bit: %d\n", k);
						return ts::Test::Fail;
					}
					sd_logmsg("\n");
				}
				
					
				//Enable Auto CMD Error Status bit
				uint32_t int_status = 0, force = 0, Get_status = 0;
				int_status = ((1<<8) << 16);
				sd_logmsg("\nEnable Auto CMD Error Status bit: 0x%x\n", int_status);
				if(!int_status_enable(device, slot, int_status))
				{
					sd_logmsg("Failed to Enable Auto CMD Error Status");
					return ts::Test::Fail;
				}
				
				for( int k=0; k<=6; k++ )
				{
					if((k!=5) && (k!=6))
					{
						//Enable Force Event Auto CMD Error status
						force = (1<<k);
						sd_logmsg("Enable Force Event Auto CMD Error Status: 0x%x\n", force);
						if(!force_event(device, slot, force))
						{
							sd_logmsg("Failed for %d\n", k);
							return ts::Test::Fail;
						}
						
						//Get the Auto CMD Error Status bit
						Get_status = get_int_status(device, slot);
						sd_logmsg("Get the Auto CMD Error Status bit: 0x%x\n", Get_status);
						if((((1<<8) << 16) & Get_status) && (0x8000 & Get_status))
						{
							//Get the Auto CMD Error Status
							Get_status = (uint16_t)get_autocmd_error(device, slot);
							sd_logmsg("Get Force Event Auto CMD Error Status: 0x%x\n", Get_status);
							if(force != ((1<<k) & Get_status))
							{
								sd_logmsg("Failed for %d\n", k);
								return ts::Test::Fail;
							}
						}
						else
						{
							sd_logmsg("Failed to update Auto CMD Error Status bit: %d\n", k);
							return ts::Test::Fail;
						}
						sd_logmsg("\n");
					}
				}
			}
		}
		
		SD_LOG_DEBUG() << "SDCard Force Event Test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDForceEventTest, SDForceEventTest);
