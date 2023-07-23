#include "sdcard_test.hpp"
#include "host_types.h"
#include "card.h"


class SDCompliance_2_1_13Test : public SDCardTest
{
  private:

  public:
    SDCompliance_2_1_13Test(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDCompliance_2_1_13Test()
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
		printf("Please Capture the trace and press Enter to exit...\n");
		getchar();
		
		SDCardTest::PostRun();
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		
		//bool PartialRead = false, PartialWrite = false;
		bool Speed = false;
		//bool BusWidth = false;
		uint8_t autocmd = 0x0;
		uint32_t card_address = 0x4000, blk_size = 512, blk_count = 0, data = 0xbeafdead;
		int device=0; 
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
				SD_LOG_DEBUG() << "--------------------- SD device : " << device << " , SD slot : "<< slot <<" ---------------------" << std::endl;
				if(!detect_card(device, slot))
				{
					SD_LOG_DEBUG() << "SDCard Detection Failed"<< std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() << "Card detected"<< std::endl;
				
				//Initialization
				//_init_status init_status;
				bool voltage_switch_enabled = false;
				if(!SDCardTest::Initalization(device, slot, voltage_switch_enabled))
				{
					SD_LOG_DEBUG() << "Initialization Failed" << std::endl;
					return ts::Test::Fail;
				}
				SD_LOG_DEBUG() <<"Initialization Complete"<< std::endl;
				
			
					//Multiple block Read/Write test
					autocmd = 0x1;
					blk_size = 512;
					blk_count = 100;
					card_address = 0x6000;
					data = 0xdeadbeaf;
					uint32_t size = (blk_size * (blk_count == 0 ? 1 : blk_count)), index = 0x00, data_index = 0x00; 
					uint8_t *write_buffer; 
					write_buffer    = new uint8_t[size]; 
					
					uint8_t array[5] = {((uint8_t)(data & 0x000000FF)), ((uint8_t)((data & 0x0000FF00) >> 8)) , ((uint8_t)((data & 0x00FF0000) >> 16)) , ((uint8_t)((data & 0xFF000000) >> 24))}; 
					//printf("Index : %x\n",(uint16_t)size); 
					
					for(index = 0 ; index < size ; index++) 
					{ 
						write_buffer[index] = array[data_index]; 
						if(++data_index == 4) 
						data_index = 0; 
					} 
						//writing the data       
					if(!pio_write( device, slot, Speed, true, autocmd, card_address, write_buffer, blk_size, blk_count)) 
					{
						printf("Failed in pio_write\n");
						return ts::Test::Fail;; 
					}
					SD_LOG_DEBUG() <<"Multi block PIO Write Completed"<< std::endl;	
					printf("Please Capture the trace and press Enter to exit...\n");
					getchar();
					SD_LOG_DEBUG() <<"Multi block PIO Read/Write Completed"<< std::endl;	
			#if SD_PCI
			}
		}
		#endif
		return TestResult; 
    }
};

TServerTestInstance(SDCompliance_2_1_13Test, SDCompliance_2_1_13Test);
