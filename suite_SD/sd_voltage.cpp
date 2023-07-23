
#include "sdcard_test.hpp"


class SDVoltageSwitchTest : public SDCardTest
{
  private:

  public:
    SDVoltageSwitchTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDVoltageSwitchTest()
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
					SD_LOG_DEBUG() << "SD card detect Failed for device" << device << " and slot" << slot << std::endl;
					return ts::Test::Fail;
				}
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled =true;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG()<<"Initialization Failed...." <<std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG()<<"Initialization Complete...."<<std::endl;
				
				if(!init_status.result.voltage_switch)
				{
					if(!init_status.host.voltage_switch)
						SD_LOG_DEBUG() << "Voltage Switch not Supported by Host" << std::endl;
					else
						SD_LOG_DEBUG() << "Voltage Switch Supported by Host" << std::endl;
					
					if(init_status.card.voltage_switch)
						SD_LOG_DEBUG() << "Voltage Switch Supported by Card, Please inserted voltage switch supported Card...." << std::endl;
					else
						SD_LOG_DEBUG() << "Voltage Switch not Supported by Card" << std::endl;
					
					return ts::Test::Fail;
				}
			}
		}
		
		SD_LOG_DEBUG() << "SD Card Voltage Switch Successful" << std::endl;
		return TestResult; 
    }
	
};

TServerTestInstance(SDVoltageSwitchTest, SDVoltageSwitchTest);
