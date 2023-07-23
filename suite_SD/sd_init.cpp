
#include "sdcard_test.hpp"


class SDCardInitTest : public SDCardTest
{
  private:

  public:
    SDCardInitTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardInitTest()
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
		int device =0;
		int slot=0;
		
		#if SD_PCI
		uint8_t Devices = 0, Slots = 0;
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
		#endif 	
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SD card detect Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				
				//_init_status init_status;
				bool voltage_switch_enabled =false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "SD Card Init test Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
		#if SD_PCI
			}
			if(TestResult == ts::Test::Fail)
				break;
		}
		#endif
		SD_LOG_DEBUG() << "SD Card Initialization Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardInitTest, SDCardInitTest);
