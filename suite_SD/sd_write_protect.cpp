
#include "sdcard_test.hpp"


class SDWriteProtectTest : public SDCardTest
{
  private:

  public:
    SDWriteProtectTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDWriteProtectTest()
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
		
		SD_LOG_DEBUG() <<"Insert the SD card with Write protection switch enabled " << std::endl;
			Devices = get_total_pcidevice();
			for(int device=0; device<Devices; device++)
			{
				Slots = get_total_slot(device);
				for(int slot=0; slot<Slots; slot++)
				{

					if(!detect_card(device, slot))
					{
						SD_LOG_DEBUG() << "SDCard Detection Failed, Please insert a valid card"<< std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "Card detected"<< std::endl;
					if(!card_writeprotected(device, slot))
					{
						SD_LOG_DEBUG() << "SDCard Write Protect test Failed for device" << device << " and slot" << slot << std::endl;
						return ts::Test::Fail;
					}
				}
			}
		
		SD_LOG_DEBUG() << "SDCard Write Protect Test Successful" << std::endl;
        return TestResult; 
    }
	
};

TServerTestInstance(SDWriteProtectTest, SDWriteProtectTest);
