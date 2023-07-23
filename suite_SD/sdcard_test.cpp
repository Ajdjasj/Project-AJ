/*
##
##  Trade secret of Advanced Micro Devices, Inc.
##  Copyright 2007, Advanced Micro Devices, Inc., (unpublished)
##
##  All rights reserved.  This notice is intended as a precaution against
##  inadvertent publication and does not imply publication or any waiver
##  of confidentiality.  The year included in the foregoing notice is the
##  year of creation of the work.
##
*/


#include "sdcard_test.hpp"
#define ADMA64BIT  0x03
#include "resource_manager.h"
using namespace env;
using namespace std;
coreutil::Logger SDLogger::mLogger("SDLOG");

namespace
{
    char buffer[1024];
}

void sd_logmsg(const char *lpOutputString, ...)
{
    va_list args;
    va_start(args, lpOutputString);
    vsnprintf(buffer, sizeof(buffer), lpOutputString, args);
    va_end(args);

    SD_LOG_DEBUG() << buffer;
}

SDCardTest::SDCardTest(const TServerTestFactory& Factory) : ts::Test(Factory),m_lg("SDCardTest")
{
	device1 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device2 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device3 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device4 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device5 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device6 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device7 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device8 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device9 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device10 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device11 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device12 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device13 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device14 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device15 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device16 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device17 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device18 = env_dev.get_env_system(); //defined in sdcard_test.hpp
	device19 = env_dev.get_env_system(); //defined in sdcard_test.hpp
}

SDCardTest::~SDCardTest()
{
}

/*****************************************/
#if SD_PCI	
void SDCardTest::EnableMMIOBusMaster(uint32_t device)
{
	uint8_t value;
	
		value = read_pcicfg(device, SDHC_PCI_OR_MMIO_COMMAND , BYTE);
		value = value | 0x06;
		write_pcicfg(device, SDHC_PCI_OR_MMIO_COMMAND , BYTE ,value );
	
}
#endif
/*****************************************/


/*****************************************/
#if SD_MMIO	
void SDCardTest::EnableMMIOBusMaster()
{
	uint8_t value;
	
		value = sd_hd_read_mmio( SDHC_PCI_OR_MMIO_COMMAND , BYTE);
		value = value | 0x06;
		sd_hd_write_mmio(SDHC_PCI_OR_MMIO_COMMAND,value, BYTE  );
	
}
#endif
/*****************************************/


//bool SDCardTest::Initalization(uint32_t Device, uint32_t slot, bool voltage_switch_enabled, _init_status *init_status)
bool SDCardTest::Initalization(uint32_t Device, uint32_t slot, bool voltage_switch_enabled)
{
	if(!init_host(Device, slot))
	{
		SD_LOG_DEBUG() << "SD host Init test Failed for device" << Device << " and slot" << slot << std::endl;
		return false;
	}
	
	if(!init_card(Device, slot, voltage_switch_enabled, &init_status))
	{
		 SD_LOG_DEBUG()<< "SD Card Init new test Failed for device" << Device << " and slot" << slot << std::endl;
		return false;
	}
	
	 SD_LOG_DEBUG()<< "SD host Init new passed" << slot << std::endl;
	
	return true;
}

void SDCardTest::DMAMemoryAlloc()
{
	if (device1 != NULL)
	{
	 	//resource1 =  device1->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource1 =  device1->palloc(0x10000); 

	}
	if (device2 != NULL)
	{
	 	//resource2 =  device2->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource2 =  device2->palloc(0x10000);
	}
	if (device3 != NULL)
	{
	 	//resource3 =  device3->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource3 =  device3->palloc(0x10000);
	}
	if (device4 != NULL)
	{
	 	//resource4 =  device4->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource4 =  device4->palloc(0x10000);
	}
	if (device5 != NULL)
 	{
	 	//resource5 =  device5->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource5 =  device5->palloc(0x10000);
	}
	if (device6 != NULL)
	{
	 	//resource6 =  device6->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource6 =  device6->palloc(0x10000);
	}
	if (device7 != NULL)
	{
	 	//resource7 =  device7->palloc((0x10000),0,0xffffffff,0, env::CacheDefault);
		resource7 =  device7->palloc(0x10000);		
	}
	if (device8 != NULL)
	{
	 	//resource8 =  device8->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource8 =  device8->palloc(0x10000);
	}
	if (device9 != NULL)
	 	resource9 =  device9->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
	
	if (device10 != NULL)
	{
	 	//resource10 =  device10->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
		resource10 =  device10->palloc(0x10000);
 	}	
	if (device11 != NULL)
	{
	 	//resource11 =  device15->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
	 	//dma_table1 =  device11->palloc((0x15000),0,0xffffffff,0,env::CacheDefault); 
		resource11 =  device15->palloc(0x10000); 
	 	dma_table1 =  device11->palloc(0x15000); 
	}
	if (device12 != NULL)
	{
	 	//resource12 =  device16->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
	 	//dma_table2 =  device12->palloc((0x15000),0,0xffffffff,0,env::CacheDefault);
			
		resource12 =  device16->palloc(0x10000); 
	 	dma_table2 =  device12->palloc(0x15000);
	}
	if (device13 != NULL)
	{
	
	 	//resource13 =  device17->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
	 	//dma_table3 =  device13->palloc((0x15000),0,0xffffffff,0,env::CacheDefault); 
		
		resource13 =  device17->palloc(0x10000); 
	 	dma_table3 =  device13->palloc(0x15000);
	}
	if (device14 != NULL)
	{
	 	//resource14 =  device18->palloc((0x10000),0,0xffffffff,0, env::CacheDefault); 
	 	//dma_table4 =  device14->palloc((0x15000),0,0xffffffff,0,env::CacheDefault); 
		
		resource14 =  device18->palloc(0x10000); 
	 	dma_table4 =  device14->palloc(0x15000); 
	}
	if (device15 != NULL)
	{
	 	//resource15 =  device19->palloc(0x10000, 0, 0xffffffff, 0, env::CacheDefault); 
		resource15 =  device19->palloc(0x10000); 
	}



}

