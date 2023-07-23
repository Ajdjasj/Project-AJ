

#include "sdcard_test.hpp"
#include "host_types.h"
#include "sd_wrapper.h"

class AlinkClockGating : public SDCardTest
{
  private:

  public:
	uint32_t misc_reg_base;
	uint32_t save_register;
	bool restore;
    AlinkClockGating(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~AlinkClockGating()
    {
    }

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun()
	{
		    SDCardTest::PreRun();
			restore = false;
			save_restore(true);
	}

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun()
	{
		    SDCardTest::PostRun();
			save_restore(false);
	}
	
	void enable_clock_gating(bool state)
	{
		reg32 temp_value;
		//env::System device1; //This is already defined as a pointer in sdcard_test constructor  
		env::Map* memptr1;
		
		memptr1 = device1->pmap((void*)(misc_reg_base + 0x2C),1);
		uint32_t *misc_reg0x2C = (uint32_t*)memptr1->getAddress();
		
		temp_value.reg_val = *misc_reg0x2C;
		temp_value.bit.b16 = state;
		*misc_reg0x2C      = temp_value.reg_val;
		restore = true;
	}

	void save_restore(bool what)
	{
//		env::System device1; //This is already defined as a pointer in sdcard_test constructor
		env::Map* memptr1;
		memptr1 = device1->pmap((void*)(misc_reg_base + 0x2C),1);
		uint32_t *misc_reg0x2C = (uint32_t*)memptr1->getAddress();
	
		if(true == what) //save register
		{
			save_register = *misc_reg0x2C;
		}
		else //restore register
		{
			if(restore == true)
			*misc_reg0x2C = save_register;
		}
	}
	
    virtual ts::Test::Result Main()
    {
		uint32_t Maxdevices = get_total_pcidevice(), Maxslots = 0;
		uint32_t device = 0, slot = 0 , error = 0x00;
		//_init_status init_status;
		//_function function;
		uint16_t speed_mode = HIGH_SPEED;
		uint32_t SD_control_register_save = 0x00;
		//_r1 status;
		uint32_t card_address = 0x5000, blk_size = 512, blk_count = 100;
		uint8_t autocmd = 0x01;
		uint8_t drive_name = Parameter<uint8_t>("drive",0, coreutil::FmtFlags(std::ios::hex));
		uint8_t *source = (uint8_t*)"log.xml";
		uint8_t destination[5] = "C:\\";
		uint8_t *dest = &destination[0];
		destination[0] = drive_name;
		char command[20];
		sprintf(command,"xcopy %s %s /y",source,dest);
		misc_reg_base = ((port_read(0x24)) & 0xFFFFE000) + 0xE00;
		
		sd_logmsg("PM_Register Base : %X\n",misc_reg_base);
		
		for(device = 0; device < Maxdevices; device++)
		{
			Maxslots = get_total_slot(device);
			for(slot = 0; slot < Maxslots; slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				//Detect Card
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
			

				enable_clock_gating(true);				
				/********************* Clock Gating Enable and Data Transfer ****************************/
				
				uint32_t test_count = 0x00;
				for(test_count = 0 ; test_count < 3 ; test_count++)
				{
					env::SleepMs(500);
					//SDCardTest::enable_device(device, true);
					env::SleepMs(500);
					if(system(command))
					{
						SD_LOG_DEBUG() << "Data Transfer failed When ALink Clock Gating Enabled"<< std::endl;
						return ts::Test::Fail;
					}
					env::SleepMs(5000);
				}
				
				enable_clock_gating(false);	

				for(test_count = 0 ; test_count < 3 ; test_count++)
				{
					env::SleepMs(500);
					//SDCardTest::enable_device(device, true);
					env::SleepMs(500);
					if(system(command))
					{
						SD_LOG_DEBUG() << "Data Transfer failed When ALink Clock Gating Enabled"<< std::endl;
						return ts::Test::Fail;
					}
					env::SleepMs(5000);
				}
				
				SD_LOG_DEBUG() << "Alink Clock Gating Test Passed"<< std::endl;
				//SDCardTest::enable_device(device, false);
				
				env::SleepMs(100);
			}
		}
		return ts::Test::Pass;	
	}
};

TServerTestInstance(AlinkClockGating, AlinkClockGating);
