
#include "sdcard_test.hpp"
#include "host_types.h"


class SDDeviceIdTest : public SDCardTest
{
  private:

  public:
    SDDeviceIdTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDDeviceIdTest()
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
		uint32_t MaxDevices = get_total_pcidevice(), MaxSlots = 0;
		uint32_t Device = 0, Slot = 0;

		uint16_t device_id 		= Parameter<uint16_t>("deviceid",0, coreutil::FmtFlags(std::ios::hex)); 
		Result TestResult = ts::Test::Fail;		
		_pci_type0_header pci_header;
		
		for(Device = 0; Device < MaxDevices; Device++)
		{
				//Read PCi header
				get_pciheader(&pci_header , Device);
				SD_LOG_DEBUG() << "pci_header.deviceID" << std::hex << pci_header.device_id;
				SD_LOG_DEBUG() << " Device ID" << std::hex << device_id;
				if(pci_header.device_id != device_id)
				//for Carrizo both PCI Header Device Id and Device ID differ
				{
					SD_LOG_DEBUG() << "Device Id : 0x"<< std::hex << device_id << "  Found in PCI Device location, Bus = 0x"<< std::hex << (uint16_t)pci_header.bus <<" Device = 0x"<< std::hex << (uint16_t)pci_header.device << " Function = 0x" << std::hex << (uint16_t)pci_header.function <<std::endl;
					TestResult = ts::Test::Pass;
				}
		}
		return TestResult;	
	}
};
TServerTestInstance(SDDeviceIdTest, SDDeviceIdTest);

//TServerTestInstance(SDHCSDXCpioReadWrite, SDHCSDXCcardpioReadWrite);