bool SDCardTest::SDMAReadWrite(uint32_t card_address, uint32_t memsize,uint32_t blk_size, uint32_t blk_count, bool Speed, bool BusWidth, uint8_t autocmd)
{
	uint32_t index =0x00;
	uint32_t  writtendata ; //= (uint32_t)resource1->base();
				SDCardTest::resource1->read(index,1,&writtendata);
				
	SD_LOG_DEBUG() << "writtendata:::::::" << std::hex << writtendata <<std::endl;
	uint32_t buffer_address[20] = {(uint32_t)resource1->base(),(uint32_t)resource2->base(),(uint32_t)resource3->base(),(uint32_t)resource4->base(),(uint32_t)resource5->base(),(uint32_t)resource6->base(),(uint32_t)resource7->base(),(uint32_t)resource8->base(),(uint32_t)resource9->base(),(uint32_t)resource10->base(),(uint32_t)resource11->base(),(uint32_t)resource12->base(),(uint32_t)resource13->base(),(uint32_t)resource14->base(),(uint32_t)resource15->base()};	
	//,(uint32_t)resource2->base(),(uint32_t)resource3->base(),(uint32_t)resource4->base(),(uint32_t)resource5->base(),(uint32_t)resource6->base(),(uint32_t)resource7->base(),(uint32_t)resource8->base(),(uint32_t)resource9->base(),(uint32_t)resource10->base(),(uint32_t)resource11->base(),(uint32_t)resource12->base(),(uint32_t)resource13->base(),(uint32_t)resource14->base(),(uint32_t)resource15->base()};	
	uint32_t buffer_index = 0x00;
	sdma_red_datatype datastruct[100];
	
	FillBufferSDMA();
	SDCardTest::resource1->read(index,1,&writtendata);
				
	SD_LOG_DEBUG() << "After FillBufferSDMA:: writtendata:::::::" << std::hex << writtendata <<std::endl;
	if(!sdma_write( 0, 0, card_address, &buffer_address[0]/*phy_address*/, memsize /*mem size*/, blk_size, blk_count, Speed, BusWidth, autocmd))
	{
		SD_LOG_DEBUG() << "SDMA Failed to block write...." << std::endl;
		return false;
	}
	
	
	
	ClearBuffer();
	if(!sdma_read( 0, 0, card_address, &buffer_address[0]/*phy_address*/, memsize /*mem size*/, blk_size, blk_count, Speed, BusWidth, autocmd , &datastruct[0]))
	{
		SD_LOG_DEBUG()<< "SDMA Failed to block Read...." << std::endl;
		return false;
	}
	
	uint64_t TotalDataLength =  (blk_size * (blk_count == 0 ? 1 : blk_count));
	uint64_t TotalCompared   = 0x00;
	//uint32_t MaxSize = 0x00;
	//uint32_t phy_array_index = 0x01;
	//uint32_t index =0x00;
	uint64_t total_legth = 0x00;
	env::Map* memptr1;
	env::System* device1 = env_dev.get_env_system();
	uint32_t *data_mem_ptr = NULL;
	uint32_t length_per_buffer = 0x00;
	uint32_t length_compared   = 0x00;
	SD_LOG_DEBUG() << "Data Byte requested  :"<<std::dec<<TotalDataLength <<std::endl;
	SD_LOG_DEBUG()<<"Data Byte compared   :      "<<std::dec<<TotalCompared <<std::endl;
	while(datastruct[index].address && datastruct[index].total_byte)
	{
		SD_LOG_DEBUG() << "Sdma address ->" << std::hex << datastruct[index].address <<std::endl;
		SD_LOG_DEBUG() << "Sdma data length -> " << std::hex <<datastruct[index].total_byte <<std::endl;
		total_legth += datastruct[index].total_byte; 		
		length_per_buffer = datastruct[index].total_byte;
		length_compared = 0x00;
		do{
			SD_LOG_DEBUG()<<"Address mapped :"<<std::hex << (datastruct[index].address + length_compared) <<std::endl;
			memptr1      = device1->pmap((void*)((uintptr_t)(datastruct[index].address + length_compared)),1);
			data_mem_ptr = (uint32_t*)memptr1->getVirtualAddress();
			
			if(length_per_buffer > 0x1000)
				length_per_buffer = 0x1000;
			
			for(buffer_index = 0; buffer_index < (length_per_buffer/sizeof(writtendata)); buffer_index++)
			{
				if(buffer_index)
					data_mem_ptr++;
				
				SD_LOG_DEBUG()<<"Before check:  data_mem_ptr" <<std::hex<< *data_mem_ptr <<std::endl;
				SD_LOG_DEBUG()<<"Before check:  writtendata" <<std::hex<<writtendata <<std::endl;
				
				
				if(*data_mem_ptr != writtendata)
				{
					//SD_LOG_DEBUG()<<"SDMA failed : offset %X , Red Data = %X , Written Data = %X \n", ((uint32_t)((datastruct[index].address + length_compared) + (buffer_index * sizeof(writtendata)))), ((uint32_t)*data_mem_ptr) , writtendata);
					SD_LOG_DEBUG()<<"SDMA failed : offset " << std::hex << ((uint32_t)((datastruct[index].address + length_compared) + (buffer_index * sizeof(writtendata)))) << "Red Data" <<std::hex << ((uint32_t)*data_mem_ptr) << "Written Data" <<std::hex << writtendata <<std::endl;
					SD_LOG_DEBUG()<<"Inside If-BLOCK:  Buffer Index" <<std::dec<<buffer_index <<std::endl;

					return false;
				}	
				SD_LOG_DEBUG()<<"Buffer Index" <<std::dec<<buffer_index <<std::endl;
			}
			//SD_LOG_DEBUG() << "Address pionting : %X, Data pionting : %X, Total Data compared : %X\n", ((uint32_t)((datastruct[index].address + length_compared) + (buffer_index * sizeof(writtendata)))), (uint32_t)*data_mem_ptr,(buffer_index * sizeof(writtendata)));
			
			SD_LOG_DEBUG() << "Address pionting :" <<std::hex << (((uint32_t)((datastruct[index].address + length_compared) + (buffer_index * sizeof(writtendata))))) <<std::hex<<"Data Pointing"<< (uint32_t)*data_mem_ptr << "Written Data" << std::hex<< ((buffer_index * sizeof(writtendata))) <<std::endl;
			length_compared += (buffer_index * sizeof(writtendata));
			length_per_buffer = datastruct[index].total_byte - length_compared;
			SD_LOG_DEBUG()<<std::dec <<(TotalCompared + length_compared) << std::endl;
		}while(length_compared < datastruct[index].total_byte);	
		TotalCompared += length_compared;
		
		SD_LOG_DEBUG()<<"Total Data compared : " << std::dec <<length_compared << std::endl;
		index++;
	}
		SD_LOG_DEBUG() <<std::endl;
		SD_LOG_DEBUG() << "Data length transfered -> " <<std::dec << total_legth <<"Bytes"<<std::endl;
		SD_LOG_DEBUG()<<"Data length compared   -> "<<std::dec<<TotalCompared << "Bytes" << std::endl;
	if((TotalDataLength != total_legth) || (TotalDataLength != TotalCompared))
		return false;
	
	return true;
}
void SDCardTest::ADMAFillTable_for_write(uint32_t block_count,uint8_t adma_type)
{
	uint32_t NO_OPERATION    		= 0x01;
	uint32_t TRANS_DATA      		= 0x21;
	uint32_t LINK_DESCRIPTOR 		= 0x31;
	uint32_t TRANS_END		 		= 0x23;
	uint32_t block_per_table 		= 0x00;
	uint32_t block_per_buffer 		= 0x00;
	uint32_t total_size_filled		= 0x00;
	bool large_size_data_transfer 	= false;
	uint32_t index 					= 0x00;
	uint32_t select_buffer 			= 0x00;
	uint32_t length_n_attribute = 0x00, address_field = 0x00 , table_wr_index = 0x00;
	uint32_t descriptor[5] = {(uint32_t)dma_table1->base() ,(uint32_t)dma_table2->base(),(uint32_t)dma_table3->base(),(uint32_t)dma_table4->base()};
	uint32_t buffer_address[6] 	= {(uint32_t)resource1->base(),(uint32_t)resource2->base(),(uint32_t)resource3->base(),(uint32_t)resource4->base(),(uint32_t)resource5->base()};	

	if(block_count == (uint32_t)data_size_singleblock)
	{
		length_n_attribute 	= ((uint32_t)((ONE_BLOCK)<< 16) | (NO_OPERATION));
		SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
		table_wr_index++;
		SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
		table_wr_index++;
		if(adma_type == ADMA64BIT)
		{
			address_field = 0x00;
			SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
			table_wr_index++;
		}
		
		address_field 		= (uint32_t)buffer_address[select_buffer];
		length_n_attribute 	= ((uint32_t)((ONE_BLOCK)<< 16) | (TRANS_END));
		
		SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
		table_wr_index++;
		SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
		table_wr_index++;
		if(adma_type == ADMA64BIT)
		{
			address_field = 0x00;
			SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
			table_wr_index++;
		}
	}

	else
	{
		block_per_table  = block_count/4;
		block_per_buffer = block_per_table/5;	
		SD_LOG_DEBUG()<<"block_per_table "<<std::dec<<block_per_table <<std::endl;
		SD_LOG_DEBUG()<<"block_per_buffer =" <<std::dec <<block_per_buffer <<std::endl;
		if(block_per_buffer > MAX_BLOCK_PER_ONE_TABLE_ENTRY)	
		{
			block_per_buffer = MAX_BLOCK_PER_ONE_TABLE_ENTRY;
			large_size_data_transfer = true;
		}	
        
		//Fill Table 1 , Table 2 , Table 3 , Table 4
		index = 0x00;
		while(index < block_per_table)
		{
				length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (NO_OPERATION));
				SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
				SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
				SDCardTest::dma_table3->write(table_wr_index,1,&length_n_attribute);
				SDCardTest::dma_table4->write(table_wr_index,1,&length_n_attribute);
				table_wr_index++;
				SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
				SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
				SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
				SDCardTest::dma_table4->write(table_wr_index,1,&address_field);				
				table_wr_index++;
				if(adma_type == ADMA64BIT)
				{
					address_field = 0x00;
					SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
					SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
					SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
					SDCardTest::dma_table4->write(table_wr_index,1,&address_field);				
					table_wr_index++;
				}
									
			
			address_field 		= (uint32_t)buffer_address[select_buffer];
			length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (TRANS_DATA));
			
			SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
			SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
			SDCardTest::dma_table3->write(table_wr_index,1,&length_n_attribute);
			SDCardTest::dma_table4->write(table_wr_index,1,&length_n_attribute);
			table_wr_index++;
			SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
			SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
			SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
			SDCardTest::dma_table4->write(table_wr_index,1,&address_field);				
			table_wr_index++;
			if(adma_type == ADMA64BIT)
			{
				address_field = 0x00;
				SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
				SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
				SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
				SDCardTest::dma_table4->write(table_wr_index,1,&address_field);	
				table_wr_index++;
			}
			
			total_size_filled = block_per_buffer;
			index += total_size_filled ;
        
			if(index <  (block_per_table - block_per_buffer))
			{
				if((table_wr_index % 4) == 0)
				{					
					length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (NO_OPERATION));
					SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
					SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
					SDCardTest::dma_table3->write(table_wr_index,1,&length_n_attribute);
					SDCardTest::dma_table4->write(table_wr_index,1,&length_n_attribute);
					table_wr_index++;
					SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
					SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
					SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
					SDCardTest::dma_table4->write(table_wr_index,1,&address_field);				
					table_wr_index++;	
					if(adma_type == ADMA64BIT)
					{
						address_field = 0x00;
						SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
						SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
						SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
						SDCardTest::dma_table4->write(table_wr_index,1,&address_field);	
						table_wr_index++;
					}
											
				}	
			}
			
			if(++select_buffer > 4)
			select_buffer = 0;
			
			
			if(large_size_data_transfer)
			{
				if((block_per_table - index) < MAX_BLOCK_PER_ONE_TABLE_ENTRY )
					block_per_buffer = (block_per_table - index);
			}
		}
		
		length_n_attribute  = ((length_n_attribute & 0xFFFF0000) | LINK_DESCRIPTOR);
        
		SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
		SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
		SDCardTest::dma_table3->write(table_wr_index,1,&length_n_attribute);
		table_wr_index++;
		SDCardTest::dma_table1->write(table_wr_index,1,(&descriptor[1]));
		SDCardTest::dma_table2->write(table_wr_index,1,(&descriptor[2]));
		SDCardTest::dma_table3->write(table_wr_index,1,(&descriptor[3]));
		table_wr_index++;
		if(adma_type == ADMA64BIT)
		{
			address_field = 0x00;
			SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
			SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
			SDCardTest::dma_table3->write(table_wr_index,1,&address_field);
			//SDCardTest::dma_table4->write(table_wr_index,1,&address_field);	
			table_wr_index++;
		}
		
		length_n_attribute  = ((length_n_attribute & 0xFFFF0000) | TRANS_END);			
		
		if(adma_type == ADMA64BIT)
			table_wr_index -= 6;
		else
			table_wr_index -= 4;
		SDCardTest::dma_table4->write(table_wr_index,1,&length_n_attribute);
	}
}


