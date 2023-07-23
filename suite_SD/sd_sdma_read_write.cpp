
#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCardSdmaReadWriteTest : public SDCardTest
{
  private:

  public:
    SDCardSdmaReadWriteTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardSdmaReadWriteTest()
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
		bool PartialRead = false, PartialWrite = false;
		bool Speed = false;
		bool BusWidth[2] = {true, false};
		uint8_t autocmd = 0x0, Write_buffer[2048] = {0}, Read_buffer[2048] = {0};
		uint32_t card_address = 0x8000, blk_size = 512, blk_count = 0;
		uint16_t type 		= Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); 
		
			SD_LOG_DEBUG()<< "---------------------  SDMA test------------- " <<std::endl;
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD device " <<std::dec << device << " SD slot " <<std::dec << slot <<" ---------------------" << std::endl;
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed for device"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected...."<<std::endl;
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() <<"Initialization Failed"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Initialization Complete...." <<std::endl;
				
				
			//Single block Read/Write test
				if(type == 1)
				{
					blk_size = 512;
					blk_count = 0;
					
					if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
					{
						SD_LOG_DEBUG() << "SDMA single block read/write failed"<< std::endl;
						return ts::Test::Fail;
					}
					if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, Speed, false, autocmd))
					{
						SD_LOG_DEBUG() << "SDMA single block read/write failed"<< std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "SDMA single block read/write complete"<< std::endl;
	
					SD_LOG_DEBUG() << "SDMA Single block Read/Write test Passed" << std::endl;
				}
				
			//Multiple block Read/Write test
				if(type == 2)
				{
					autocmd = 0x1;
					blk_size = 512;
					blk_count = 400;
					//for(int k=0; k<2; k++)
					{

			
			           //Read/Write Data and Compare the data
						if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, Speed, false, autocmd))
						{
							SD_LOG_DEBUG()<< "SDMA multiple block read/write failed , Buffer boundry = 4Kb, Data width 1Bit, total data size = "<< std::dec << blk_count <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG () << "SDMA multiple block read/write Completed , Buffer boundry = 4Kb, Data width 1Bit, total data size = " << std::dec << blk_count <<"blocks"<< std::endl;
						
						if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							SD_LOG_DEBUG() << "SDMA multiple block read/write failed , Buffer boundry = 4Kb, Data width 4Bit, total data size = " << std::dec <<blk_count <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG() << "SDMA multiple block read/write Completed , Buffer boundry = 4Kb, Data width 4Bit, total data size =" << std::dec <<blk_count << "blocks";						
						if(!SDMAReadWrite(card_address, 0x01 /*mem size*/, blk_size, blk_count, Speed, false, autocmd))
						{
							SD_LOG_DEBUG() << "SDMA multiple block read/write failed , Buffer boundry = 8Kb, Data width 1Bit, total data size =" << std::dec <<blk_count <<"blocks" <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG()<<"SDMA multiple block read/write Completed , Buffer boundry = 8Kb, Data width 1Bit, total data size =" <<std::dec<< blk_count <<"blocks"<<std::endl;

						if(!SDMAReadWrite(card_address, 0x01 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							SD_LOG_DEBUG()<<"SDMA multiple block read/write failed , Buffer boundry = 8Kb, Data width 4Bit, total data size = " <<std::dec << blk_count << "blocks" <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG() << "SDMA multiple block read/write Completed , Buffer boundry = 8Kb, Data width 4Bit, total data size =" << std::dec << blk_count <<"blocks" <<std::endl;
						
						if(!SDMAReadWrite(card_address, 0x02 /*mem size*/, blk_size, blk_count, Speed, false, autocmd))
						{
							SD_LOG_DEBUG()<<"SDMA multiple block read/write failed , Buffer boundry = 16Kb, Data width 1Bit, total data size =" <<std::dec<< blk_count <<"blocks" <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG()<<"SDMA multiple block read/write Completed , Buffer boundry = 16Kb, Data width 1Bit, total data size =" <<std::dec<< blk_count <<" blocks" <<std::endl;
						
						if(!SDMAReadWrite(card_address, 0x02 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							SD_LOG_DEBUG() << "SDMA multiple block read/write failed , Buffer boundry = 16Kb, Data width 4Bit, total data size = " <<std::dec << blk_count <<"blocks" <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG() << "SDMA multiple block read/write Completed , Buffer boundry = 16Kb, Data width 4Bit, total data size =" <<std::dec<< blk_count << "blocks" <<std::endl;
						
						if(!SDMAReadWrite(card_address, 0x03 /*mem size*/, blk_size, blk_count, Speed, false, autocmd))
						{
							SD_LOG_DEBUG() << "SDMA multiple block read/write failed , Buffer boundry = 32Kb, Data width 1Bit, total data size = " << std::dec <<"blocks" <<std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG() <<"SDMA multiple block read/write Completed , Buffer boundry = 32Kb, Data width 1Bit, total data size ="<< std::dec << blk_count << "blocks" << std::endl;
						
						if(!SDMAReadWrite(card_address, 0x03 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
						        SD_LOG_DEBUG() << "SDMA multiple block read/write failed , Buffer boundry = 32Kb, Data width 4Bit, total data size = " << std::dec << blk_count <<" blocks";
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG() << "SDMA multiple block read/write Completed , Buffer boundry = 32Kb, Data width 4Bit, total data size =" <<std::dec << blk_count<< "blocks" <<std::endl;
						
#if 0
						blk_count = 800;
						if(!SDMAReadWrite(card_address, 0x00 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
							//return ts::Test::Fail;
						}
						sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 4Kb . total data size = %d blocks \n",blk_count);
						
						
						if(!SDMAReadWrite(card_address, 0x01 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 8Kb . total data size = %d blocks \n",blk_count);
							//return ts::Test::Fail;
						}
						sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 8Kb . total data size = %d blocks \n",blk_count);
						
						if(!SDMAReadWrite(card_address, 0x02 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 16Kb . total data size = %d blocks \n",blk_count);
							//return ts::Test::Fail;
						}
						sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 16Kb . total data size = %d blocks \n",blk_count);
						
						if(!SDMAReadWrite(card_address, 0x03 /*mem size*/, blk_size, blk_count, Speed, true, autocmd))
						{
							sd_logmsg("SDMA multiple block read/write failed , Buffer boundry = 32Kb . total data size = %d blocks \n",blk_count);
							//return ts::Test::Fail;
						}
						sd_logmsg("SDMA multiple block read/write Completed , Buffer boundry = 32Kb . total data size = %d blocks \n",blk_count);						
#endif						
					}
					SD_LOG_DEBUG() << "SDMA Multiple block Read/Write test Passed" << std::endl;
				}	
			}
			//free(SDCardTest::resource1);
			//free(SDCardTest::resource2);
			//free(SDCardTest::resource3);
		}
		
		//SD_LOG_DEBUG() << "SDMA Read/Write test Successful" << std::endl;
		return TestResult; 
    }
};

TServerTestInstance(SDCardSdmaReadWriteTest, SDCardSdmaReadWriteTest);
