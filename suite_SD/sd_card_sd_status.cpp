
#include "sdcard_test.hpp"


class SDCardSDStatusTest : public SDCardTest
{
  private:

  public:
    SDCardSDStatusTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardSDStatusTest()
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
					TestResult = ts::Test::Fail;
					SD_LOG_DEBUG() << "SDCard Detection Failed for device" << device << " and slot" << slot << std::endl;
					break;
				}
				sd_logmsg("Card detected....\n");
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					TestResult = ts::Test::Fail;
					break;
				}
				sd_logmsg("Host Initialization Complete....\n");
				
				//Card status
				_r1 card_status;
				uint32_t error = 0x00;
				if(!get_card_status( device, slot, &card_status, &error ))
				{
					sd_logmsg("\n---- Failed to get Card Status ----\n\n");
					TestResult = ts::Test::Fail;
					break;
				}
				sd_logmsg("\n---- Card Status: 0x%x ----\n", card_status);
				if(card_status.bit.current_state != stby)
				{
					sd_logmsg("---- Card is not in standby state ----\n");
					TestResult = ts::Test::Fail;
					break;
				}

				sd_logmsg("\n---- Card Status ----\n");
				if(card_status.bit.ake_seq_error)
					sd_logmsg("Error in the sequence of the authentication process\n");
				else
					sd_logmsg("No Error in the sequence of the authentication process\n");
				
				if(card_status.bit.app_cmd)
					sd_logmsg("App CMD Enabled\n");
				else
					sd_logmsg("App CMD Disabled\n");
				
				sd_logmsg("Ready for Data: %d\n", card_status.bit.rdy_for_data);
				if(card_status.bit.rdy_for_data)
					sd_logmsg("Ready for Data\n");
				else
					sd_logmsg("Not Ready for Data\n");
				
				sd_logmsg("Current state: ");
				switch(card_status.bit.current_state)
				{
					case 0:
							sd_logmsg("idle\n");
							break;
					case 1:
							sd_logmsg("ready\n");
							break;
					case 2:
							sd_logmsg("ident\n");
							break;
					case 3:
							sd_logmsg("stby\n");
							break;
					case 4:
							sd_logmsg("tran\n");
							break;
					case 5:
							sd_logmsg("data\n");
							break;
					case 6:
							sd_logmsg("rcv\n");
							break;
					case 7:
							sd_logmsg("prg\n");
							break;
					case 8:
							sd_logmsg("dis\n");
							break;
					case 15:
							sd_logmsg("Reserved for IO mode\n");
							break;
					case 9:
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
							sd_logmsg("Card is an Unsupported state\n");
				}
				
				sd_logmsg("Erase Reset: %d\n", card_status.bit.erase_reset);
				
				if(card_status.bit.card_ecc_disable)
					sd_logmsg("Card ECC Disabled\n");
				else
					sd_logmsg("Card ECC Enabled\n");
				
				if(card_status.bit.wp_erase_skip)
					sd_logmsg("Write Protected Erase skip\n");
				else
					sd_logmsg("Erase Write Protected\n");
				
				if(card_status.bit.csd_overwrite)
					sd_logmsg("CSD Overwrite error\n");
				else
					sd_logmsg("No CSD Overwrite error\n");
				
				if(card_status.bit.error)
					sd_logmsg("A general or an unknown error\n");
				else
					sd_logmsg("No general or an unknown error\n");
				
				if(card_status.bit.cc_error)
					sd_logmsg("Internal card controller error\n");
				else
					sd_logmsg("No Internal card controller error\n");
				
				if(card_status.bit.card_ecc_failed)
					sd_logmsg("Card ECC Failed\n");
				else
					sd_logmsg("Card ECC Success\n");
				
				if(card_status.bit.illegal_command)
					sd_logmsg("Illegal Command Error\n");
				else
					sd_logmsg("No Illegal Command Error\n");
				
				if(card_status.bit.com_crc_error)
					sd_logmsg("Command CRC Error\n");
				else
					sd_logmsg("No Command CRC Error\n");
				
				if(card_status.bit.lock_unlock_fail)
				{
					TestResult = ts::Test::Fail;
					sd_logmsg("Card Lock/Unclock Failure\n");
				}
				else
					sd_logmsg("No Card Lock/Unclock Failure\n");
				
				if(card_status.bit.card_is_locked)
				{
					TestResult = ts::Test::Fail;
					sd_logmsg("Card is Locked\n");
				}
				else
					sd_logmsg("Card is Unlocked\n");
				
				if(card_status.bit.wp_violation)
					sd_logmsg("Write Protected area violation by host\n");
				else
					sd_logmsg("No Write Protected area violation by host\n");
				
				if(card_status.bit.erase_param)
					sd_logmsg("Erase Param Error\n");
				else
					sd_logmsg("No Erase Param Error\n");
				
				if(card_status.bit.erase_seq_error)
					sd_logmsg("Erase Sequence Error\n");
				else
					sd_logmsg("No Erase Sequence Error\n");
				
				if(card_status.bit.block_len_error)
					sd_logmsg("Block Length Error\n");
				else
					sd_logmsg("No Block Length Error\n");
				
				if(card_status.bit.address_error)
					sd_logmsg("Misalligned Address Error\n");
				else
					sd_logmsg("No Misalligned Address Error\n");
				
				if(card_status.bit.out_of_range)
					sd_logmsg("Out of Range Error\n");
				else
					sd_logmsg("No Out of Range Error\n");
				sd_logmsg("---- End of Card Status ----\n");
				
				
				//SD status
				bool speed = true, width = true;
				_r1 status;
				_sd_status sd_status;
				if(!get_sd_status( device, slot, speed , width, &status, &sd_status))
				{
					sd_logmsg("\n---- Failed to get SD Status ----\n\n");
					TestResult = ts::Test::Fail;
					break;
				}
				
				sd_logmsg("\n---- SD Status ----\n");
				sd_logmsg("Erase Offset: 0x%x\n", sd_status.erase_offset);
				sd_logmsg("Erase Timeout: 0x%x\n", sd_status.erase_timeout);
				sd_logmsg("Erase Size: 0x%x\n", sd_status.erase_size);
				sd_logmsg("AU Size: 0x%x\n", sd_status.au_size);
				sd_logmsg("Performance Move: 0x%x\n", sd_status.performance_move);
				sd_logmsg("Speed Class: 0x%x\n", sd_status.speed_class);
				sd_logmsg("Size of Protected Area: 0x%x\n", sd_status.size_of_protected_area);
				if(0 == sd_status.sd_card_type)
					sd_logmsg("SD Card Type: Regular SD RD/WR card\n");
				else
				{
					sd_logmsg("SD Card Type: Not a Regular SD RD/WR card\n");
					TestResult = ts::Test::Fail;
				}
				
				sd_logmsg("Security Functions: 0x%x\n", sd_status.security_funtions);
				
				if(sd_status.secured_mode)
					sd_logmsg("Secured Mode\n");
				else
					sd_logmsg("Not in Secured Mode\n");
				sd_logmsg("Data Bus Width: 0x%x\n", sd_status.data_bus_width);
				sd_logmsg("---- End of SD Status ----\n");
			}
			if(TestResult == ts::Test::Fail)
				break;
		}
		if(TestResult != ts::Test::Fail)
			SD_LOG_DEBUG() << "SD Card Status and SD Status Test Successful" << std::endl;
			
        return TestResult; 
    }
	
};

TServerTestInstance(SDCardSDStatusTest, SDCardSDStatusTest);