bool SDCardTest::ADMAFillTable_for_read(uint32_t block_count,uint8_t adma_type)
{
	uint32_t NO_OPERATION    = 0x01;
	uint32_t TRANS_DATA      = 0x21;
	uint32_t LINK_DESCRIPTOR = 0x31;
	uint32_t TRANS_END		 = 0x23;
	uint32_t block_per_table 		= 0x00;
	uint32_t block_per_buffer 		= 0x00;
	//bool large_size_data_transfer 	= false;
	uint32_t index 					= 0x00;
	uint32_t select_buffer 			= 0x00;
	uint32_t length_n_attribute = 0x00, address_field = 0x00 , table_wr_index = 0x00;
	uint32_t descriptor[5] = {(uint32_t)dma_table1->base() ,(uint32_t)dma_table2->base(),(uint32_t)dma_table3->base(),(uint32_t)dma_table4->base()};
	uint32_t buffer_address[11] 	= {(uint32_t)resource6->base(),(uint32_t)resource7->base(),(uint32_t)resource8->base(),(uint32_t)resource9->base(),(uint32_t)resource10->base(),(uint32_t)resource11->base(),(uint32_t)resource12->base(),(uint32_t)resource13->base(),(uint32_t)resource14->base(),(uint32_t)resource15->base()};	
	
	if(block_count == (uint32_t)data_size_singleblock)
	{
		length_n_attribute 	= ((uint32_t)((ONE_BLOCK)<< 16) | (NO_OPERATION));
		SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
		table_wr_index++;
		SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
		table_wr_index++;
		if(adma_type == ADMA64BIT)
		{
			address_field = 0x00;
			SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
			table_wr_index++;
		}
		
		address_field 		= (uint32_t)buffer_address[select_buffer];
		length_n_attribute 	= ((uint32_t)((ONE_BLOCK)<< 16) | (TRANS_END));
		
		SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
		table_wr_index++;
		SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
		table_wr_index++;
		if(adma_type == ADMA64BIT)
		{
			address_field = 0x00;
			SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
			table_wr_index++;
		}
	}

	else
	{
			block_per_table  = block_count/2;
			block_per_buffer = block_per_table/5;	
			sd_logmsg("block_per_table = %d\n",block_per_table);
			sd_logmsg("block_per_buffer = %d\n",block_per_buffer);
			if(block_per_buffer > MAX_BLOCK_PER_ONE_TABLE_ENTRY)	
			{
				return false;
			}	
				
			//Fill Table 1
			index = 0x00;
			while(index < 5)
			{
				if(select_buffer == 10)
					return false;

				length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (NO_OPERATION));
				SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
				table_wr_index++;
				SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
				table_wr_index++;
				if(adma_type == ADMA64BIT)
				{
					address_field = 0x00;
					SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
					table_wr_index++;
				}
				
				address_field 		= (uint32_t)buffer_address[select_buffer];
				length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (TRANS_DATA));
				
				SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
				table_wr_index++;
				SDCardTest::dma_table1->write(table_wr_index,1,&address_field);
				table_wr_index++;
				if(adma_type == ADMA64BIT)
				{
					address_field = 0x00;
					SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
					table_wr_index++;
				}
				
				select_buffer++;
				index++ ;
				
				if(index <  (block_per_table - block_per_buffer))
				{
					if((table_wr_index % 4) == 0)
					{
						length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (NO_OPERATION));
						SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
						table_wr_index++;
						SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
						table_wr_index++;
						if(adma_type == ADMA64BIT)
						{
							address_field = 0x00;
							SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
							table_wr_index++;
						}
					}
			    }
			}
		
			length_n_attribute  = ((length_n_attribute & 0xFFFF0000) | LINK_DESCRIPTOR);
			SDCardTest::dma_table1->write(table_wr_index,1,&length_n_attribute);
			table_wr_index++;
			SDCardTest::dma_table1->write(table_wr_index,1,(&descriptor[1]));
			table_wr_index++;
			if(adma_type == ADMA64BIT)
			{
				address_field = 0x00;
				SDCardTest::dma_table1->write(table_wr_index,1,(&address_field));
				table_wr_index++;
			}		
			
			//Fill Table 2				
			index = 0x00;
			table_wr_index = 0;
			
			while(index < 5)
			{
				if(select_buffer == 10)
					return false;
					
				address_field 		= (uint32_t)buffer_address[select_buffer];
				length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (TRANS_DATA));
				
				SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
				table_wr_index++;
				SDCardTest::dma_table2->write(table_wr_index,1,&address_field);
				table_wr_index++;
				if(adma_type == ADMA64BIT)
				{
					address_field = 0x00;
					SDCardTest::dma_table2->write(table_wr_index,1,(&address_field));
					table_wr_index++;
				}
				select_buffer++;
				index ++ ;
				if(index <  (block_per_table - block_per_buffer))
				{			
					if((table_wr_index % 4) == 0)
					{
						length_n_attribute 	= ((uint32_t)((block_per_buffer * ONE_BLOCK)<< 16) | (NO_OPERATION));
						SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
						table_wr_index++;
						SDCardTest::dma_table2->write(table_wr_index,1,(&address_field));
						table_wr_index++;
						if(adma_type == ADMA64BIT)
						{
							address_field = 0x00;
							SDCardTest::dma_table2->write(table_wr_index,1,(&address_field));
							table_wr_index++;
						}
					}
				}
			}			

			
			if(adma_type == ADMA64BIT)
				table_wr_index -= 3;
			else
				table_wr_index -= 2;
			length_n_attribute  = ((length_n_attribute & 0xFFFF0000) | TRANS_END);			
			SDCardTest::dma_table2->write(table_wr_index,1,&length_n_attribute);
	}
	return true;
}
void  SDCardTest::ClearBuffer()
{
	uint32_t index = 0x00;
	uint32_t data  = 0x00;
	//Fill the physical memory buffer with data
	try {
	for(index = 0 ; index < (0x10000/(sizeof(data))) ;  index++)
	{
		SDCardTest::resource1->write(index,1,&data);
		SDCardTest::resource2->write(index,1,&data);
		SDCardTest::resource3->write(index,1,&data);
		SDCardTest::resource4->write(index,1,&data);
		SDCardTest::resource5->write(index,1,&data);
		SDCardTest::resource6->write(index,1,&data);
		SDCardTest::resource7->write(index,1,&data);
		SDCardTest::resource8->write(index,1,&data);
		SDCardTest::resource9->write(index,1,&data);
		SDCardTest::resource10->write(index,1,&data);
		SDCardTest::resource11->write(index,1,&data);
		SDCardTest::resource12->write(index,1,&data);
		SDCardTest::resource13->write(index,1,&data);
		SDCardTest::resource14->write(index,1,&data);
		SDCardTest::resource15->write(index,1,&data);
	}
	}
	catch (...) {
		 SD_LOG_DEBUG() << "SDCARD ClearBuffer failed 1st for loop Index=: " << std::hex << index <<std::endl;	
	}
    
	try {
	for(index = 0 ; index < (0x15000/(sizeof(data))) ;  index++)
	{
		SDCardTest::dma_table1->write(index,1,&data);
		SDCardTest::dma_table2->write(index,1,&data);
		SDCardTest::dma_table3->write(index,1,&data);
		SDCardTest::dma_table4->write(index,1,&data);
	}
	}
	catch (...) {
		 //SD_LOG_DEBUG() << "SDCARD ClearBuffer failed 2nd for loop Index=: " << std::hex << index <<std::endl;	
		 SD_LOG_DEBUG() << "SDCARD ClearBuffer failed 2nd for loop Index=: " << std::hex << index <<std::endl;	
	}
    
}

