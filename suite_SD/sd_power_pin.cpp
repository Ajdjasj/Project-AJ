#include <env_time.h>
#include "sdcard_test.hpp"
#include "host_types.h"


class SDPowerPinTest : public SDCardTest
{
  private:

  public:
    SDPowerPinTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDPowerPinTest()
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
		
		uint32_t Device = 0, Slot = 0 , error = 0x00;
		//_init_status init_status;
		uint16_t type 		= Parameter<uint16_t>("type",0, coreutil::FmtFlags(std::ios::hex)); 
		uint16_t delay 		= Parameter<uint16_t>("powercycledelay",0, coreutil::FmtFlags(std::ios::hex));		
	
	#if SD_PCI
	uint32_t MaxDevices = get_total_pcidevice(), MaxSlots = 0;
		for(Device = 0; Device < MaxDevices; Device++)
		{
			MaxSlots = get_total_slot(Device);
			for(Slot = 0; Slot < MaxSlots; Slot++)
			{
				SD_LOG_DEBUG() << "--------------------- SD Device : " << Device << " , SD Slot : "<< Slot <<" ---------------------" << std::endl;
	#endif
				//Detect Card
				if(!detect_card(Device, Slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed "<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				if(!SDCardTest::Initalization(Device, Slot, true))
				{
					SD_LOG_DEBUG() << "Initalization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Init Completed" << std::endl;
				
				//Change the speed mode
				if(1 == type)
				{
					if(!go_inactive( &error))
					{
						SD_LOG_DEBUG() << "Go Inactive Failed"<< std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "Card Is in Inactive State"<< std::endl;
				}
				else if(2 == type)
				{
					if(init_status.result.voltage_switch == false)
					{
						SD_LOG_DEBUG() << "Voltage switch Failed "<< std::endl;
						if(init_status.host.voltage_switch == false)
							SD_LOG_DEBUG() << "Host doesnot support Voltage switch "<<std::endl;
						if(init_status.card.voltage_switch == false)
							SD_LOG_DEBUG() << "Card doesnot support Voltage switch "<<std::endl;
						return ts::Test::Fail;
					}
					SD_LOG_DEBUG() << "Card Is in 1.8 Operating Voltage "<< std::endl;
				}
				else
				{
					SD_LOG_DEBUG() << "Wrong Type selected by the user from build.xml , type should be"<<std::endl<<" 1 for test : Power Pin test - Card Inactive->PowerCycle->Card Should be active" << std::endl<< " 2 for Power Pin test - (Need UHS-I Card): Card Voltage Switch->PowerCycle->Card Voltage Switch"<< std::endl;
					return ts::Test::Fail;
				}

				card_power(false, Device, Slot);
				env::SleepMs(delay);
				card_power(true, Device, Slot);

				//Initialization
				if(!SDCardTest::Initalization(Device, Slot,true))
				{
					if(1 == type)
					{
						SD_LOG_DEBUG() << "Second Initalization Failed Card not recoverd from Inactive state Power Cycle Failed "<< std::endl;
						SD_LOG_DEBUG() << "Power Cycle Failed to Hard Reset The Card"<<std::endl;
					}
					else
						SD_LOG_DEBUG() << "Second Initalization Failed for SD Device " << std::endl;
					
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Second Init Completed"<< std::endl;

				if(1 == type)
					return ts::Test::Pass;

				if(init_status.result.voltage_switch == false)
				{
					SD_LOG_DEBUG() << "Voltage switch Failed after Power Cycle "<< std::endl;
					if(init_status.card.voltage_switch == false)
						SD_LOG_DEBUG() << "Power Cycle Failed to Hard Reset The Card"<<std::endl;
					return ts::Test::Fail;
				}
			#if SD_PCI
			}
		}
		#endif
		return ts::Test::Pass;	
	}
};
TServerTestInstance(SDPowerPinTest, SDPowerPinTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
