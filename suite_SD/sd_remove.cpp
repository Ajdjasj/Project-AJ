
#include "sdcard_test.hpp"


class SDCardRemoveTest : public SDCardTest
{
  private:

  public:
    SDCardRemoveTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardRemoveTest()
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
		char ch = '\0';
		
		printf("Is SD card Removed ?\n");
		ch = getchar();
		while( (ch != 'y') && (ch != 'Y') && (ch != 'n') && (ch != 'N') )
		{
			ch = getchar();
		}
		
		if((ch == 'y') || (ch == 'Y'))
		{
			Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			Slots = get_total_slot(device);
			for(int slot=0; slot<Slots; slot++)
			{
				if(detect_card(device, slot))
					{
						SD_LOG_DEBUG() << "SDCard Remove test Failed for device" << device << " and slot" << slot << std::endl;
						return ts::Test::Fail; 
					}
				}
			}
		}
		else
		{
			SD_LOG_DEBUG() << "SDCard not removed" << std::endl;
			return ts::Test::Skip; 
		}
		
		SD_LOG_DEBUG() << "SDCard remove test Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDCardRemoveTest, SDCardRemoveTest);