void SDCardTest::FillBufferSDMA()
{
	//uint32_t data = (uint32_t)resource1->base();
	uint32_t data = (uint32_t)0x5A5A5A5A;
	uint32_t index = 0x00;

	//size = (0x1000/(sizeof(data)))-1;

	//Fill the physical memory buffer with data
	for(index = 0 ; index < ((0x10000/(sizeof(data)))) ;  index++)
	{
		SDCardTest::resource1->write(index,1,&data);
		SDCardTest::resource2->write(index,1,&data);
		SDCardTest::resource3->write(index,1,&data);
		SDCardTest::resource4->write(index,1,&data);
		SDCardTest::resource5->write(index,1,&data);
		SDCardTest::resource6->write(index,1,&data);
		SDCardTest::resource7->write(index,1,&data);
		SDCardTest::resource8->write(index,1,&data);
		SDCardTest::resource9->write(index,1,&data);
		SDCardTest::dma_table1->write(index,1,&data);
		SDCardTest::dma_table2->write(index,1,&data);
		SDCardTest::dma_table3->write(index,1,&data);
		SDCardTest::dma_table4->write(index,1,&data);
		SDCardTest::resource10->write(index,1,&data);
		SDCardTest::resource11->write(index,1,&data);
		SDCardTest::resource12->write(index,1,&data);
		SDCardTest::resource13->write(index,1,&data);
		SDCardTest::resource14->write(index,1,&data);
		SDCardTest::resource15->write(index,1,&data);
	}
	#if 0
	catch(...) {
		 //SD_LOG_DEBUG() << "SDCARD DMA write failed Index=: " << std::hex << index <<std::endl;	
		 SD_LOG_DEBUG() << "SDCARD DMA write failed Index=: " << std::hex << index <<std::endl;	
	}
	#endif			
	#if 0
	sd_logmsg("Table 1 : %X\n",SDCardTest::dma_table1->base());
	sd_logmsg("Table 2 : %X\n",SDCardTest::dma_table2->base());
	sd_logmsg("Table 3 : %X\n",SDCardTest::dma_table3->base());
	sd_logmsg("Table 4 : %X\n",SDCardTest::dma_table4->base());
	sd_logmsg("Buffer 1 : %X\n",SDCardTest::resource1->base());
	sd_logmsg("Buffer 2 : %X\n",SDCardTest::resource2->base());
	sd_logmsg("Buffer 3 : %X\n",SDCardTest::resource3->base());
	sd_logmsg("Buffer 4 : %X\n",SDCardTest::resource4->base());
	sd_logmsg("Buffer 5 : %X\n",SDCardTest::resource5->base());
	sd_logmsg("Buffer 6 : %X\n",SDCardTest::resource6->base());
	sd_logmsg("Buffer 7 : %X\n",SDCardTest::resource7->base());
	sd_logmsg("Buffer 8 : %X\n",SDCardTest::resource8->base());
	sd_logmsg("Buffer 9 : %X\n",SDCardTest::resource9->base());
	sd_logmsg("Buffer 10 : %X\n",SDCardTest::resource10->base());
	sd_logmsg("Buffer 11 : %X\n",SDCardTest::resource11->base());
	sd_logmsg("Buffer 12 : %X\n",SDCardTest::resource12->base());
	sd_logmsg("Buffer 13 : %X\n",SDCardTest::resource13->base());
	sd_logmsg("Buffer 14 : %X\n",SDCardTest::resource14->base());
	sd_logmsg("Buffer 15 : %X\n",SDCardTest::resource15->base());

	#endif
}



