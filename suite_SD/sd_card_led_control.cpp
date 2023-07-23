
#include "sdcard_test.hpp"
//#include "sd_wrapper.h"
//#include "SDHostRegisters.hpp"

class SDCardLEDControlTest : public SDCardTest
{
  private:

  public:
    SDCardLEDControlTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardLEDControlTest()
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
		uint32_t present_state = 0, previous_state = 0;
		
		//Read LED Control status
		present_state = 0;
		previous_state = 0;
		SET_SD_REGISTER_READ(&previous_value,HOST_HOST_CONTROL_1,0,0);
		if(!read_host_reg(&previous_value))
		{
			SD_LOG_DEBUG() << "read HOST_HOST_CONTROL_1 register failed" << std::endl;
			return ts::Test::Fail;
		}
		previous_state = previous_value.sd_reg.value;
		
		//LED ON
		if((previous_state & 0x1))
		{
			SD_LOG_DEBUG() << "LED is already ON" <<std::endl;
			return ts::Test::Skip;
		}
		else
		{
			previous_state |= 0x1;
			
			//Write LED ON status
			SET_SD_REGISTER_WRITE(&written_value,HOST_HOST_CONTROL_1,0,0,previous_state);
			if(!write_host_reg(&written_value))
			{
				SD_LOG_DEBUG() << "write HOST_HOST_CONTROL_1 register failed" << std::endl;
				return ts::Test::Fail;
			}
		}
		
		//Read LED Control status
		present_state = 0;
		while(!(present_state&0x1))
		{
			SET_SD_REGISTER_READ(&present_value,HOST_HOST_CONTROL_1,0,0);
			if(!read_host_reg(&present_value))
			{
				SD_LOG_DEBUG() << "read HOST_HOST_CONTROL_1 register failed" << std::endl;
				return ts::Test::Fail;
			}
			present_state = present_value.sd_reg.value;
			if(!(present_state & 0x1))
			{
				sd_logmsg("Failed to change the LED control bit: 0x%x\n", present_state);
				return ts::Test::Fail;
			}
		}
		sd_logmsg("Keeping LED ON for 10 seconds: 0x%x\n", present_state);
		env::SleepMs(10000);
		
		//LED OFF
		if(!(present_state & 0x1))
		{
			SD_LOG_DEBUG() << "LED is already OFF" <<std::endl;
			return ts::Test::Skip;
		}
		present_state = (present_state & 0xFE);
		//Write LED OFF status
		SET_SD_REGISTER_WRITE(&written_value,HOST_HOST_CONTROL_1,0,0,present_state);
		if(!write_host_reg(&written_value))
		{
			SD_LOG_DEBUG() << "write HOST_HOST_CONTROL_1 register failed" << std::endl;
			return ts::Test::Fail;
		}
		//env::SleepMs(200);
		
		//Read LED Control status
		SET_SD_REGISTER_READ(&present_value,HOST_HOST_CONTROL_1,0,0);
		if(!read_host_reg(&present_value))
		{
			SD_LOG_DEBUG() << "read HOST_HOST_CONTROL_1 register failed" << std::endl;
			return ts::Test::Fail;
		}
		present_state = present_value.sd_reg.value;
		if((present_state & 0x1))
		{
			sd_logmsg("Failed to change the LED control bit: 0x%x\n", present_state);
			return ts::Test::Fail;
		}
		sd_logmsg("Keep LED OFF for 10 seconds: 0x%x\n", present_state);
		env::SleepMs(10000);
		
		SD_LOG_DEBUG() << "LED Control Successful" <<std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardLEDControlTest, SDCardLEDControlTest);
