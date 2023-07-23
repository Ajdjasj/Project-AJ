
#include "sdcard_test.hpp"
//#include "sd_wrapper.h"
#include "sd_host_registers.hpp"

class SDHostRegistersTest : public SDCardTest
{
  private:

  public:
    SDHostRegistersTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDHostRegistersTest()
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
		_reg previous_value, written_value, present_value, host_ctrl2;

		uint32_t present_state = 0, previous_state = 0,host_ctrl2_value = 0x00;;
		sd_logmsg("\n-------------------------------------------------------------------------");
		sd_logmsg("\n------------------------  Host R/W Register Test  -----------------------\n");
		sd_logmsg("-------------------------------------------------------------------------\n");		
		uint16_t i=0;
		//for( uint16_t i=0; i<15; i++)
		while(rw_register[i].offset != HOST_END_OF_ARRAY)
		{
			present_state = 0;
			previous_state = 0;
			SET_SD_REGISTER_READ(&previous_value,rw_register[i].offset,0,0);
			if(!read_host_reg(&previous_value))
			{
				SD_LOG_DEBUG() << "read_host_reg failed"<< std::endl;
				return ts::Test::Fail;
			}
			
			sd_logmsg("Register Name           : %s\n", rw_register[i].name);
			sd_logmsg("Register Offset         : 0x%x\n", previous_value.sd_reg.offset);
			sd_logmsg("Register Width          : 0x%x\n", previous_value.sd_reg.width);
			sd_logmsg("First Read value        : 0x%x\n", previous_value.sd_reg.value);
			previous_state = GetRegFormat( previous_value.sd_reg.width, ~previous_value.sd_reg.value );
			previous_state = (previous_state ^ rw_register[i].mask);
			previous_state = (previous_state & (~rw_register[i].clear));
			sd_logmsg("Written value           : 0x%x\n", previous_state);

			if(HOST_CLOCK_CONTROL == rw_register[i].offset)
			{
				SET_SD_REGISTER_READ(&host_ctrl2,HOST_HOST_CONTROL_2,0,0);
				read_host_reg(&host_ctrl2);
				host_ctrl2_value = host_ctrl2.sd_reg.value;
				host_ctrl2.sd_reg.value = (host_ctrl2.sd_reg.value & 0x7FFF);
				write_host_reg(&host_ctrl2);
				host_ctrl2.sd_reg.value = host_ctrl2_value;
			}
			
			
			SET_SD_REGISTER_WRITE(&written_value,rw_register[i].offset,0,0,previous_state);
			if(!write_host_reg(&written_value))
			{
				SD_LOG_DEBUG() << "write_host_reg failed" << std::endl;
				
				if(HOST_CLOCK_CONTROL == rw_register[i].offset)
					write_host_reg(&host_ctrl2);

				return ts::Test::Fail;
			}

			env::SleepMs(100);
				
			SET_SD_REGISTER_READ(&present_value,rw_register[i].offset,0,0);
			if(!read_host_reg(&present_value))
			{
				SD_LOG_DEBUG() << "read_host_reg failed" << std::endl;
				if(HOST_CLOCK_CONTROL == rw_register[i].offset)
					write_host_reg(&host_ctrl2);
				return ts::Test::Fail;
			}
			present_state = present_value.sd_reg.value;
			sd_logmsg("Final Read value        : 0x%x\n", present_state);
			if(previous_state != present_state)
			{
				SD_LOG_DEBUG() << "Host Register Test failed for R/W register Offset : 0x"<<std::hex<< previous_value.sd_reg.offset<<", Register Name : "<<rw_register[i].name<<std::endl;
				write_host_reg(&previous_value);
				if(HOST_CLOCK_CONTROL == rw_register[i].offset)
					write_host_reg(&host_ctrl2);				
				return ts::Test::Fail;
			}
			if(HOST_CLOCK_CONTROL == rw_register[i].offset)
				write_host_reg(&host_ctrl2);
			write_host_reg(&previous_value);
			sd_logmsg("-----------------------------------\n");
			i++;
		}
		
		sd_logmsg("\n-------------------------------------------------------------------------");
		sd_logmsg("\n------------------------  Host RO Register Test  ------------------------\n");
		sd_logmsg("-------------------------------------------------------------------------\n");
		//for( uint16_t i=0; i<19; i++)
		i = 0;
		while(ro_register[i].offset != HOST_END_OF_ARRAY)
		{
			present_state = 0;
			previous_state = 0;
			SET_SD_REGISTER_READ(&previous_value,ro_register[i].offset,0,0);
			if(!read_host_reg(&previous_value))
			{
				SD_LOG_DEBUG() << "read_host_reg failed" << i << std::endl;
				return ts::Test::Fail;
			}
			sd_logmsg("Register Name           : %s\n", ro_register[i].name);
			sd_logmsg("Register Offset         : 0x%x\n", previous_value.sd_reg.offset);
			sd_logmsg("Register Width          : 0x%x\n", previous_value.sd_reg.width);
			sd_logmsg("First Read value        : 0x%x\n", previous_value.sd_reg.value);
			previous_state = GetRegFormat( previous_value.sd_reg.width, previous_value.sd_reg.value );
			sd_logmsg("Written value           : 0x%x\n", ~previous_state);
			
			SET_SD_REGISTER_WRITE(&written_value,ro_register[i].offset,0,0,~previous_state);
			if(!write_host_reg(&written_value))
			{
				SD_LOG_DEBUG() << "write_host_reg failed" << std::endl;
				return ts::Test::Fail;
			}
			
			env::SleepMs(100);
				
			SET_SD_REGISTER_READ(&present_value,ro_register[i].offset,0,0);
			if(!read_host_reg(&present_value))
			{
				SD_LOG_DEBUG() << "read_host_reg failed"<< std::endl;
				return ts::Test::Fail;
			}
			present_state = present_value.sd_reg.value;
			sd_logmsg("Final Read value        : 0x%x\n", present_value.sd_reg.value);

			if(previous_value.sd_reg.value != present_state)
			{
				SD_LOG_DEBUG() << "Register failed for RO register: 0x" <<std::hex<< previous_value.sd_reg.offset<<", Register Name : "<<ro_register[i].name<<std::endl;;
				return ts::Test::Fail;
			}
			sd_logmsg("-----------------------------------\n");
			i++;
		}
		
		SD_LOG_DEBUG() << "Host Registers Successful" <<std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDHostRegistersTest, SDHostRegistersTest);

//TServerTestInstance(SDHostRegisters, SDHostRegistersTest);