void SDCardTest::FillBufferADMA()
{
		uint32_t data1 = (uint32_t)resource1->base();
	//	uint32_t data1 = (uint32_t)0x5A5A5A5A;
		uint32_t index = 0x00;
		uint32_t data = 0x00;
		//Fill the physical memory buffer with data
		for(index = 0 ; index < (0x10000/(sizeof(data1))) ;  index++)
		{
			SDCardTest::resource1->write(index,1,&data1);
			SDCardTest::resource2->write(index,1,&data1);
			SDCardTest::resource3->write(index,1,&data1);
			SDCardTest::resource4->write(index,1,&data1);
			SDCardTest::resource5->write(index,1,&data1);
			SDCardTest::resource6->write(index,1,&data);
			SDCardTest::resource7->write(index,1,&data);
			SDCardTest::resource8->write(index,1,&data);
			SDCardTest::resource9->write(index,1,&data);
			SDCardTest::resource10->write(index,1,&data);
			SDCardTest::resource11->write(index,1,&data);
			SDCardTest::resource12->write(index,1,&data);
			SDCardTest::resource13->write(index,1,&data);
			SDCardTest::resource14->write(index,1,&data);
			SDCardTest::resource15->write(index,1,&data);
		}

		for(index = 0 ; index < (0x15000/(sizeof(data1))) ;  index++)
		{
			SDCardTest::dma_table1->write(index,1,&data1);
			SDCardTest::dma_table2->write(index,1,&data1);
			SDCardTest::dma_table3->write(index,1,&data1);
			SDCardTest::dma_table4->write(index,1,&data1);
		}



		
		
}

