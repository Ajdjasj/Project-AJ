
#include "sdcard_test.hpp"


class SDCardInsertSimulationTest : public SDCardTest
{
  private:

  public:
    SDCardInsertSimulationTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardInsertSimulationTest()
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
		_reg previous_value, written_value, present_value;
		uint32_t present_state = 0, InsertRemoveValue = 0;
		uint32_t int_status = 0;
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				//Disable Interrupt(Interrupt Status/Signal Enable register)
				{
					int_status = 0x0;
					
					int_status_enable(device, slot, int_status);
					int_signal_enable(device, slot, int_status);
				}
			
				//
				present_state = 0;
				InsertRemoveValue = 0;
				uint8_t restore = 0x00;
				SET_SD_REGISTER_READ(&previous_value,HOST_HOST_CONTROL_1,0,0);
				if(!read_host_reg(&previous_value))
				{
					SD_LOG_DEBUG() << "read HOST_HOST_CONTROL_1 register failed" << std::endl;
					return ts::Test::Fail;
				}
				restore = (uint8_t)previous_value.sd_reg.value;
				InsertRemoveValue = previous_value.sd_reg.value;
				//sd_logmsg("Host Control 1: 0x%x\n", InsertRemoveValue); //Dont use this way, threat of vsnpritf being hit
				SD_LOG_DEBUG() << "Host Control 1:" << std::hex << InsertRemoveValue << std::endl;
				//
				//if(!(InsertRemoveValue & 0x80))
				{
					InsertRemoveValue |= (1<<CARD_DETECT_SIGNAL_SELECT_);
					//
					SET_SD_REGISTER_WRITE(&written_value,HOST_HOST_CONTROL_1,0,0,InsertRemoveValue);
					if(!write_host_reg(&written_value))
					{
						SD_LOG_DEBUG() << "write HOST_HOST_CONTROL_1 register failed" << std::endl;
						return ts::Test::Fail;
					}
					
					env::SleepMs(500);
					
					//Enable Card Insert and Removal Status
					int_status = (1<<CARD_INSTERT_STATUS_ENABLE_BIT)|(1<<CARD_REMOVAL_STATUS_ENABLE_BIT);
					sd_logmsg("Host HOST_INT_STATUS: 0x%x\n", int_status);
					int_status_enable(device, slot, int_status);
					
					
					//Insert
					sd_logmsg("\n----Insert----\n");
					InsertRemoveValue |= (1<<CARD_DETECT_TEST_LEVEL);
					//
					SET_SD_REGISTER_WRITE(&written_value,HOST_HOST_CONTROL_1,0,0,InsertRemoveValue);
					if(!write_host_reg(&written_value))
					{
						SD_LOG_DEBUG() << "write HOST_HOST_CONTROL_1 register failed" << std::endl;
						return ts::Test::Fail;
					}
					
					env::SleepMs(500);
					
					//
					int_status = get_int_status( device, slot);
					sd_logmsg("Host HOST_INT_STATUS: 0x%x\n", int_status);
					
					//
					SET_SD_REGISTER_READ(&present_value,HOST_PRESENT_STATE,0,0);
					if(!read_host_reg(&present_value))
					{
						SD_LOG_DEBUG() << "read HOST_PRESENT_STATE register failed" << std::endl;
						return ts::Test::Fail;
					}
					present_state = present_value.sd_reg.value;
					sd_logmsg("Host HOST_PRESENT_STATE: 0x%x\n", present_state);
					if(!(present_state&(1<<CARD_INSERT_BIT)))
					{
						if((int_status&(1<<CARD_INSTERT_STATUS_BIT)))
						{
							sd_logmsg("Card Insert Simulation Failed\n");
							return ts::Test::Fail;
						}
					}
					sd_logmsg("Card Insert Simulation Completed for device %d and slot %d\n", device, slot);
					
					
					//Removal
					sd_logmsg("\n----Remove----\n");
					SET_SD_REGISTER_READ(&previous_value,HOST_HOST_CONTROL_1,0,0);
					if(!read_host_reg(&previous_value))
					{
						SD_LOG_DEBUG() << "read HOST_HOST_CONTROL_1 register failed" << std::endl;
						return ts::Test::Fail;
					}
					InsertRemoveValue = previous_value.sd_reg.value;
					
					//
					InsertRemoveValue ^= (1<<CARD_DETECT_TEST_LEVEL);
					SET_SD_REGISTER_WRITE(&written_value,HOST_HOST_CONTROL_1,0,0,InsertRemoveValue);
					if(!write_host_reg(&written_value))
					{
						SD_LOG_DEBUG() << "write HOST_HOST_CONTROL_1 register failed" << std::endl;
						return ts::Test::Fail;
					}
					
					env::SleepMs(500);
					
					//
					int_status = get_int_status(device, slot);
					sd_logmsg("Host HOST_INT_STATUS: 0x%x\n", int_status);
					
					
					//
					SET_SD_REGISTER_READ(&present_value,HOST_PRESENT_STATE,0,0);
					if(!read_host_reg(&present_value))
					{
						SD_LOG_DEBUG() << "read HOST_PRESENT_STATE register failed" << std::endl;
						return ts::Test::Fail;
					}
					present_state = present_value.sd_reg.value;
					sd_logmsg("Host HOST_PRESENT_STATE: 0x%x\n", present_state);
					if((present_state&(1<<CARD_INSERT_BIT)))
					{
						if((int_status&(1<<CARD_INSTERT_STATUS_BIT)))
						{
							sd_logmsg("Card Remove Simulation Failed\n");
							return ts::Test::Fail;
						}
					}
					SET_SD_REGISTER_WRITE(&written_value,HOST_HOST_CONTROL_1,0,0,restore);
					write_host_reg(&written_value);

					sd_logmsg("Card Removal Simulation Completed for device %d and slot %d\n\n", device, slot);
				}
			}
		}
		
		SD_LOG_DEBUG() << "SDCard Insert/Removal Simulation Test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardInsertSimulationTest, SDCardInsertSimulationTest);
