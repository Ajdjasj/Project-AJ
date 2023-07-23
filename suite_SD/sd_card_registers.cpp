
#include "sdcard_test.hpp"
#include <stdio.h>
#include "host_types.h"

class SDCardRegisterTest : public SDCardTest
{
  private:

  public:
    SDCardRegisterTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardRegisterTest()
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
		//_reg previous_value, written_value, present_value;
		uint32_t present_state = 0, previous_state = 0;
		//_reg ReadData, WriteData;
		
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
					sd_logmsg("Initialization Failed....\n");
					return ts::Test::Fail;
				}
				sd_logmsg("Initialization Complete....\n");
				
				if(init_status.result.card_unusable)
				{
					SD_LOG_DEBUG() << "Card is Unusable" << std::endl;
					return ts::Test::Fail;
				}
				
				
				if(init_status.card.sdhc_sdxc)
				{
					SD_LOG_DEBUG() << "Card is SDHC/SDXC" << std::endl;
				}
				else
				{
					SD_LOG_DEBUG() << "Card is SDSC" << std::endl;
				}
				
				if(!init_status.result.voltage_switch)
				{
					if(!init_status.host.voltage_switch)
						SD_LOG_DEBUG() << "Voltage Switch not Supported by Host" << std::endl;
					else
						SD_LOG_DEBUG() << "Voltage Switch Supported by Host" << std::endl;
					
					if(!init_status.card.voltage_switch)
						SD_LOG_DEBUG() << "Voltage Switch Supported by Card, Please inserted voltage switch supported Card...." << std::endl;
					else
						SD_LOG_DEBUG() << "Voltage Switch Supported by Card" << std::endl;
				}
				
				sd_logmsg("\n---- OCR register ----\n\n");
				
				if(init_status.result.mem_init)
				{
					SD_LOG_DEBUG() << "Card is Memory card" << std::endl;
					if(init_status.card.mem_ocr.bit.v2_7v_to_v2_8)
						SD_LOG_DEBUG() << "Voltage 2.7 to 2.8 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v2_8v_to_v2_9)
						SD_LOG_DEBUG() << "Voltage 2.8 to 2.9 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v2_9v_to_v3_0)
						SD_LOG_DEBUG() << "Voltage 2.9 to 3.0 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v3_0v_to_v3_1)
						SD_LOG_DEBUG() << "Voltage 3.0 to 3.1 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v3_1v_to_v3_2)
						SD_LOG_DEBUG() << "Voltage 3.1 to 3.2 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v3_2v_to_v3_3)
						SD_LOG_DEBUG() << "Voltage 3.2 to 3.3 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v3_3v_to_v3_4)
						SD_LOG_DEBUG() << "Voltage 3.3 to 3.4 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v3_4v_to_v3_5)
						SD_LOG_DEBUG() << "Voltage 3.4 to 3.5 is supported" << std::endl;
					if(init_status.card.mem_ocr.bit.v3_5v_to_v3_6)
						SD_LOG_DEBUG() << "Voltage 3.5 to 3.6 is supported" << std::endl;
					
					
					if(init_status.card.mem_ocr.bit.s18a)
					{
						SD_LOG_DEBUG() << "Switching to 1.8V Accepted" << std::endl;
					}
					
					if(init_status.card.mem_ocr.bit.busy)
					{
						SD_LOG_DEBUG() << "Card Powerup Status bit is set" << std::endl;
						if(init_status.card.mem_ocr.bit.ccs)
							SD_LOG_DEBUG() << "Card Capacity Status bit is set" << std::endl;
						else
							SD_LOG_DEBUG() << "Card Capacity Status bit is not set" << std::endl;
					}
					else
						SD_LOG_DEBUG() << "Card Powerup Status bit is not set" << std::endl;
				}
				else if(init_status.result.sdio_init)
				{
					SD_LOG_DEBUG() << "Card is SDIO" << std::endl;
				}
				else
				{
					SD_LOG_DEBUG() << "Card is Neither Memory nor SDIO card" << std::endl;
					return ts::Test::Fail;
				}
				
				sd_logmsg("\n---- SD Card OCR register successful ----\n\n");
				
				//CID Register
				sd_logmsg("---- CID Register ----\n");
				//CID info
				sd_logmsg("Manufacture ID: 0x%x\n", init_status.card.cid.mid());
				sd_logmsg("OEM/Application ID: 0x%x\n", init_status.card.cid.oid());
				uint64_t ProductName = init_status.card.cid.pnm();
				sd_logmsg("Product Name: %s\n", (char *)&ProductName);
				sd_logmsg("Product Revision: %d.%d\n", ((init_status.card.cid.prv() >> 4) & 0xF), (init_status.card.cid.prv() & 0xF));
				sd_logmsg("Product Serial Number: 0x%x\n", init_status.card.cid.psn());
				sd_logmsg("Manufacturing Date: %d/20%d\n\n", (init_status.card.cid.mdt() & 0xF),((init_status.card.cid.mdt() >> 4) & 0xFF));

				//RCA Register
				sd_logmsg("---- RCA Register ----\n");
				uint16_t rca1;
				uint32_t error = 0;
				if(!get_rca( &rca1, device, slot, &error))
				{
					sd_logmsg("---- SD Card register failed for RCA register ----\n\n");
					return ts::Test::Fail;
				}
				sd_logmsg("rca1 = %x\n",rca1);
				
				uint16_t rca;
				if(!get_rca( &rca, device, slot, &error))
				{
					sd_logmsg("---- SD Card register failed for RCA register ----\n\n");
					return ts::Test::Fail;
				}
				sd_logmsg("rca = %x\n",rca);
				if(rca == rca1)
				{
					sd_logmsg("SD Card register failed get new RCA\n");
					return ts::Test::Fail;
				}
				sd_logmsg("---- SD Card RCA register successful ----\n\n");
				
				//CSD Register
				sd_logmsg("---- CSD Register ----\n");
				_r2 ptr_csd;
				if(!get_csd(device, slot, &ptr_csd, &error))
				{
					sd_logmsg("---- SD Card register failed for CSD structure ----\n\n");
					return ts::Test::Fail;
				}
				sd_logmsg("---- SD Card CSD structure successful ----\n");
			
				//CSD strcture
				bool version1 = true;
				sd_logmsg("CSD structure: 0x%x\n", ptr_csd.csd_structure());
				if((1 == (ptr_csd.csd_structure() & 0x1)) && (2 != (ptr_csd.csd_structure() & 0x2)) )
					sd_logmsg("CSD register is version 1.0\n");
				else if(2 == (ptr_csd.csd_structure() & 0x2) )
				{
					sd_logmsg("CSD register is version 2.0\n");
					version1 = false;
				}
				else
				{
					sd_logmsg("---- CSD register is neither version 1.0 not version 2.0 ----\n\n");
					version1 = false;
					
					return ts::Test::Fail;
				}
				//TAAC
				uint16_t time_unit[8] = {1, 10, 100, 1, 10, 100, 1, 10}, TimeUnit;
				double time_value[16] = {0, 1.0, 1.2, 1.3, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0};
				uint8_t TimeValue;
				TimeUnit = (ptr_csd.taac() & 0x7);
				TimeValue = ((ptr_csd.taac() >> 3) & 0xF);				
				//Time Unit value
				if((TimeUnit >= 0) && (TimeUnit<=2))
					sd_logmsg("Time Unit: %dns\n", time_unit[TimeUnit]);
				else if((TimeUnit >= 3) && (TimeUnit<=5))
					sd_logmsg("Time Unit: %dus\n", time_unit[TimeUnit]);
				else if((TimeUnit == 6) || (TimeUnit<=7))
					sd_logmsg("Time Unit: %dms\n", time_unit[TimeUnit]);
				else
					sd_logmsg("Time Unit value is out of range\n");				
				//Time value
				sd_logmsg("Time Value: %.2f\n", time_value[TimeValue]);
				
				//NSAC
				sd_logmsg("Data read access-time-2 in CLK cycles (NSAC*100): 0x%x\n", ptr_csd.nsac());
				sd_logmsg("Data read access-time-2 in CLK cycles (NSAC*100): 0x%x\n", ((uint64_t)ptr_csd.nsac() * (uint64_t)100));
				
				//Transfer Speed
				uint32_t trans_speed[8] = {100, 1, 10, 100, 0, 0, 0, 0};
				uint8_t trans_rate_unit;
				trans_rate_unit = (ptr_csd.trans_speed() & 0x7);
				TimeValue = ((ptr_csd.trans_speed() >> 3) & 0xF);
				if(0 == trans_rate_unit)
					sd_logmsg("Transfer Rate Unit: %dkbps\n", trans_speed[trans_rate_unit]);
				else if((trans_rate_unit >= 1) && (trans_rate_unit <= 3))
					sd_logmsg("Transfer Rate Unit: %dMbps\n", trans_speed[trans_rate_unit]);
				else
					sd_logmsg("Transfer Rate Unit value is out of range\n");
				//Time value
				sd_logmsg("Time Value: %f\n", time_value[TimeValue]);
				
				//Card Command Classes(CCC)
				uint16_t CmdClassValue;
				CmdClassValue = ptr_csd.ccc();
				if(!(0x5B5 & CmdClassValue))
				{
					sd_logmsg("SD Card register failed for CCC bits\n");
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card register successful for CCC bits\n");
				sd_logmsg("Card Supports the Command Class ");
				for( int k=0; k<=11; k++)
				{
					if(0x1 & CmdClassValue)
						sd_logmsg("%d ", k);
					
					CmdClassValue = CmdClassValue >> 1;
				}
				sd_logmsg("\n");
				
				//Read Block Length
				if((ptr_csd.read_bl_length() >= 9) && (ptr_csd.read_bl_length() <= 11))
					sd_logmsg("MAX write data block length: %d Bytes\n", (1 << ptr_csd.read_bl_length()));
				else
					sd_logmsg("MAX write data block length is out of range\n");
				
				//Partial blocks for read
				if(ptr_csd.read_bl_partial())
					sd_logmsg("partial blocks for read allowed\n");
				else
					sd_logmsg("partial blocks for read not allowed\n");
				
				sd_logmsg("Write block misalignment: 0x%x\n", ptr_csd.write_blk_misalign());
				sd_logmsg("Read block misalignment: 0x%x\n", ptr_csd.read_blk_misalign());
				if(ptr_csd.dsr_imp())
					sd_logmsg("DSR implemented\n");
				else
					sd_logmsg("DSR not implemented\n");
				
				//Device size
				sd_logmsg("Device size: %d\n", ptr_csd.c_size());
				
				//VDD read/write current max/min
				if(version1)
				{
					float VDD_RW_Min_Current[8] = {0.5, 1, 5, 10, 25, 35, 60, 100};
					float VDD_RW_Max_Current[8] = {1, 5, 10, 25, 35, 45, 80, 200};
					sd_logmsg("Min read current @VDD min: %fmA\n", VDD_RW_Min_Current[ptr_csd.vdd_r_curr_min()]);
					sd_logmsg("Max read current @VDD max: %fmA\n", VDD_RW_Max_Current[ptr_csd.vdd_r_curr_max()]);
					sd_logmsg("Min write current @VDD min: %fmA\n", VDD_RW_Min_Current[ptr_csd.vdd_w_curr_min()]);
					sd_logmsg("Max write current @VDD max: %fmA\n", VDD_RW_Max_Current[ptr_csd.vdd_w_curr_max()]);
					
					sd_logmsg("Device size multiplier: %d\n", (0x4 << ptr_csd.c_size_mult()));
				}
				
				sd_logmsg("Erase single block enable: %d\n", ptr_csd.erase_block_enable());
				sd_logmsg("Erase sector size: %d write block\n",( ptr_csd.sector_size()+1));
				sd_logmsg("Write protect group size: %d erase sector\n", (ptr_csd.wp_grp_size()+1));
				
				//Group write protect
				if(ptr_csd.wp_grp_enable())
					sd_logmsg("Group write protect possible\n");
				else
					sd_logmsg("Group write protect not possible\n");
				
				//Write speed factor
				if((ptr_csd.r2w_factor() >= 0) && (ptr_csd.r2w_factor() <=5))
					sd_logmsg("Write speed factor: %d\n", (1 << ptr_csd.r2w_factor()));
				else
					sd_logmsg("Write speed factor is out of range\n");
				
				//Write Block Length
				if((ptr_csd.write_bl_length() >= 9) && (ptr_csd.write_bl_length() <= 11))
					sd_logmsg("MAX write data block length: %d Bytes\n", (1 << ptr_csd.write_bl_length()));
				else
					sd_logmsg("MAX write data block length is out of range\n");
				
				//Partial block for write
				if(ptr_csd.write_bl_partial())
					sd_logmsg("Partial blocks for write allowed\n");
				else
					sd_logmsg("Partial blocks for write not allowed\n");

				//Read/Write bits
				sd_logmsg("Copy flag: %d\n", ptr_csd.copy());
				
				//Permanent write protect
				if(ptr_csd.perm_write_protect())
					sd_logmsg("Card is Permanently write protected\n");
				else
					sd_logmsg("Card is not Permanently write protected\n");
				
				//Temporary write protect
				if(ptr_csd.tmp_write_protect())
					sd_logmsg("Card is Temporary write protected\n");
				else
					sd_logmsg("Card is not Temporary write protected\n");
				
				//File System
				if(ptr_csd.file_format_grp())
				{
					switch(ptr_csd.file_format())
					{
						case 0:
							sd_logmsg("Hard disk-like file system with partition table\n");
							break;
						case 1:
							sd_logmsg("DOS FAT (floppy-like) with boot sector only (no partition table)\n");
							break;
						case 2:
							sd_logmsg("Universal File Format\n");
							break;
						case 3:
							sd_logmsg("Others or Unknown File system\n");
							break;
						default:
							sd_logmsg("Unknown File Format\n");
							break;
					}
				}
				else
					sd_logmsg("Unknown File Format Group\n\n");
				
				#if 0
				//Write CSD
				#endif

				//SCR Register
				sd_logmsg("---- SCR Register ----\n");
				_scr ptr_scr;
				_r1 status;
				bool speed = true, width = true;
				if(!get_scr(device, slot, &ptr_scr, &status, speed, width))
				{
					sd_logmsg("---- SD Card register failed for SCR register ----\n\n");
					return ts::Test::Fail;
				}
				sd_logmsg("SD Card register SCR successful\n");
				
				sd_logmsg("SCR Structure: %d\n", (ptr_scr.scr_structure() & 0x1));
				sd_logmsg("SD Memory Card - Spec. Version: 0x%x\n", (ptr_scr.sd_spec() & 0x7));
				sd_logmsg("data_status_after erases: %d\n", ptr_scr.data_stst_after_erase());
				sd_logmsg("SD Security Support: 0x%x\n", ptr_scr.sd_security());
				sd_logmsg("DAT Bus widths supported: %d\n", ptr_scr.sd_bus_width());
				sd_logmsg("Spec. Version 3.00 or higher: %d\n", ptr_scr.sd_spec3());
				sd_logmsg("Extende Security Support: %d\n", ptr_scr.ex_security());
				sd_logmsg("Command Support bits: \n", ptr_scr.cmd_support());
			}
		}
		
		
		SD_LOG_DEBUG() << "SD Card Register Test Successful" << std::endl;
        return TestResult; 
    }
	
};

TServerTestInstance(SDCardRegisterTest, SDCardRegisterTest);