bool SDCardTest::ADMA2ReadWrite(uint32_t device, uint32_t slot, uint32_t card_address, data_size size,bool Speed, bool BusWidth, uint8_t autocmd, uint8_t adma_type)
{
	uint32_t blk_count = size;
	uint32_t split_read = size;
	uint32_t index =0x00;
	bool single_block = false;
	//Fill the buffer
	if(size == data_size_singleblock)
		single_block = true;
	FillBufferADMA();
	//sd_logmsg("BLOCK COUNT = %d\n",blk_count);
	//Fill descriptor table entry
	ADMAFillTable_for_write(blk_count,adma_type);

	
	if(!dma2_write(device , slot , card_address ,(uint32_t) dma_table1->base() , 0x200,(blk_count == 1 ? 0 : blk_count), Speed, BusWidth, autocmd, adma_type))
	{
		//SD_LOG_DEBUG() << "ADMA write failed : " << std::endl; 
		SD_LOG_DEBUG() << "ADMA write failed : " << std::endl; 
		return false;
	}
	if(blk_count > 1280)
		split_read = 1280;	
	do
	{
		if(!ADMAFillTable_for_read(split_read,adma_type))
			return false;
		if(!dma2_read(device , slot , card_address , (uint32_t) dma_table1->base()  , 0x200, (single_block == true ? 0 : split_read), Speed, BusWidth, autocmd, adma_type))
		{
			SD_LOG_DEBUG() << "ADMA read failed : "<<std::endl; 
			return false;
		}
		 SD_LOG_DEBUG()<< "ADMA read PASSED : "<<std::endl; 
		
		uint32_t Red_data1 = 0x00,Red_data2 = 0x00,Red_data3 = 0x00,Red_data4 = 0x00,Red_data5 = 0x00,Red_data6 = 0x00,Red_data7 = 0x00, Red_data8 = 0x00,Red_data9 = 0x00,Red_data10 = 0x00;
		//uint32_t Written_data = (uint32_t)resource1->base();

		uint32_t Written_data ; 
		uint32_t block_per_table  = split_read/2;
		uint32_t block_per_buffer = block_per_table/5;		
		
		SDCardTest::resource1->read(index,1,&Written_data);
		 SD_LOG_DEBUG()<< "WRITTEN DATA: "<<std::hex<< Written_data<<std::endl; 

		if(single_block == true )
		{
			for(index = 0 ; index < ((ONE_BLOCK)/4) ; index++  )
			{
				SDCardTest::resource6->read(index,1,&Red_data1);
				if((Red_data1 != Written_data))
				{
					SD_LOG_DEBUG()<<"Read Write Failed index         : " << std::dec <<index << std::endl;
					SD_LOG_DEBUG()<<"Written Value                   : " <<std::hex<<Written_data<<std::endl;
					SD_LOG_DEBUG() <<"Red Value                       : " << std::hex << Red_data1;

					
					return false;
				}
			}
		}
		else
		{
			for(index = 0 ; index < ((block_per_buffer * ONE_BLOCK)/4) ; index++  )
			{
				SDCardTest::resource6->read(index,1,&Red_data1);
				SDCardTest::resource7->read(index,1,&Red_data2);
				SDCardTest::resource8->read(index,1,&Red_data3);
				SDCardTest::resource9->read(index,1,&Red_data4);
				SDCardTest::resource10->read(index,1,&Red_data5);
				SDCardTest::resource11->read(index,1,&Red_data6);
				SDCardTest::resource12->read(index,1,&Red_data7);
				SDCardTest::resource13->read(index,1,&Red_data8);
				SDCardTest::resource14->read(index,1,&Red_data9);
				SDCardTest::resource15->read(index,1,&Red_data10);
				if((Red_data1 != Written_data) || (Red_data2 != Written_data)|| (Red_data3 != Written_data)|| (Red_data4 != Written_data)|| (Red_data5 != Written_data)|| (Red_data6 != Written_data)|| (Red_data7 != Written_data)|| (Red_data8 != Written_data)|| (Red_data9 != Written_data)|| (Red_data10 != Written_data))
				{
					//sd_logmsg("Read Write Failed index         : %d\n",index);
					SD_LOG_DEBUG() << "Read Write Failed index         : " << std::dec << index <<std::endl;
					SD_LOG_DEBUG() << "Written Value                   : " << std::hex <<Written_data <<std::endl;
					//sd_logmsg("Red Value                       : %X - %X - %X - %X - %X - %X - %X - %X - %X - %X \n",Red_data1,Red_data2,Red_data3,Red_data4,Red_data5,Red_data6,Red_data7,Red_data8,Red_data9,Red_data10);
					SD_LOG_DEBUG()<<"Red Value"<<std::hex<<Red_data1<<"-"<<std::hex<<Red_data2<<"-"<<std::hex<<Red_data3<<"-"<<std::hex<<Red_data4<<"-"<<std::hex<<Red_data5 <<"-"<<std::hex<< Red_data6<<"-"<<std::hex<<Red_data7<<"-"<<std::hex<<Red_data8<<"-"<<std::hex<<Red_data9<<"-"<<std::hex<<Red_data10 <<std::endl;

					return false;
				}
			}
		}
		
		blk_count -= split_read;
		if(init_status.card.sdhc_sdxc == true)
			card_address += split_read;
		else
			card_address += (split_read * ONE_BLOCK);
		if(blk_count > 1280)
			split_read = 1280;
		else
			split_read = blk_count;
	
	}while(blk_count);		
	return true;
}


