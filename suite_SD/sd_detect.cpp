
#include "sdcard_test.hpp"


class SDCardDetectTest : public SDCardTest
{
  private:

  public:
    SDCardDetectTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardDetectTest()
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
       // Result TestResult = ts::Test::Pass;
		uint8_t device = 0, slot = 0;
		
		//char ch = '\0';
		uint32_t test_type = Parameter<uint32_t>("type",0, coreutil::FmtFlags(std::ios::hex));
		
	#if SD_PCI
		uint8_t Maxdevice = get_total_pcidevice();
		uint8_t Maxslot = 0;
	#if 1
		_reg prsnt_state_register;
		uint64_t PCIvalue = 0;
		//PCI read
		PCIvalue = read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
		SD_LOG_DEBUG() << "SD SDHC_PCI_SDCONTROL: before " << std::hex << PCIvalue << std::endl;
		PCIvalue = ((PCIvalue & 0xFFFFFF) | 0x02000000);
		//write_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD, PCIvalue);
		//utilSleep(200);
		//PCIvalue = read_pcicfg(device, SDHC_PCI_SDCONTROL , DWORD);
		sd_logmsg("SD SDHC_PCI_SDCONTROL: after 0x%x\n", PCIvalue);
  		SET_SD_REGISTER_READ(&prsnt_state_register,HOST_CAPABILITIES,device,slot);
  		if(!read_host_reg(&prsnt_state_register))
  			return TestResult;
  		sd_logmsg("Present State Register : HOST_CAPABILITIES -->%X\n", prsnt_state_register.sd_reg.value); 
  		SD_LOG_DEBUG() << "Present State Register : HOST_CAPABILITIES -->" << std::hex << prsnt_state_register.sd_reg.value << std::endl; 

  		SET_SD_REGISTER_READ(&prsnt_state_register,HOST_HOST_VERSION,device,slot);
  		if(!read_host_reg(&prsnt_state_register))
  			return TestResult;
  		sd_logmsg("Present State Register : HOST_HOST_VERSION -->%X\n", prsnt_state_register.sd_reg.value); 
  		SD_LOG_DEBUG() << "Present State Register : HOST_HOST_VERSION -->" << std::hex <<  prsnt_state_register.sd_reg.value << std::endl; 
		#endif
		for(device=0; device < Maxdevice; device++)
		{
			Maxslot = get_total_slot(device);
			for(slot=0; slot < Maxslot; slot++)
			{
	#endif
	#if (SD_PCI || SD_MMIO)
				if(1 == test_type || test_type == 2 || test_type == 3 )
				{
					if(!detect_card(device, slot))
					{
						SD_LOG_DEBUG() << "SDCard Detection Failed for device" << device << " and slot" << slot << std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "SDCard Detection Successful" << std::endl;
					return ts::Test::Pass;
				}
	#endif
			 	#if 0	
				else if(2 == test_type)
				{
					sd_logmsg("Is SD card Inserted in slot : %d (y/n)?\n",(slot +1));
					ch = getchar();
					while( (ch != 'y') && (ch != 'Y') && (ch != 'n') && (ch != 'N') )
					{
						ch = getchar();
					}
					
					if((ch == 'y') || (ch == 'Y'))
					{
						if(!detect_card(device, slot))
						{
							SD_LOG_DEBUG() << "SDCard Insert test Failed "<< std::endl;
							return ts::Test::Fail;
						}
						SD_LOG_DEBUG() << "SDCard Insert Test Successful" << std::endl;
						return ts::Test::Pass;
					}
					
					else if((ch == 'n') || (ch == 'N'))
					{
						SD_LOG_DEBUG() << "Selected No , Nothing to execute test skiped" << std::endl;
						return ts::Test::Skip;
					}
				}
				#endif 

				#if 0
				else if(3 == test_type)
				{
					sd_logmsg("Is SD card Removed in slot : %d (y/n)?\n",(slot +1));
					ch = getchar();
					while( (ch != 'y') && (ch != 'Y') && (ch != 'n') && (ch != 'N') )
					{
						ch = getchar();
					}
					
					if((ch == 'y') || (ch == 'Y'))
					{
						if(detect_card(device, slot))
						{
							SD_LOG_DEBUG() << "SDCard Remove test Failed" << std::endl;
							return ts::Test::Fail; 

						}
						SD_LOG_DEBUG() << "SDCard remove test Successful" << std::endl;
						return ts::Test::Pass;						
					}
					else if((ch == 'n') || (ch == 'N'))
					{
						SD_LOG_DEBUG() << "Selected No , Nothing to execute test skiped" << std::endl;
						return ts::Test::Skip; 
					}
					
				}
				#endif
		#if SD_PCI
			}
		}
		#endif
	return ts::Test::Pass;		
    }
	
};

TServerTestInstance(SDCardDetectTest, SDCardDetectTest);
