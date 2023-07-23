
#include "sdcard_test.hpp"

#define PCI_CAP1_SIZE	14
#define PCI_CAP2_SIZE	10


#define PCI_CFG_HOST_VALUE_READ(Position, value, Mask) 	((value >> Position) & Mask)
#define PCI_CFG_HOST_VALUE_WRITE(Position, value, WriteValue, Mask) 	((WriteValue << Position) | (value & (~(Mask << Position))))

//Structure for SD Debug register and Host Capabilities register
struct PCI_cfg
{
	uint32_t Position;
	uint32_t BitSize;
	uint8_t Name[40];
};

//SD Debug register array
PCI_cfg PCI_Cap1_Reg[PCI_CAP1_SIZE] = { 
{(0), (6), {"Timeout Clock Frequency"}}, 
{(7), (1), {"Timeout Clock Select"}}, 
{(8), (8), {"Base Clock Frequency"}}, 
{(16), (6), {"Max Block Length"}}, 
{(18), (1), {"MMC 8-bit support"}}, 
{(19), (1), {"Advanced DMA support"}}, 
{(20), (1), {"Advanced DMA2 support"}},
{(21), (1), {"High speed support"}}, 
{(22), (1), {"SDMA support"}}, 
{(23), (1), {"Suspend/Resume support"}}, 
{(24), (3), {"Voltage support"}}, 
{(28), (1), {"64-bit system address support"}}, 
{(29), (1), {"Asynchronous Interrupt Support"}}, 
{(30), (2), {"SlotType"}}, 
};

PCI_cfg PCI_Cap2_Reg[PCI_CAP2_SIZE] = {
{(0), (1), {"SDR50 Support"}},
{(1), (1), {"SDR104 Support"}},
{(2), (1), {"DDR50 Support"}},
{(4), (1), {"Driver Type A Support"}},
{(5), (1), {"Driver Type C Support"}},
{(6), (1), {"Driver Type D Support"}},
{(8), (4), {"Timer Count for Re-Tuning"}},
{(13), (1), {"Use Tuning for SDR50"}},
{(14), (2), {"Re-Tuning Modes"}},
{(16), (8), {"Clock Multiplier"}},
};

//Host Capabilities register array
PCI_cfg Host_Cap1_Reg[PCI_CAP1_SIZE] = {
{(0), (6), {"Timeout Clock Frequency"}},
{(7), (1), {"Timeout Clock Select"}},
{(8), (8), {"Base Clock Frequency"}},
{(16), (6), {"Max Block Length"}},
{(18), (1), {"MMC 8-bit support"}},
{(19), (1), {"Advanced DMA support"}},
{(20), (1), {"Advanced DMA2 support"}},
{(21), (1), {"High speed support"}},
{(22), (1), {"SDMA support"}},
{(23), (1), {"Suspend/Resume support"}},
{(24), (3), {"Voltage support"}},
{(28), (1), {"64-bit system address support"}},
{(29), (1), {"Asynchronous Interrupt Support"}},
{(30), (2), {"SlotType"}},
};


PCI_cfg Host_Cap2_Reg[PCI_CAP2_SIZE] = {
{(0), (1), {"SDR50 Support"}},
{(1), (1), {"SDR104 Support"}},
{(2), (1), {"DDR50 Support"}},
{(4), (1), {"Driver Type A Support"}},
{(5), (1), {"Driver Type C Support"}},
{(6), (1), {"Driver Type D Support"}},
{(8), (4), {"Timer Count for Re-Tuning"}},
{(13), (1), {"Use Tuning for SDR50"}},
{(14), (2), {"Re-Tuning Modes"}},
{(16), (8), {"Clock Multiplier"}},
};


uint32_t mask_calculate(uint32_t Bits)
{
	uint32_t MaskValue = 1;
	
	Bits = Bits - 1;
	while(Bits)
	{
		MaskValue = (MaskValue << 1) | 0x1;
		Bits = Bits - 1;
	};
	
	return MaskValue;
}

class SDCardBackdoorOverwriteTest : public SDCardTest
{
  private:

  public:
    SDCardBackdoorOverwriteTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCardBackdoorOverwriteTest()
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
		_reg previous_value; //, written_value;
		uint32_t HostCap = 0, PCIvalue = 0, BackupPCIValue = 0;
		uint8_t sd_pci_cap1 = 0, sd_host_cap1 = 0, sd_pci_cap1_Cmp = 0, sd_host_cap1_Cmp = 0;
		uint8_t sd_pci_cap2 = 0, sd_host_cap2 = 0, sd_pci_cap2_Cmp = 0, sd_host_cap2_Cmp = 0;
		uint32_t sd_pci_cap_mask = 0, host_cap_mask = 0;
		
		
		Devices = get_total_pcidevice();
		for(int device=0; device<Devices; device++)
		{
			//PCI read
			BackupPCIValue = PCIvalue = read_pcicfg(device, SDHC_PCI_SDCAP_0 , DWORD);
			sd_logmsg("PCI SD Cap1 Register: 0x%x\n", PCIvalue);

			//Host Cap read
			HostCap = 0;
			SET_SD_REGISTER_READ(&previous_value,HOST_CAPABILITIES,0,0);
			if(!read_host_reg(&previous_value))
			{
				SD_LOG_DEBUG() << "read HOST_CAPABILITIES register failed" << std::endl;
				write_pcicfg(device, SDHC_PCI_SDCAP_0, DWORD, BackupPCIValue);
				return ts::Test::Fail;
			}
			HostCap = previous_value.sd_reg.value;
			sd_logmsg("Host Capabilities1 Register: 0x%x\n", HostCap);
			
			//Extract the values
			for(int i=0; i<PCI_CAP1_SIZE; i++)
			{
				sd_pci_cap_mask = mask_calculate(PCI_Cap1_Reg[i].BitSize);
				host_cap_mask = mask_calculate(Host_Cap1_Reg[i].BitSize);

				sd_logmsg("\n---- %s ----\n", PCI_Cap1_Reg[i].Name);
				sd_pci_cap1 = PCI_CFG_HOST_VALUE_READ(PCI_Cap1_Reg[i].Position, PCIvalue, sd_pci_cap_mask);
				sd_host_cap1 = PCI_CFG_HOST_VALUE_READ(Host_Cap1_Reg[i].Position, HostCap, host_cap_mask);
				//sd_logmsg("SD Cap1 value: 0x%x\n", sd_pci_cap1);
				//sd_logmsg("Host Capabilities value: 0x%x\n", sd_host_cap1);
				sd_logmsg("Mask Value: 0x%x\n", sd_pci_cap_mask);
				
				//Write the changed value
				sd_pci_cap1 = (sd_pci_cap_mask & (~sd_pci_cap1));
				sd_host_cap1 = (host_cap_mask & (~sd_host_cap1));
				sd_logmsg("SD Cap1 value: 0x%x\n", sd_pci_cap1);
				sd_logmsg("Host Capabilities value: 0x%x\n", sd_host_cap1);
				PCIvalue = PCI_CFG_HOST_VALUE_WRITE(PCI_Cap1_Reg[i].Position, PCIvalue, sd_pci_cap1, sd_pci_cap_mask);
				
				//PCI write
				write_pcicfg(device, SDHC_PCI_SDCAP_0 , DWORD ,PCIvalue );
				
				env::SleepMs(100);
		
				//PCI read
				PCIvalue = read_pcicfg(device, SDHC_PCI_SDCAP_0 , DWORD);
				sd_logmsg("SD Cap1 After Write: 0x%x\n", PCIvalue);

				//Host Cap read
				HostCap = 0;
				SET_SD_REGISTER_READ(&previous_value,HOST_CAPABILITIES,0,0);
				if(!read_host_reg(&previous_value))
				{
					SD_LOG_DEBUG() << "read HOST_CAPABILITIES1 register failed" << std::endl;
					write_pcicfg(device, SDHC_PCI_SDCAP_0, DWORD, BackupPCIValue);
					return ts::Test::Fail;
				}
				HostCap = previous_value.sd_reg.value;
				sd_logmsg("Host Cap1 After Write: 0x%x\n", HostCap);

				sd_pci_cap_mask = PCI_CFG_HOST_VALUE_READ(PCI_Cap1_Reg[i].Position, PCIvalue, sd_pci_cap_mask);
				host_cap_mask = PCI_CFG_HOST_VALUE_READ(Host_Cap1_Reg[i].Position, HostCap, host_cap_mask);
				sd_logmsg("Read SD Capabilities1 value: 0x%x\n", sd_pci_cap_mask);
				sd_logmsg("Read Host Capabilities1 value: 0x%x\n", host_cap_mask);
				if(host_cap_mask != sd_host_cap1)
				{
					sd_logmsg("Failed for: %s\n", Host_Cap1_Reg[i].Name);
					sd_logmsg("Expected Host Capabilities1 value: 0x%x\n", sd_host_cap1);
					sd_logmsg("Read Host Capabilities1 value: 0x%x\n", host_cap_mask);
					SD_LOG_DEBUG() << "SDCard Backdoor Overwrite Test Failed for Host Capabilities1" << std::endl;
					write_pcicfg(device, SDHC_PCI_SDCAP_0, DWORD, BackupPCIValue);
					return ts::Test::Fail;
				}
				write_pcicfg(device, SDHC_PCI_SDCAP_0, DWORD, BackupPCIValue);
			}
			
			//PCI read
			BackupPCIValue = PCIvalue = read_pcicfg(device, SDHC_PCI_SDCAP_1 , DWORD);
			sd_logmsg("PCI SD Cap2 Register: 0x%x\n", PCIvalue);

			//Host Cap read
			HostCap = 0;
			SET_SD_REGISTER_READ(&previous_value,HOST_CAPABILITIES_1,0,0);
			if(!read_host_reg(&previous_value))
			{
				SD_LOG_DEBUG() << "read Host Capabilities 2 register failed" << std::endl;
				write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD, BackupPCIValue);
				return ts::Test::Fail;
			}
			HostCap = previous_value.sd_reg.value;
			sd_logmsg("Host Capabilities2 Register: 0x%x\n", HostCap);
			