bool SDCardTest::ADMA2Write_3_4_16(uint32_t device, uint32_t slot, uint32_t card_address, data_size size,bool Speed, bool BusWidth, uint8_t autocmd, uint8_t adma_type, uint8_t* LockData)
{
	uint32_t blk_count = (uint32_t)size;
	//uint32_t split_read = (uint32_t)size;
	//uint32_t index =0x00;
	//bool single_block = false;
	//Fill the buffer
	//if(size == data_size_singleblock)
		//single_block = true;
	FillBufferADMA();
	//printf("BLOCK COUNT = %d\n",blk_count);
	//Fill descriptor table entry
	ADMAFillTable_for_write(blk_count,adma_type);
#if 1
	if(!dma2_write_3_4_16(device , slot , card_address , (uint64_t)dma_table1->base() , (uint32_t)0x200,(uint64_t)(blk_count == 1 ? 0 : blk_count), Speed, BusWidth, autocmd, adma_type,  LockData))
	{
		SD_LOG_DEBUG() << "ADMA write failed : "<<std::endl; 
		return false;
	}
#endif	
	return true;
}

bool SDCardTest::ADMA2ReadWrite_with_asyncabort(uint32_t card_address,  uint32_t blk_size, uint32_t blk_count, bool Speed, bool BusWidth, uint8_t autocmd)
{
	//uint32_t blk_count = size;
	//uint32_t split_read = blk_count;
	//uint32_t index =0x00;
	FillBufferADMA();
	ADMAFillTable_for_write(blk_count,0x02);
	
	if(!dma2_write_with_asynchabort(0 , 0 , card_address , (uint32_t) dma_table1->base() , blk_size, blk_count, Speed, BusWidth, autocmd, 0x02))
	{
		 //SD_LOG_DEBUG()<< "ADMA write failed : "<<std::endl; 
		 SD_LOG_DEBUG()<< "ADMA write failed : "<<std::endl; 
		return false;
	}
	return true;
}

bool SDCardTest::ADMA2ReadWrite_with_block_gap(uint32_t card_address,  uint32_t blk_size, uint32_t blk_count, bool Speed, bool BusWidth, uint8_t autocmd)
{
	//uint32_t split_read = blk_count;
	//uint32_t index =0x00;
	FillBufferADMA();
	ADMAFillTable_for_write(blk_count,0x02);

	if(!dma2_write_with_block_gap(0 , 0 , card_address ,(uint32_t) dma_table1->base() , blk_size, blk_count, Speed, BusWidth, autocmd, 0x02))
	{
		 //SD_LOG_DEBUG()<< "ADMA write failed : "<<std::endl; 
		 SD_LOG_DEBUG()<< "ADMA write failed : "<<std::endl; 
		return false;
	}
	return true;

#if 0
		uint64_t table_data = 0x0000000000000000;
		uint32_t rhs = 0x00, lhs = 0x00, i; 
		uint32_t check = 0, ReadData = 0;
		uint32_t temp = 0;
	
		if(!blk_count)
		{
			for(int k=0; k<(blk_size/sizeof(check)); k++)
			{
				SDCardTest::resource1->write(k,1,&Data);
			}
		}
		else
		{
			for(int k=0;k<(blk_size*blk_count * 2/sizeof(check)); k++)
			{
				SDCardTest::resource3->write(k,1,&check);
			}
			SDCardTest::resource3->read(0,1,&temp);
			sd_logmsg("Resource3: 0x%x\n", temp);
		
			for(int k=0; k<((blk_size*blk_count)/(sizeof(check))); k++)
			{
				SDCardTest::resource1->write(k,1,&Data);
			}
			SDCardTest::resource1->read(0,1,&temp);
			sd_logmsg("Resource1: 0x%x\n", temp);
			
			check = ((Data & 0xFFFF0000)>>16) | ((Data & 0x0000FFFF)<<16);
			for(int k=0; k<((blk_size*blk_count)/(sizeof(check))); k++)
			{
				SDCardTest::resource2->write(k,1,&check);
			}
			SDCardTest::resource2->read(0,1,&temp);
			sd_logmsg("Resource2: 0x%x 0x%x\n", temp, check);
		}
		
	
		sd_logmsg("\n----ADMA----\n");
		
		table_data =(uint64_t) (((uint64_t)(SDCardTest::resource1->base() << 32) )| (uint64_t)((blk_size*blk_count) << 16) | (uint64_t)(0x21));
		lhs = (uint32_t)((table_data & 0xffffffff00000000) >> 32);
		rhs = (uint32_t)((table_data & 0x00000000ffffffff));
		SDCardTest::dma_table1->write(0,1,&rhs);
		SDCardTest::dma_table1->write(1,1,&lhs);
		SD_LOG_DEBUG()<< "Table 1 : "<< std::hex << table_data <<std::endl;
		table_data = (uint64_t)(((uint64_t)(SDCardTest::resource2->base() << 32)) | (uint64_t)((blk_size*blk_count) << 16) | (uint64_t)(0x23));
		lhs = (uint32_t)((table_data & 0xffffffff00000000) >> 32);
		rhs = (uint32_t)((table_data & 0x00000000ffffffff));
		SD_LOG_DEBUG()<< "Table 1 : "<< std::hex << table_data <<std::endl;
		SDCardTest::dma_table1->write(2,1,&rhs);
		SDCardTest::dma_table1->write(3,1,&lhs);

		card_address = 0x5000;

		if(!dma2_write_with_block_gap(0 , 0 , card_address , SDCardTest::dma_table1->base() , blk_size, (blk_count*2), Speed, BusWidth, autocmd, 0x02))
		{
			 SD_LOG_DEBUG()<< "ADMA write failed : "<<std::endl; 
			return false;
		}
	return true;
#endif	
}
/************************************************************/


	/************************************************************/
	bool SDCardTest::pio_data_transfer(uint32_t device , uint32_t slot ,bool speed, bool width, uint8_t auto_cmd ,uint32_t card_address,uint32_t data, uint32_t block_size, uint32_t block_count) 
	{ 
		uint32_t size = (block_size * (block_count == 0 ? 1 : block_count)), index = 0x00, data_index = 0x00; 
		uint8_t *write_buffer, *read_buffer; 
		write_buffer    = new uint8_t[size]; 
		read_buffer     = new uint8_t[size]; 
		memset(write_buffer, 0 , size); 
		memset(read_buffer , 0x0 , size); 
		uint8_t array[5] = {((uint8_t)(data & 0x000000FF)), ((uint8_t)((data & 0x0000FF00) >> 8)) , ((uint8_t)((data & 0x00FF0000) >> 16)) , ((uint8_t)((data & 0xFF000000) >> 24))}; 
		//sd_logmsg("Index : %x\n",(uint16_t)size); 
		 
		for(index = 0 ; index < size ; index++) 
		{ 
			write_buffer[index] = array[data_index]; 
			if(++data_index == 4) 
			data_index = 0; 
		} 
		 
		//writing the data       
		if(!pio_write( device, slot, speed, width, auto_cmd, card_address, write_buffer, block_size, block_count)) 
		{
			sd_logmsg("Failed in pio_write\n");
			return false; 
		}
		//Reading the data 
		 if(!pio_read( device, slot, speed, width, auto_cmd, card_address, read_buffer, block_size, block_count)) 
		{
			sd_logmsg("Failed in pio_read\n");
			return false; 
		}

		 for(index = 0 ; index < size ; index++) 
		 { 
			 if(write_buffer[index] != read_buffer[index]) 
			{
				
				sd_logmsg("Failed Compare: pio_data_transfer, index %x, write_buffer[index] %x, read_buffer[index] %x\n", index, write_buffer[index], read_buffer[index]);
				 return false; 
			}
		 }    
		 return true; 
	}
	/************************************************************/


	/************************************************************/
	#if SD_PCI
	bool SDCardTest::enable_device(uint32_t device, bool status)
	{
		char command[30];
		_pci_type0_header pci_header;
		get_pciheader(&pci_header , device);

		sprintf(command,"devcon %s *dev_%x",(status == true ? "enable" : "disable"),pci_header.device_id);
		sd_logmsg("%s Device : %s\n",(status == true ? "enabling" : "disabling"),command);
		return (!system(command));
	}
	#endif
	/************************************************************/
	
	
	
	
	/************************************************************/
	void SDCardTest::PreRun()
	{
		
		ts::Test::PreRun();
		sd_logmsg("ENTRY TO SDCARD\n");
		bool status = false;
		
			if (init_driver((uint32_t)0,(uint32_t)0x14,(uint32_t)0x7))  // made common for PCI space and MMIO space, though, bus device funtion are not appicable for MMIO space
		#if SD_PCI
		uint32_t device = 0,Maxdevice=0;
			env::SleepMs(100);
			Maxdevice = get_total_pcidevice();
			sd_logmsg("Maxdevice %d  : \n",Maxdevice);
			for(int device=0; device<Maxdevice; device++)
			{
			
				sd_logmsg("Prerun Inside for loop...........\n");
				sd_logmsg("enable_device %d Device : \n",device);
				enable_device(device, false);
				SDCardTest::EnableMMIOBusMaster(device);
			}
			env::SleepMs(100);
		#elif SD_MMIO
			 status = SdGetVirtualAdress(); // get virtual addresses for both CFG and HostCntrl reg space
			if(status==false)
				sd_logmsg(" Failed in getting virtual Address for CFG and Host contoler Register\n");
			
			status = false;
			
			// this enables the IOMUX shared pins , to eMMC,CMD, Data lines.
			status = Enable_IO_Mux_AM4_ZP_RV_SD_MMIO(); 
			if(status==false)
				sd_logmsg(" Failed in getting virtual Address for CFG and Host contoler Register\n");
					
			sd_logmsg("Prerun Inside ..........\n");
			
			SDCardTest::EnableMMIOBusMaster();
		
		#endif
		
		env::SleepMs(100);
		SDCardTest::DMAMemoryAlloc();
		 
	}
	/************************************************************/


	
	/************************************************************/
	void SDCardTest::PostRun()
	{
		
		// Call base class version
		ts::Test::PostRun();
		#if SD_PCI
			uint32_t device = 0, Maxdevice = 0;
			Maxdevice = get_total_pcidevice();
			#if 1
			for(int device=0; device<Maxdevice; device++)
			{
				sd_logmsg("Postrun Inside for loop...........\n");
				sd_logmsg("enable_device %s Device : \n",device);
				enable_device(device, true);
			}
			#endif
		#endif
	}
	/************************************************************/
	
	