			//Extract the values
			for(int i=0; i<PCI_CAP2_SIZE; i++)
			{
				sd_pci_cap_mask = mask_calculate(PCI_Cap2_Reg[i].BitSize);
				host_cap_mask = mask_calculate(Host_Cap2_Reg[i].BitSize);

				sd_logmsg("\n---- %s ----\n", PCI_Cap2_Reg[i].Name);
				sd_pci_cap1 = PCI_CFG_HOST_VALUE_READ(PCI_Cap2_Reg[i].Position, PCIvalue, sd_pci_cap_mask);
				sd_host_cap1 = PCI_CFG_HOST_VALUE_READ(Host_Cap2_Reg[i].Position, HostCap, host_cap_mask);
				//sd_logmsg("SD Cap2 value: 0x%x\n", sd_pci_cap1);
				//sd_logmsg("Host Capabilities2 value: 0x%x\n", sd_host_cap1);
				sd_logmsg("Mask Value: 0x%x\n", sd_pci_cap_mask);
				
				//Write the changed value
				sd_pci_cap1 = (sd_pci_cap_mask & (~sd_pci_cap1));
				sd_host_cap1 = (host_cap_mask & (~sd_host_cap1));
				sd_logmsg("SD Cap2 value: 0x%x\n", sd_pci_cap1);
				sd_logmsg("Host Capabilities2 value: 0x%x\n", sd_host_cap1);
				PCIvalue = PCI_CFG_HOST_VALUE_WRITE(PCI_Cap2_Reg[i].Position, PCIvalue, sd_pci_cap1, sd_pci_cap_mask);
				
				//PCI write
				write_pcicfg(device, SDHC_PCI_SDCAP_1 , DWORD ,PCIvalue );
				
				env::SleepMs(100);
		
				//PCI read
				PCIvalue = read_pcicfg(device, SDHC_PCI_SDCAP_1 , DWORD);
				sd_logmsg("SD Cap2 After Write: 0x%x\n", PCIvalue);

				//Host Cap read
				HostCap = 0;
				SET_SD_REGISTER_READ(&previous_value,HOST_CAPABILITIES_1,0,0);
				if(!read_host_reg(&previous_value))
				{
					SD_LOG_DEBUG() << "read Host Capabilities 2 register failed" << std::endl;
					write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD, BackupPCIValue);
					return ts::Test::Fail;
				}
				HostCap = previous_value.sd_reg.value;
				sd_logmsg("Host Cap2 After Write: 0x%x\n", HostCap);

				sd_pci_cap_mask = PCI_CFG_HOST_VALUE_READ(PCI_Cap2_Reg[i].Position, PCIvalue, sd_pci_cap_mask);
				host_cap_mask = PCI_CFG_HOST_VALUE_READ(Host_Cap2_Reg[i].Position, HostCap, host_cap_mask);
				sd_logmsg("Read SD Capabilities2 value: 0x%x\n", sd_pci_cap_mask);
				sd_logmsg("Read Host Capabilities2 value: 0x%x\n", host_cap_mask);
				if(host_cap_mask != sd_host_cap1)
				{
					sd_logmsg("Failed for: %s\n", Host_Cap2_Reg[i].Name);
					sd_logmsg("Expected Host Capabilities2 value: 0x%x\n", sd_host_cap1);
					sd_logmsg("Read Host Capabilities2 value: 0x%x\n", host_cap_mask);
					SD_LOG_DEBUG() << "SDCard Backdoor Overwrite Test Failed for Host Capabilities2" << std::endl;
					write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD, BackupPCIValue);
					return ts::Test::Fail;
				}
				write_pcicfg(device, SDHC_PCI_SDCAP_1, DWORD, BackupPCIValue);
			}
		}
		SD_LOG_DEBUG() << "SDCard Backdoor Overwrite Test Successful" << std::endl;
        return TestResult; 
    }
	
};

TServerTestInstance(SDCardBackdoorOverwriteTest, SDCardBackdoorOverwriteTest);
