/*
/*
##
##  Trade secret of Advanced Micro Devices, Inc.
##  Copyright 2008, Advanced Micro Devices, Inc., (unpublished)
##
##  All rights reserved.  This notice is intended as a precaution against
##  inadvertent publication and does not imply publication or any waiver
##  of confidentiality.  The year included in the foregoing notice is the
##  year of creation of the work.
##
*/
#ifndef _SD_TEST_
#define _SD_TEST_
#include <iostream>
//#include <ts_test.h>  // not in current code

#include "host.h"
//#include "sdcard_test.hpp"
//#include "sdcard.hpp"
//#include "fat32lib.hpp"
#include "host_types.h"

#if SD_PCI
	#include "env_Pci.h"
	#include "sd_pci.h"
#endif

#define TOTAL_BLOCK_SIZE 0x1600
#define BLOCK_LENGTH 0x200
#if 0
bool find_device(unsigned short bus1, unsigned short device1, unsigned short function1)
{
	bool status = false;
	try
	{
	    class env::sys::Device* system = new class env::sys::Device;
	    //env::pci::Client* client = new env::pci::Client((&*system->pci().device(env::pci::Location(0,bus1,device1,function1))));

		std::cout << "Device Found Bus : "<< std::hex << bus1 <<" Device :"<< std::hex << device1 <<" Function :"<< std::hex << function1 << std::endl ;
		status = true;
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "Exception for Bus :"<< std::hex << bus1 <<" Device :"<< std::hex << device1 <<" Function :"<< std::hex << function1 << "--> "<<e.what() << std::endl ;
		status = false;
		delete system;
		//delete client;
		//continue;
	}
	return status;
    
}
#endif



bool sdma_transfer(uint32_t Card_add, uint32_t data)
{
uint32_t check = 0x00;
uint32_t i = 0x00;
							env::sys::Device device1,device2,device3,device4,device5,device6;
							env::dma::Resource* resource1;
							env::dma::Resource* resource2;
							env::dma::Resource* resource3;
							env::dma::Resource* resource4;
							env::dma::Resource* resource5;
							env::dma::Resource* resource6;
							 resource1 = device1.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource1->base());
							 
							 resource2 = device2.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource2->base());
							 
							 resource3 = device3.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource3->base());
							 
							 resource4 = device4.palloc(0x6000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource4->base());

 //uint8_t check_data = 0x5A;
//resource1->write(0x2000,1,&check_data);							 
// resource->mPtr = 0x10;
//resource1->mBase
							for(i=0;i<0x800; i= i + 1)
							 {
							 check = data;
							 resource1->write(i,1,&check);
							 }

							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0xabcdef5b;
							 resource2->write(i,1,&check);
							 }	

							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0x5a5a5a5a;
							 resource3->write(i,1,&check);
							 }
							 
 std::cout << "Base address : "<<std::hex << resource1->base() <<std::endl;
 uint32_t write_address[4] = {resource1->base() , resource2->base() , resource3->base()};
 uint32_t read_address[2] =  {resource4->base()};

 if(!sdma_write( 0 ,  0 , Card_add /*card_address*/ , &write_address[0]/*phy_address*/ , 7 /*mem size*/, 0x200 /*blk_size*/, 16 /*blk_count*/, false, true, 0x01))
    std::cout << "sdma failed : "<<std::endl;
 
  if(!sdma_read( 0 ,  0 , Card_add /*card_address*/ , &read_address[0]/*phy_address*/ , 7 /*mem size*/, 0x200 /*blk_size*/, 16 /*blk_count*/, false, true, 0x01))
    std::cout << "sdma failed : "<<std::endl;
	else{
uint32_t data_read;
	int j = 0;
							for(i=0;i<(0x200 * 4); i= i + 1)
							 {
							  resource4->read(i,1,&data_read);
							#if 0							 
							 printf("DATA [%d] = %X",i,(uint16_t)data_read);
							  printf("\t\t"); 
							  if(j++ == 10)
							  {
								printf("\n");
								j=0;
								
							 }	
							#endif
							if(data_read != 0xdeadbeef)
							  printf( "Data Transfer Failed \n");
							 }
							}
return true;
}

class SDTest : public ts::Test
{
  public:
 SDTest(const TServerTestFactory& Factory) : ts::Test(Factory)
 {
 }

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun()
	{
		    ts::Test::PreRun();
	}

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun()
	{
		    ts::Test::PostRun();
	}

	virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		//_pci_node node1, *ptrnode;
		// ptrnode = &node1;
		//    SDCARD sdcard(mTCoreUserContext, 0); 
		bool result = false;
        SD_LOG_DEBUG() << "SD Sample Test....." << std::endl;
uint32_t check;
uint32_t i;
uint8_t chk;


#if 0
							env::sys::Device system3;
							env::pci::Client client3((*system3.pci().device(env::pci::Location(0,0,20,7))));
							env::sys::Device device1;
							env::dma::Resource* resource1,resource2;
							 // env::pci::Client client3((*system3.pci().device(env::pci::Location(0,0,17,0))));
							 resource1 = device1.palloc(0x8000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X",resource1->base());
//uint8_t check_data = 0x5A;
//resource1->write(0x2000,1,&check_data);							 
							// resource->mPtr = 0x10;
							for(i=0;i<0x1000; i= i + 1)
							 {
							 check = 0xdeadbeef;
							 resource1->write(i,1,&check);
							 }
							 
							 for(i=0x4000;i<0x8000; i= i + 1)
							 {
							 chk = 0xde;
							 resource1->write(i,1,&chk);
							 }
#if 0
							 resource1 = device1.palloc(0x1000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X",resource1->base());
#endif
							 
//utilSleep(15000);  							 
							 //check = 0xCDCDCDCD;
							 //resource->write(0,1,&check);

uint32_t data;
for(i=0;i<10;++i)
{
	data = client3.cfgr32(i);
	std::cout << "CFG R  : " << std::dec << i*4 << " = " << std::hex << std::showbase << data << std::endl;

}

	for(i=0;i<=0xFC;i=i+4)
	{
		data = client3.regr32(i);
		std::cout << "Reg MMIO R  : " << std::dec << i << " = " << std::hex << std::showbase << data << std::endl;
	}


	data = client3.regr8(0x29);
	std::cout << "Reg MMIO R test : " << std::dec << "0x29" << " = " << std::hex << std::showbase << data << std::endl;

	data = 0x00;
	client3.regw8(0x29,0x0F);
	data = client3.regr8(0x29);
	std::cout << "Reg MMIO R  should be equal to  0x0f: " << std::dec << "0x0F" << " = " << std::hex << std::showbase << data << std::endl;

#endif



result = init_driver();
_pci_type0_header *p_header = new _pci_type0_header;



/************* Disable driver and cnfig CFG *****************/
//int result1 = system ("devcon disable *dev_7813");
int result1 = system ("devcon disable *dev_7906");
_reg* sd_pci_register_def = new _reg;
sd_pci_register_def->pci_reg.value		= 0x00;
sd_pci_register_def->pci_reg.offset		= 0x04;
sd_pci_register_def->pci_reg.width		= 0x01;
sd_pci_register_def->pci_reg.device		= 0x00;

result = read_pcicfg(sd_pci_register_def);
reg8 command;
command.reg_val = sd_pci_register_def->pci_reg.value;
command.bit.b1 = 1;
command.bit.b2 = 1;
sd_pci_register_def->pci_reg.value		= command.reg_val;
result = write_pcicfg(sd_pci_register_def);
/************* Disable driver and cnfig CFG *****************/
if(init_host(0,0))
{
SD_LOG_DEBUG() << "Host init pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "Host init  Fail"  <<std::endl;
}

_init_status init_status;
if(init_card(0,0,&init_status))
{
SD_LOG_DEBUG() << "Card init pass" <<std::endl;

SD_LOG_DEBUG() << "-------------------------------Host---------------------------" <<std::endl;
SD_LOG_DEBUG() << "version : " <<std::hex<<(uint16_t)init_status.host.version<<std::endl;
SD_LOG_DEBUG() << "voltage_switch : " <<std::hex<<((init_status.host.voltage_switch == true) ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "ddr50 : " <<std::hex<<(init_status.host.ddr50 == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "sdr12 : " <<std::hex<<(init_status.host.sdr12 == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "sdr25 : " <<std::hex<<(init_status.host.sdr25 == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "sdr50 : " <<std::hex<<(init_status.host.sdr50 == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "sdr104 : " <<std::hex<<(init_status.host.sdr104 == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "hs : " <<std::hex<<(init_status.host.hs == true ? 1 : 0)<<std::endl;
	


SD_LOG_DEBUG() << "-------------------------------Card---------------------------" <<std::endl;
SD_LOG_DEBUG() << "voltage_switch : " <<std::hex<<(init_status.card.voltage_switch == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "sdhc_sdxc : " <<std::hex<<(init_status.card.sdhc_sdxc == true ? 1 : 0)<<std::endl;
SD_LOG_DEBUG() << "rca : " <<std::hex<<(init_status.card.rca)<<std::endl;
SD_LOG_DEBUG() << "------------ocr------------" <<std::endl;
		SD_LOG_DEBUG() <<" rsvd1:7;"									 << (uint16_t)init_status.card.mem_ocr.bit.rsvd1	        << std::endl;
		SD_LOG_DEBUG() <<" low_voltage:1;"							 << (uint16_t)init_status.card.mem_ocr.bit.low_voltage	<< std::endl;
		SD_LOG_DEBUG() <<" rsvd2:7;"									 << (uint16_t)init_status.card.mem_ocr.bit.rsvd2	        << std::endl;
		SD_LOG_DEBUG() <<" v2_7v_to_v2_8:1;"							 << (uint16_t)init_status.card.mem_ocr.bit.v2_7v_to_v2_8	<< std::endl;
		SD_LOG_DEBUG() <<" v2_8v_to_v2_9:1;"   						 << (uint16_t)init_status.card.mem_ocr.bit.v2_8v_to_v2_9	<< std::endl;
		SD_LOG_DEBUG() <<" v2_9v_to_v3_0:1;"   						 << (uint16_t)init_status.card.mem_ocr.bit.v2_9v_to_v3_0	<< std::endl;
		SD_LOG_DEBUG() <<" v3_0v_to_v3_1:1;"   						 << (uint16_t)init_status.card.mem_ocr.bit.v3_0v_to_v3_1	<< std::endl;
		SD_LOG_DEBUG() <<" v3_1v_to_v3_2:1;"							 << (uint16_t)init_status.card.mem_ocr.bit.v3_1v_to_v3_2	<< std::endl;
 		SD_LOG_DEBUG() <<" v3_2v_to_v3_3:1;"							 << (uint16_t)init_status.card.mem_ocr.bit.v3_2v_to_v3_3	<< std::endl;  
		SD_LOG_DEBUG() <<" v3_3v_to_v3_4:1;"							 << (uint16_t)init_status.card.mem_ocr.bit.v3_3v_to_v3_4	<< std::endl;
		SD_LOG_DEBUG() <<" v3_4v_to_v3_5:1;"							 << (uint16_t)init_status.card.mem_ocr.bit.v3_4v_to_v3_5	<< std::endl;
		SD_LOG_DEBUG() <<" v3_5v_to_v3_6:1;"   						 << (uint16_t)init_status.card.mem_ocr.bit.v3_5v_to_v3_6	<< std::endl;
		SD_LOG_DEBUG() <<" s18a:1;"   								 << (uint16_t)init_status.card.mem_ocr.bit.s18a			<< std::endl;
		SD_LOG_DEBUG() <<" rsvd3:5;"   								 << (uint16_t)init_status.card.mem_ocr.bit.rsvd3			<< std::endl;
		SD_LOG_DEBUG() <<" ccs:1;"									 << (uint16_t)init_status.card.mem_ocr.bit.ccs			<< std::endl;
 		SD_LOG_DEBUG() <<" busy:1;"									 << (uint16_t)init_status.card.mem_ocr.bit.busy			<< std::endl;  

SD_LOG_DEBUG() << "------------cid------------" <<std::endl;		

		SD_LOG_DEBUG() << "rhs = "<< init_status.card.cid.rhs()<<std::endl;
		SD_LOG_DEBUG() << "lhs = "<< init_status.card.cid.lhs()<<std::endl;
		SD_LOG_DEBUG() <<"cid.mdt()	"     << init_status.card.cid.mdt()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.cid_rsvd"	 << init_status.card.cid.cid_rsvd1()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.psn()	"	 << init_status.card.cid.psn()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.prv()	"	 << init_status.card.cid.prv()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.pnm()	"     << init_status.card.cid.pnm()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.oid()	"     << init_status.card.cid.oid()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.mid()	"     << init_status.card.cid.mid()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.rhs()	"	 << init_status.card.cid.rhs()	<< std::endl;
 		SD_LOG_DEBUG() <<"cid.lhs()	"	 << init_status.card.cid.lhs()	<< std::endl;  		


SD_LOG_DEBUG() << "-------------------------------Result---------------------------" <<std::endl;
SD_LOG_DEBUG() << "voltage_switch" <<std::hex<<init_status.result.voltage_switch<<std::endl;
SD_LOG_DEBUG() << "sdio_init" <<std::hex<<init_status.result.sdio_init<<std::endl;
SD_LOG_DEBUG() << "mem_init" <<std::hex<<init_status.result.mem_init<<std::endl;
SD_LOG_DEBUG() << "card_unusable" <<std::hex<<init_status.result.card_unusable<<std::endl;

}
else
{
SD_LOG_DEBUG() << "Card init Fail"  <<std::endl;
}

#if 0
//utilSleep(15000);

if(result)
{

		result = get_pciheader(p_header,0);
		SD_LOG_DEBUG() << "Inside Actual SD, get_total_pcidevice = " << std::hex << (get_total_pcidevice()) << std::endl;
		SD_LOG_DEBUG() << "SD Bus vendor_id = "   << std::hex << (uint16_t)(p_header->vendor_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus device_id = "   << std::hex << (p_header->device_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus command = "     << std::hex << (p_header->command) << std::endl;
		SD_LOG_DEBUG() << "SD Bus status = "      << std::hex << (p_header->status) << std::endl;
		SD_LOG_DEBUG() << "SD Bus revision_id = " << std::hex << (uint16_t)(p_header->revision_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus classcode = "   << std::hex << (p_header->classcode) << std::endl;
		SD_LOG_DEBUG() << "SD Bus cachelinesize = " << std::hex << (uint16_t)(p_header->cachelinesize) << std::endl;
		SD_LOG_DEBUG() << "SD Bus latencytimer = " << std::hex << (uint16_t)(p_header->latencytimer) << std::endl;
		SD_LOG_DEBUG() << "SD Bus headertype = " << std::hex << (uint16_t)(p_header->headertype) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bist = " << std::hex << (uint16_t)(p_header->bist) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (p_header->bar0) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (p_header->bar1) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (p_header->bar2) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (p_header->bar3) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (p_header->bar4) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (p_header->bar5) << std::endl;
		SD_LOG_DEBUG() << "SD Bus slotinfo = " << std::hex << (uint16_t)(p_header->slotinfo) << std::endl;
		
}



_reg* sd_host_register_def = new _reg;
sd_host_register_def->sd_reg.value		= 0x00;
sd_host_register_def->sd_reg.offset		= 0x29;
sd_host_register_def->sd_reg.width		= 0x01;
sd_host_register_def->sd_reg.device		= 0x00;
sd_host_register_def->sd_reg.slot		= 0x00;
result = read_host_reg(sd_host_register_def);
SD_LOG_DEBUG() << "SD register Read 0x29 = " << std::hex << sd_host_register_def->sd_reg.value <<" result  : " << result<<std::endl;


sd_host_register_def->sd_reg.value		= 0x0F;
sd_host_register_def->sd_reg.offset		= 0x29;
sd_host_register_def->sd_reg.width		= 0x01;
sd_host_register_def->sd_reg.device		= 0x00;
sd_host_register_def->sd_reg.slot		= 0x00;
result = write_host_reg(sd_host_register_def);
SD_LOG_DEBUG() << "SD register write 0x29 = " << std::hex << sd_host_register_def->sd_reg.value <<" result  : " << result<<std::endl;

sd_host_register_def->sd_reg.value		= 0x00;
sd_host_register_def->sd_reg.offset		= 0x29;
sd_host_register_def->sd_reg.width		= 0x01;
sd_host_register_def->sd_reg.device		= 0x00;
sd_host_register_def->sd_reg.slot		= 0x00;
result = read_host_reg(sd_host_register_def);
SD_LOG_DEBUG() << "SD register Read 0x29 = " << std::hex << sd_host_register_def->sd_reg.value <<" result  : " << result<<std::endl;

if(detect_card(0,0))
{
SD_LOG_DEBUG() << "Card detected" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "Card removed" <<std::endl;
}
/*___________________________________*/
if(card_writeprotected(0,0))
{
SD_LOG_DEBUG() << "Card Write Protected" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "Card not Write Protected" <<std::endl;
}
uint16_t rca = 0x00;
/*___________________________________*/
if(init_host(0,0))
{
SD_LOG_DEBUG() << "Host init pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "Host init  Fail"  <<std::endl;
}
if(sw_host_reset(SW_ALL_LINE_RESET,0,0))
{
SD_LOG_DEBUG() << "S/W reset ALL pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "S/W reset ALL Fail"  <<std::endl;
}

/*___________________________________*/
if(go_idle(0,0))
{
SD_LOG_DEBUG() << "go_idle pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "go_idle Fail"  <<std::endl;
}
/*___________________________________*/

_r7 r7_1;
if(send_if_cond(0x01,&r7_1,0,0))
{
SD_LOG_DEBUG() << "send_if_cond pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "send_if_cond Fail"  <<std::endl;
}
/*___________________________________*/

_r3 ocr;
if(get_ocr(0,0,&ocr))
{
SD_LOG_DEBUG() << "get_ocr pass : "<< ocr.reg_val <<std::endl;


		SD_LOG_DEBUG() <<" rsvd1:7;"									 << (uint16_t)ocr.bit.rsvd1	        << std::endl;
		SD_LOG_DEBUG() <<" low_voltage:1;"							 << (uint16_t)ocr.bit.low_voltage	<< std::endl;
		SD_LOG_DEBUG() <<" rsvd2:7;"									 << (uint16_t)ocr.bit.rsvd2	        << std::endl;
		SD_LOG_DEBUG() <<" v2_7v_to_v2_8:1;"							 << (uint16_t)ocr.bit.v2_7v_to_v2_8	<< std::endl;
		SD_LOG_DEBUG() <<" v2_8v_to_v2_9:1;"   						 << (uint16_t)ocr.bit.v2_8v_to_v2_9	<< std::endl;
		SD_LOG_DEBUG() <<" v2_9v_to_v3_0:1;"   						 << (uint16_t)ocr.bit.v2_9v_to_v3_0	<< std::endl;
		SD_LOG_DEBUG() <<" v3_0v_to_v3_1:1;"   						 << (uint16_t)ocr.bit.v3_0v_to_v3_1	<< std::endl;
		SD_LOG_DEBUG() <<" v3_1v_to_v3_2:1;"							 << (uint16_t)ocr.bit.v3_1v_to_v3_2	<< std::endl;
 		SD_LOG_DEBUG() <<" v3_2v_to_v3_3:1;"							 << (uint16_t)ocr.bit.v3_2v_to_v3_3	<< std::endl;  
		SD_LOG_DEBUG() <<" v3_3v_to_v3_4:1;"							 << (uint16_t)ocr.bit.v3_3v_to_v3_4	<< std::endl;
		SD_LOG_DEBUG() <<" v3_4v_to_v3_5:1;"							 << (uint16_t)ocr.bit.v3_4v_to_v3_5	<< std::endl;
		SD_LOG_DEBUG() <<" v3_5v_to_v3_6:1;"   						 << (uint16_t)ocr.bit.v3_5v_to_v3_6	<< std::endl;
		SD_LOG_DEBUG() <<" s18a:1;"   								 << (uint16_t)ocr.bit.s18a			<< std::endl;
		SD_LOG_DEBUG() <<" rsvd3:5;"   								 << (uint16_t)ocr.bit.rsvd3			<< std::endl;
		SD_LOG_DEBUG() <<" ccs:1;"									 << (uint16_t)ocr.bit.ccs			<< std::endl;
 		SD_LOG_DEBUG() <<" busy:1;"									 << (uint16_t)ocr.bit.busy			<< std::endl;  
}
else
{
SD_LOG_DEBUG() << "get_ocr Fail"  <<std::endl;
}
/*___________________________________*/

_r3 r3_1;
if(send_op_cond(true,true,&r3_1,rca))
{
SD_LOG_DEBUG() << "send_op_cond pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "send_op_cond Fail"  <<std::endl;
}


_r2 cid;
if(get_all_cid(&cid,0,0))
{
		SD_LOG_DEBUG() << "get_all_cid pass" <<std::endl;
		SD_LOG_DEBUG() << "get_all_cid pass rhs = "<< cid.rhs()<<std::endl;
		SD_LOG_DEBUG() << "get_all_cid pass rhs = "<< cid.lhs()<<std::endl;
		SD_LOG_DEBUG() <<"cid.mdt()	"     << cid.mdt()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.cid_rsvd"	 << cid.cid_rsvd1()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.psn()	"	 << cid.psn()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.prv()	"	 << cid.prv()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.pnm()	"     << cid.pnm()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.oid()	"     << cid.oid()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.mid()	"     << cid.mid()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.rhs()	"	 << cid.rhs()	<< std::endl;
 		SD_LOG_DEBUG() <<"cid.lhs()	"	 << cid.lhs()	<< std::endl;  
}
else
{
SD_LOG_DEBUG() << "get_all_cid Fail"  <<std::endl;
}
/*___________________________________*/

/*___________________________________*/

if(init_card(0,0))
{
SD_LOG_DEBUG() << "Card init pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "Card init Fail"  <<std::endl;
}
/*___________________________________*/

if(get_rca(&rca,0,0))
{
SD_LOG_DEBUG() << "Get rca pass : " << rca << std::endl;
}
else
{
SD_LOG_DEBUG() << "Get rca Fail"  <<std::endl;
}
#endif 
#if 0
/*___________________________________*/

_r1 card_status;
uint32_t error = 0x00;
if(get_card_status(&card_status,rca,&error))
{
		SD_LOG_DEBUG() << "get_card_status pass : " << card_status.reg_val <<std::endl;
		SD_LOG_DEBUG() <<"card_status.manufature_testmode" 	 << (uint16_t)card_status.bit.manufature_testmode<< std::endl;
		SD_LOG_DEBUG() <<"card_status.app_specific:" 		 << (uint16_t)card_status.bit.app_specific<< std::endl;
		SD_LOG_DEBUG() <<"card_status.ake_seq_error:" 		 << (uint16_t)card_status.bit.ake_seq_error<< std::endl;
		SD_LOG_DEBUG() <<"card_status.sdio:" 				 << (uint16_t)card_status.bit.sdio<< std::endl;
		SD_LOG_DEBUG() <<"card_status.app_cmd:" 			     << (uint16_t)card_status.bit.app_cmd<< std::endl;
		SD_LOG_DEBUG() <<"card_status.rsvd1:" 				 << (uint16_t)card_status.bit.rsvd1<< std::endl;
		SD_LOG_DEBUG() <<"card_status.rdy_for_data:" 		 << (uint16_t)card_status.bit.rdy_for_data<< std::endl;
		SD_LOG_DEBUG() <<"card_status.current_state:" 		 << (uint16_t)card_status.bit.current_state<<std::endl;
		SD_LOG_DEBUG() <<"card_status.erase_reset		"	 << (uint16_t)card_status.bit.erase_reset<< std::endl;
		SD_LOG_DEBUG() <<"card_status.card_ecc_disable:"	     << (uint16_t)card_status.bit.card_ecc_disable<< std::endl;
		SD_LOG_DEBUG() <<"card_status.wp_erase_skip:" 		 << (uint16_t)card_status.bit.wp_erase_skip<< std::endl;
		SD_LOG_DEBUG() <<"card_status.csd_overwrite:" 		 << (uint16_t)card_status.bit.csd_overwrite<< std::endl;
		SD_LOG_DEBUG() <<"card_status.rsvd2:"				 << (uint16_t)card_status.bit.rsvd2<< std::endl;
		SD_LOG_DEBUG() <<"card_status.error:" 				 << (uint16_t)card_status.bit.error<< std::endl;
		SD_LOG_DEBUG() <<"card_status.cc_error:" 			 << (uint16_t)card_status.bit.cc_error<< std::endl;
		SD_LOG_DEBUG() <<"card_status.card_ecc_failed:" 	     << (uint16_t)card_status.bit.card_ecc_failed<< std::endl;
		SD_LOG_DEBUG() <<"card_status.illegal_command:" 	     << (uint16_t)card_status.bit.illegal_command<< std::endl;
		SD_LOG_DEBUG() <<"card_status.com_crc_error:" 		 << (uint16_t)card_status.bit.com_crc_error<< std::endl;
		SD_LOG_DEBUG() <<"card_status.lock_unlock_fail:" 	 << (uint16_t)card_status.bit.lock_unlock_fail<< std::endl;
		SD_LOG_DEBUG() <<"card_status.card_is_locked:" 		 << (uint16_t)card_status.bit.card_is_locked<< std::endl;
		SD_LOG_DEBUG() <<"card_status.wp_violation:" 		 << (uint16_t)card_status.bit.wp_violation<< std::endl;
		SD_LOG_DEBUG() <<"card_status.erase_param:" 		     << (uint16_t)card_status.bit.erase_param<< std::endl;
		SD_LOG_DEBUG() <<"card_status.erase_seq_error:" 	     << (uint16_t)card_status.bit.erase_seq_error<< std::endl;
		SD_LOG_DEBUG() <<"card_status.block_len_error:" 	     << (uint16_t)card_status.bit.block_len_error<< std::endl;
		SD_LOG_DEBUG() <<"card_status.address_error:" 		 << (uint16_t)card_status.bit.address_error<< std::endl;
 		SD_LOG_DEBUG() <<"out_of_range:" 		 			 << (uint16_t)card_status.bit.out_of_range<< std::endl;                   	


}
else
{
SD_LOG_DEBUG() << "get_card_status Fail"  <<std::endl;
}
/*___________________________________*/

_r2 cid1;
if(get_cid(0,0,&cid1))
{
		SD_LOG_DEBUG() << "get_cid pass rhs = "<< cid1.rhs()<<std::endl;
		SD_LOG_DEBUG() << "get_cid pass rhs = "<< cid1.lhs()<<std::endl;
		SD_LOG_DEBUG() << "get_all_cid pass" <<std::endl;
		SD_LOG_DEBUG() <<"cid.mdt()	"     << cid1.mdt()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.cid_rsvd"	 << cid1.cid_rsvd1()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.psn()	"	 << cid1.psn()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.prv()	"	 << cid1.prv()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.pnm()	"     << cid1.pnm()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.oid()	"     << cid1.oid()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.mid()	"     << cid1.mid()	<< std::endl;
		SD_LOG_DEBUG() <<"cid.rhs()	"	 << cid1.rhs()	<< std::endl;
 		SD_LOG_DEBUG() <<"cid.lhs()	"	 << cid1.lhs()	<< std::endl;  
}
else
{
SD_LOG_DEBUG() << "get_cid fail"  <<std::endl;
}
#endif
/*___________________________________*/
_r2 csd;
uint32_t error;
if(get_csd(0,0,&csd,&error))
{
SD_LOG_DEBUG() << "get_csd pass" <<std::endl;

		SD_LOG_DEBUG() << "get_all_csd pass rhs = "<< csd.rhs()<<std::endl;
		SD_LOG_DEBUG() << "get_all_csd pass rhs = "<< csd.lhs()<<std::endl;

		SD_LOG_DEBUG() <<"csd.csd_rsvd1()			" << (uint16_t)csd.csd_rsvd1()			<< std::endl;
		SD_LOG_DEBUG() <<"csd.file_format()		" << (uint16_t)csd.file_format()		<< std::endl;
		SD_LOG_DEBUG() <<"csd.tmp_write_protect()	" << (uint16_t)csd.tmp_write_protect()	<< std::endl;
		SD_LOG_DEBUG() <<"csd.perm_write_protect()" << (uint16_t)csd.perm_write_protect()<< std::endl;
		SD_LOG_DEBUG() <<"csd.copy()				" << (uint16_t)csd.copy()				<< std::endl;
		SD_LOG_DEBUG() <<"csd.file_format_grp()	" << (uint16_t)csd.file_format_grp()	<< std::endl;
		SD_LOG_DEBUG() <<"csd.rsvd2()				" << (uint16_t)csd.rsvd2()				<< std::endl;
		SD_LOG_DEBUG() <<"csd.write_bl_partial()	" << (uint16_t)csd.write_bl_partial()	<< std::endl;
 		SD_LOG_DEBUG() <<"csd.write_bl_length()	" << (uint16_t)csd.write_bl_length()	<< std::endl;  
		SD_LOG_DEBUG() <<"csd.r2w_factor()		" << (uint16_t)csd.r2w_factor()		<< std::endl;
		SD_LOG_DEBUG() <<"csd.rsvd3()				" << (uint16_t)csd.rsvd3()				<< std::endl;
		SD_LOG_DEBUG() <<"csd.wp_grp_enable()		" << (uint16_t)csd.wp_grp_enable()		<< std::endl;
		SD_LOG_DEBUG() <<"csd.wp_grp_size()		" << (uint16_t)csd.wp_grp_size()		<< std::endl;
		SD_LOG_DEBUG() <<"csd.sector_size()		" << (uint16_t)csd.sector_size()		<< std::endl;
		SD_LOG_DEBUG() <<"csd.erase_block_enable()" << (uint16_t)csd.erase_block_enable()<< std::endl;
 		SD_LOG_DEBUG() <<"csd.c_size_mult()		" << (uint16_t)csd.c_size_mult()		<< std::endl;  
		SD_LOG_DEBUG() <<"csd.vdd_w_curr_max()	" << (uint16_t)csd.vdd_w_curr_max()	<< std::endl;
		SD_LOG_DEBUG() <<"csd.vdd_w_curr_min()	" << (uint16_t)csd.vdd_w_curr_min()	<< std::endl;
		SD_LOG_DEBUG() <<"csd.vdd_r_curr_max()	" << (uint16_t)csd.vdd_r_curr_max()	<< std::endl;
		SD_LOG_DEBUG() <<"csd.vdd_r_curr_min()	" << (uint16_t)csd.vdd_r_curr_min()	<< std::endl;
		SD_LOG_DEBUG() <<"csd. c_size()			" << (uint16_t)csd. c_size()			<< std::endl;
		SD_LOG_DEBUG() <<"csd.rsvd4()				" << (uint16_t)csd.rsvd4()				<< std::endl;
		SD_LOG_DEBUG() <<"csd.dsr_imp()			" << (uint16_t)csd.dsr_imp()			<< std::endl;
		SD_LOG_DEBUG() <<"csd.read_blk_misalign()	" << (uint16_t)csd.read_blk_misalign()	<< std::endl;
 		SD_LOG_DEBUG() <<"csd.write_blk_misalign()" << (uint16_t)csd.write_blk_misalign()<< std::endl;  
		SD_LOG_DEBUG() <<"csd.read_bl_partial()	" << (uint16_t)csd.read_bl_partial()	<< std::endl;
		SD_LOG_DEBUG() <<"csd.read_bl_length()	" << (uint16_t)csd.read_bl_length()	<< std::endl;
		SD_LOG_DEBUG() <<"csd. ccc()				" << (uint16_t)csd. ccc()				<< std::endl;
		SD_LOG_DEBUG() <<"csd. trans_speed()		" << (uint16_t)csd. trans_speed()		<< std::endl;
		SD_LOG_DEBUG() <<"csd. nsac()				" << (uint16_t)csd. nsac()				<< std::endl;
		SD_LOG_DEBUG() <<"csd. taac()				" << (uint16_t)csd. taac()				<< std::endl;
 		SD_LOG_DEBUG() <<"csd.rsvd5()				" << (uint16_t)csd.rsvd5()				<< std::endl;  
		SD_LOG_DEBUG() <<"csd.csd_structure()		" << (uint16_t)csd.csd_structure()		<< std::endl;  

}
else
{
SD_LOG_DEBUG() << "get_csd fail"  <<std::endl;
}
/*___________________________________*/

SD_LOG_DEBUG() << "------------------------PIO Single Block Read/Write ---------------------------"  <<std::endl;
_r1 status;
#if 0

if(!select_card(rca,&status))
SD_LOG_DEBUG() << "select_card Fail"  <<std::endl;

if(get_card_status(&status,rca))
if(tran == status.bit.current_state)	 
SD_LOG_DEBUG() << "Current state : tran"  <<std::endl;

if(!set_card_buswidth(rca,0x00,&status,&error))
SD_LOG_DEBUG() << "set_card_buswidth"  <<std::endl;


if(!set_card_blocklength(0x200,&status,&error))
SD_LOG_DEBUG() << "set_card_blocklength"  <<std::endl;




if(!set_host_buswidth( false, 0,0))
SD_LOG_DEBUG() << "set_host_buswidth Fail"  <<std::endl;


if(!set_timeout_ctrl(0x0E,0,0))
SD_LOG_DEBUG() << "set_timeout_ctrl"  <<std::endl;


if(!set_host_block_length_n_count( 0x200,  0x01, 0,0))
SD_LOG_DEBUG() << "set_host_block_length_n_count Fail"  <<std::endl;

if(!set_transfermode(false, false, 0x00, false, false, 0,0))
SD_LOG_DEBUG() << "set_transfermode"  <<std::endl;






//if(!set_card_blockcount(0x01,&status))
//SD_LOG_DEBUG() << "set_card_blockcount"  <<std::endl;

if(!write_singleblock(0x5000,&status))
SD_LOG_DEBUG() << "write_singleblock"  <<std::endl;

_reg sd_register;
reg32 present_state;

utilSleep(150);

  SET_SD_REGISTER_READ(&sd_register,HOST_PRESENT_STATE,0,0);
  if(!read_host_reg(&sd_register))
		SD_LOG_DEBUG() << "read_host_reg failed"  <<std::endl;
	
  present_state.reg_val = sd_register.sd_reg.value;
		SD_LOG_DEBUG() << "HOST_PRESENT_STATE : "<<std::hex<<sd_register.sd_reg.value  <<std::endl;


if(present_state.bit.b10 == 0x01)
  {
  uint32_t loop_c1 =0x00;
 SD_LOG_DEBUG() << "Inside present_state.bit.b10 == 0x01"  <<std::endl;
 for (loop_c1 = 0; loop_c1 < 128; loop_c1++)
		    {
		         SET_SD_REGISTER_WRITE(&sd_register,HOST_BUFFER_0,0,0,0xdeadbeef);
				 sd_register.sd_reg.width = 0x04;
				   if(!write_host_reg(&sd_register))
						SD_LOG_DEBUG() << "write_host_reg failed"  <<std::endl;
		    }
  
  }
  utilSleep(150); 


  if(!get_transfer_cmplete(&error))
    SD_LOG_DEBUG() << "TRNS complete error : "<<error  <<std::endl;
  
 #endif
 
 
 
 #if 0
 uint32_t error;
   _reg sd_register;
  reg32 present_state;
 uint32_t card_address = 0x4000;
 uint32_t size = BLOCK_LENGTH * 2;
 
 uint8_t buff_write[0x200 * 7];
 uint8_t buff_read[0x200 * 7];
  uint8_t buff_read1[0x200 * 7];
 int icount = 0;
 
 for(icount = 0; icount <0x100 ;icount+= 4)
 {
	buff_write[icount] = 0xaf;
	buff_write[icount+1] = 0xbe;
	buff_write[icount + 2] = 0xad;
	buff_write[icount + 3] = 0xde;
 }
 
 for(icount = 0x100; icount <0x200 ;icount+= 4)
 {
	buff_write[icount] = 0x5A;
	buff_write[icount+1] = 0x5A;
	buff_write[icount + 2] = 0x5A;
	buff_write[icount + 3] = 0x5A;
 }
 
 

  if(!single_block_write( 0,0, false , false ,card_address,&buff_write[0], 0x200 , 0x00))
    {
		SD_LOG_DEBUG() << "single_block_write failed"  <<std::endl;
		return TestResult;
	}
  
  
  if(!single_block_read( 0,0, false, false ,card_address,&buff_read[0], 0x100 , 0x00))
{    
	SD_LOG_DEBUG() << "single_block_read failed"  <<std::endl;
	        return TestResult;
}	 
 for(icount = 0; icount <0x100 ;icount+= 1)
 {
	printf("RED Data [%d]  =  %X\n",icount,buff_read[icount]);
 }	 
	 
    if(!single_block_read( 0,0, false, false ,(card_address + 0x100),&buff_read[0x100], 0x100 , 0x00))
{    
	SD_LOG_DEBUG() << "single_block_read failed"  <<std::endl;
	        return TestResult;
 }
 for(icount = 0x100; icount <0x200 ;icount+= 1)
 {
	printf("RED Data [%d]  =  %X\n",icount,buff_read[icount]);
 }
 
 
   if(!multi_block_read( 0,0, false, false ,card_address,&buff_read1[0], 0x40 ,0x08))
     SD_LOG_DEBUG() << "multi_block_read failed"  <<std::endl;
  for(icount = 0x00; icount <0x200 ;icount++)
 {
	printf("RED Data [%d]  =  %X\n",icount,buff_read1[icount]);
 }
 
 #endif
 
 
 
 
 
 
 #if 0 
sdma_transfer(0x4000,0xDEADBEEF);
 if(!set_normal_mode( 0,  0,  HIGH_SPEED, DIVIDED_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_normal_mode failed "  <<std::endl;
	//goto END_LINE;
}
#endif

#if 0

 if(!set_UHS_mode( 0,  0,  SDR_12, DIVIDED_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x2000,0xDEADBEEF);

 if(!set_UHS_mode( 0,  0,  SDR_25, DIVIDED_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x3000,0xDEADBEEF);

 if(!set_UHS_mode( 0,  0,  SDR_50, PROGRAMABLE_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x4000,0xDEADBEEF);
#if 0

 if(!set_UHS_mode( 0,  0,  SDR_104, PROGRAMABLE_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x6000,0xDEADBEEF);

 if(!set_UHS_mode( 0,  0,  DDR_50, PROGRAMABLE_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x6000,0xDEADBEEF);
#endif
#endif
#if 0
 if(!set_UHS_mode( 0,  0,  SDR_12, PRESET_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x5000,0xDEADBEEF);

 if(!set_UHS_mode( 0,  0,  SDR_25, PRESET_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x6000,0xDEADBEEF);

 if(!set_UHS_mode( 0,  0,  SDR_50, PRESET_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x7000,0xDEADBEEF);
#if 0

 if(!set_UHS_mode( 0,  0,  SDR_104, PRESET_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x6000,0xDEADBEEF);

 if(!set_UHS_mode( 0,  0,  DDR_50, PRESET_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}
sdma_transfer(0x6000,0xDEADBEEF);
#endif
#endif


if(!set_UHS_mode( 0,  0,  SDR_50, PRESET_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_UHS_mode failed "  <<std::endl;
	//goto END_LINE;
}

if(!tuning_procedure( 0, 0))
{
	SD_LOG_DEBUG() << "tuning_procedure failed "  <<std::endl;
	//goto END_LINE;
}
else
{
#if 1

_scr scr_data;
//if(!get_scr(0, 0, &scr_data,&status,false, true))
//printf("get_scr failed \n");

//printf(" CMD Support : %X\n",(uint16_t) scr_data.cmd_support());



env::sys::Device device1,device2;
env::dma::Resource* resource1;
env::dma::Resource* resource2;
uint32_t int_status =0x00000000,data = 0x00000000;
resource1 = device1.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
printf("Base : %X\n",resource1->base());

resource2 = device2.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
printf("Base : %X\n",resource2->base());
bool toggle = true;
bool failed = false;
int count_data_trans = 0x00;
do
{
		if(toggle == true)
		{
			data = 0xDEADBEEF;
			toggle = false;
		}	
		else
		{
		data = 0xBEEFDEAD;
		toggle = true;
		}
		for(i=0;i<0x800; i= i + 1)
		{
			resource1->write(i,1,&data);
		}
		uint32_t write_address[4] = {resource1->base()};
		uint32_t read_address[2] =  {resource2->base()};
		
		if(!sdma_write( 0 ,  0 , 0x6000 /*card_address*/ , &write_address[0]/*phy_address*/ , 7 /*mem size*/, 0x200 /*blk_size*/, 16 /*blk_count*/, false, false, 0x01))
		{
			std::cout << "sdma write failed : "<<std::endl;
			failed = true;
		}	
		if(failed == false)
		{
			utilSleep(150);
			if(!sdma_read( 0 ,  0 , 0x6000 /*card_address*/ , &read_address[0]/*phy_address*/ , 7 /*mem size*/, 0x200 /*blk_size*/, 16 /*blk_count*/, false, false, 0x01))
				{
					std::cout << "sdma read failed : "<<std::endl;
					failed = true;
				}
				else{
						uint32_t data_read;
						int j = 0;
						for(i=0;i<(0x200 * 4); i= i + 1)
						{
							resource2->read(i,1,&data_read);
							if(data_read != data)
							{
								printf( "Data Transfer Failed \n");
								failed = true;
								break;
							}
						}
					}	
		}			
	int_status = get_int_status(0,0);
	utilSleep(150);
	count_data_trans++;
}while(((int_status & 0x04001000) == 0x00000000) && (failed == false));

printf("Interrupt : %X\n", int_status);
printf("count_data_trans : %X\n", count_data_trans);
}



#endif

#if 0
 /* Error int recovery*/
 
 
 
#endif



 #if 0
 
 uint8_t buff_write[0x200 * 7];
 uint8_t buff_read[0x200 * 7];
  uint8_t buff_read1[0x200 * 7];
 int icount = 0;
  for(icount = 0x00; icount <((0x200 * 2)) ;icount+= 4)
 {
	buff_write[icount] = 0x5A;
	buff_write[icount+1] = 0xAB;
	buff_write[icount + 2] = 0xCD;
	buff_write[icount + 3] = 0xDE;
 }
   for(icount = (0x200 * 2); icount <((0x200 * 4)) ;icount+= 4)
 {
	buff_write[icount]     = 0xEF;
	buff_write[icount+1]   = 0xBE;
	buff_write[icount + 2] = 0xAD;
	buff_write[icount + 3] = 0xDE;
 }
  if(!set_normal_mode( 0,  0,  HIGH_SPEED, DIVIDED_CLK_SELECTION_MODE))
{
	SD_LOG_DEBUG() << "set_normal_mode failed "  <<std::endl;
	//goto END_LINE;
}


if(!pio_write(0, 0,false, false,0x01 , 0x7000,&buff_write[0],0x200,4))
	SD_LOG_DEBUG() << "pio_write failed "  <<std::endl;

if(!pio_write(0, 0,false, false,0x01 , 0x7000,&buff_write[0],0x200,4))
	SD_LOG_DEBUG() << "pio_write failed "  <<std::endl;	

for(icount = 0x00; icount <((0x200 * 4)) ;icount++)
{
	if((buff_read[icount] != buff_write[icount]))
	{
		SD_LOG_DEBUG() << "data Comparison Failed : "<<std::dec<<icount <<std::endl;		
	}
}	
	
	
	
 #if 0
if(!pio_write_with_block_gap(0, 0,true, true,0x01 , 0x6000,&buff_write[0],0x200,4))
	SD_LOG_DEBUG() << "pio_write block Gap failed "  <<std::endl;
if(!pio_read_with_block_gap(0, 0,false, false,0x01 , 0x7000,&buff_read1[0],0x200,4))
{
	SD_LOG_DEBUG() << "pio_Read block Gap failed "  <<std::endl;
 goto END_LINE;
 }
 if(!pio_read_with_block_gap(0, 0,false, false,0x01 , 0x7000,&buff_read[0],0x200,4))
	{
	SD_LOG_DEBUG() << "pio_Read block Gap failed "  <<std::endl;
 //goto END_LINE;
  }



	

	
for(icount = 0x00; icount <((0x200 * 4)) ;icount++)
{
	if((buff_read[icount] != buff_write[icount]))
	{
		SD_LOG_DEBUG() << "data Comparison Failed : "<<std::dec<<icount <<std::endl;		
	}
}
 
 #endif

 #endif
 

 
 #if 0
  for(icount = 0x00; icount <((0x200 * 7)) ;icount+= 4)
 {
	buff_write[icount] = 0x5A;
	buff_write[icount+1] = 0xAB;
	buff_write[icount + 2] = 0xCD;
	buff_write[icount + 3] = 0xDE;
 }
 
 
 
 card_address = 0x8000;
 if(!pio_write(0,  0, true, true , 0x01 , card_address,&buff_write[0],0x200,3))
{
 SD_LOG_DEBUG() << "pio_write failed 0x200,3 "  <<std::endl;
return TestResult;
}
 if(!pio_write(0,  0, true, true , 0x00 , (card_address + 4),&buff_write[0x200 * 3],0x200,0))
{
 SD_LOG_DEBUG() << "pio_write failed 0x200,0"  <<std::endl;
return TestResult;
}
 if(!pio_write(0,  0, true, true , 0x01 , (card_address + 5),&buff_write[0x200 * 4],0x200,2))
{
 SD_LOG_DEBUG() << "pio_write failed 0x200,2"  <<std::endl;
return TestResult;
}	
	
 if(!pio_read(0,  0, true, true , 0x01 , card_address,&buff_read1[0],0x200,3))
{	SD_LOG_DEBUG() << "pio_read failed first"  <<std::endl;	
return TestResult;
}	
 if(!pio_read(0,  0, true, true , 0x01 , (card_address + 4),&buff_read1[0x200 * 3],0x200,3))
{	SD_LOG_DEBUG() << "pio_read failed second"  <<std::endl;	
 return TestResult;
 }
 
 for(icount = 0x00; icount <((0x200 * 6)) ;icount++)
 {
	printf("RED Data [%d]  =  %X\n",icount,buff_read1[icount]);
 }
 
 
 
 
 
 


  if(!multi_block_write( rca, false , false ,card_address,&buff_write[0], size))
     SD_LOG_DEBUG() << "multi_block_write failed"  <<std::endl;
  
  



for(icount = 0; icount <size ;icount+=4)
 {
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount + 3]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount + 2]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount + 1]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount])     & 0x00ff));
 } 

 for(icount = 0; icount <size ;icount++)
 {
	if(buff_write[icount] != buff_read[icount])
	SD_LOG_DEBUG() << "PIO transfer failed"<<std::endl;	
 }

   if(!erase_data(rca,card_address,(card_address + size),&status))
     SD_LOG_DEBUG() << "Erase failed"  <<std::endl;
   utilSleep(150); 
  if(!multi_block_read( rca, false, false ,card_address,&buff_read[0], size))
     SD_LOG_DEBUG() << "multi_block_read failed"  <<std::endl;	 

for(icount = 0; icount <size ;icount+=4)
 {
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount + 3]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount + 2]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount + 1]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount])     & 0x00ff));
 }	 
	 
#endif
#if 0
uint8_t lock_card_data[10];
/*Force Erase New*/
lock_card_data[0] = 0x08; 
lock_card_data[1] = 0x00; 
lock_card_data[2] = 'a'; 
lock_card_data[3] = 'b'; 
if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
if(get_card_status(0,0,&status,&error))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Force Unlock Passed"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}
#endif
#if 0
uint8_t lock_card_data[10];
lock_card_data[0] = 0x01; 
lock_card_data[1] = 0x02; 
lock_card_data[2] = 'm'; 
lock_card_data[3] = 'a'; 
/* Set Passwd*/
  if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(0,0,&status))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		SD_LOG_DEBUG() << "Passwd set"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}

/*Lock*/
lock_card_data[0] = 0x04; 
lock_card_data[1] = 0x02; 
lock_card_data[2] = 'm'; 
lock_card_data[3] = 'a'; 
if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(0,0,&status))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x01 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Locked"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}


/*Change*/
lock_card_data[0] = 0x01; 
lock_card_data[1] = 0x03; 
lock_card_data[2] = 'm'; 
lock_card_data[3] = 'a'; 
lock_card_data[4] = 'a'; 
lock_card_data[5] = 'b'; 
if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(0,0,&status))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		SD_LOG_DEBUG() << "Change"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}
/*Lock New*/
lock_card_data[0] = 0x04; 
lock_card_data[1] = 0x01; 
lock_card_data[2] = 'a'; 
lock_card_data[3] = 'b'; 
if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(0,0,&status))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x01 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Locked"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}

/*Force Erase New*/
lock_card_data[0] = 0x08; 
lock_card_data[1] = 0x00; 
lock_card_data[2] = 'a'; 
lock_card_data[3] = 'b'; 
if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
if(get_card_status(0,0,&status))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Force Unlock Passed"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}	


/*CLR New*/
lock_card_data[0] = 0x02; 
lock_card_data[1] = 0x02; 
lock_card_data[2] = 'a'; 
lock_card_data[3] = 'b'; 
if(!lock_unlock(0,0,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(0,0,&status))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Passwd clr and UnLocked"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}




/* Force Erase*/
  if(!lock_unlock(rca,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(&status,rca))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Passwd set"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}
/* Set Passwd*/
  if(!lock_unlock(rca,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(&status,rca))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Passwd set"  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}

/* Lock The card*/
lock_card_data[0] = 0x04; 
  if(!lock_unlock(rca,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(&status,rca))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x01 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Card Locked "  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}


/* UnLock The card*/
lock_card_data[0] = 0x00; 
  if(!lock_unlock(rca,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(&status,rca))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Card UnLocked "  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}
/* Chg Passwd*/

lock_card_data[0] = 0x01; 
lock_card_data[1] = 0x04; 
lock_card_data[2] = 'A'; 
lock_card_data[3] = 'B'; 
lock_card_data[4] = 'C'; 
lock_card_data[5] = 'D'; 


  if(!lock_unlock(rca,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(&status,rca))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "Card UnLocked "  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}

/* CLR Passwd*/
lock_card_data[0] = 0x02; 
lock_card_data[1] = 0x02; 
lock_card_data[2] = 'C'; 
lock_card_data[3] = 'D'; 

  if(!lock_unlock(rca,&lock_card_data[0],false, false , &status))
    SD_LOG_DEBUG() << "lock_unlock failed"  <<std::endl;
	
if(get_card_status(&status,rca))
{
	if(0x00 == status.bit.lock_unlock_fail)
	{
		if(0x00 == status.bit.card_is_locked)	 
		SD_LOG_DEBUG() << "pswd cleared "  <<std::endl;
	}
	else
	{
		SD_LOG_DEBUG() << "Lock/Unlock failed status is set "  <<std::endl;
	}
}
 


/* SCR register read*/
_scr scr;
if(!get_scr(0 , 0 , &scr,&status,true,true))
     SD_LOG_DEBUG() << "get_scr failed"  <<std::endl;

     SD_LOG_DEBUG() << "scr scr.scr_structure() = "  << std::hex<<(uint16_t)scr.scr_structure() <<std::endl;	
     SD_LOG_DEBUG() << "scr sd_spec() = "  << std::hex<<(uint16_t)scr.sd_spec() <<std::endl;	
     SD_LOG_DEBUG() << "scr data_stst_after_erase() = "  << std::hex<<(uint16_t)scr.data_stst_after_erase() <<std::endl;	
     SD_LOG_DEBUG() << "scr sd_security() = "  << std::hex<<(uint16_t)scr.sd_security() <<std::endl;	
     SD_LOG_DEBUG() << "scr sd_bus_width() = "  << std::hex<<(uint16_t)scr.sd_bus_width() <<std::endl;	
     SD_LOG_DEBUG() << "scr sd_spec3() = "  << std::hex<<(uint16_t)scr.sd_spec3() <<std::endl;	
     SD_LOG_DEBUG() << "scr ex_security() = "  << std::hex<<(uint16_t)scr.ex_security() <<std::endl;	
     SD_LOG_DEBUG() << "scr cmd_support() = "  << std::hex<<(uint16_t)scr.cmd_support() <<std::endl;	
     printf("SCR = %016llX",scr.scr_value );


_function function, function1;
	if(!get_supported_functions(0 , 0 , false,false,&function, &status))
     SD_LOG_DEBUG() << "get_supported_functions failed"  <<std::endl; 
	 
     SD_LOG_DEBUG() << "Group1 :"<<  std::hex << function.group1<<std::endl; 
     SD_LOG_DEBUG() << "Group2 :"<<  std::hex << function.group2<<std::endl; 	 
     SD_LOG_DEBUG() << "Group3 :"<<  std::hex << function.group3<<std::endl; 
     SD_LOG_DEBUG() << "Group4 :"<<  std::hex << function.group4<<std::endl; 	 
     SD_LOG_DEBUG() << "Group5 :"<<  std::hex << function.group5<<std::endl; 
     SD_LOG_DEBUG() << "Group6 :"<<  std::hex << function.group6<<std::endl; 	 
	 
	 function1.group1 = 0x1;
	 function1.group2 = 0xA;
	 function1.group3 = 0xB;
	 function1.group4 = 0xC;
	 function1.group5 = 0xD;
	 function1.group6 = 0xE;

 
	 if(!switch_function(0 , 0 , false,false,&function1, &status))
		SD_LOG_DEBUG() << "switch_function failed"  <<std::endl; 
		
	//if(!get_supported_functions(rca, false,false,&function, &status))
   //  SD_LOG_DEBUG() << "get_supported_functions failed"  <<std::endl; 
	 
     SD_LOG_DEBUG() << "maxCurrent :"<<  std::hex << function1.maxCurrent<<std::endl; 	 
     SD_LOG_DEBUG() << "Group1 :"<<  std::hex << function1.group1<<std::endl; 
     SD_LOG_DEBUG() << "Group2 :"<<  std::hex << function1.group2<<std::endl; 	 
     SD_LOG_DEBUG() << "Group3 :"<<  std::hex << function1.group3<<std::endl; 
     SD_LOG_DEBUG() << "Group4 :"<<  std::hex << function1.group4<<std::endl; 
     SD_LOG_DEBUG() << "Group5 :"<<  std::hex << function1.group5<<std::endl; 
     SD_LOG_DEBUG() << "Group6 :"<<  std::hex << function1.group6<<std::endl; 
     SD_LOG_DEBUG() << "current_grp1 :"<<  std::hex << function1.current_grp1<<std::endl; 
     SD_LOG_DEBUG() << "current_grp2 :"<<  std::hex << function1.current_grp2<<std::endl; 	 
     SD_LOG_DEBUG() << "current_grp3 :"<<  std::hex << function1.current_grp3<<std::endl; 
     SD_LOG_DEBUG() << "current_grp4 :"<<  std::hex << function1.current_grp4<<std::endl; 
     SD_LOG_DEBUG() << "current_grp5 :"<<  std::hex << function1.current_grp5<<std::endl; 
     SD_LOG_DEBUG() << "current_grp6 :"<<  std::hex << function1.current_grp6<<std::endl; 
	 SD_LOG_DEBUG() << "struct_version :"<<  std::hex << function1.struct_version<<std::endl; 
     SD_LOG_DEBUG() << "busy_grp1 :"<<  std::hex << function1.busy_grp1<<std::endl; 
     SD_LOG_DEBUG() << "busy_grp2 :"<<  std::hex << function1.busy_grp2<<std::endl; 	 
     SD_LOG_DEBUG() << "busy_grp3 :"<<  std::hex << function1.busy_grp3<<std::endl; 
     SD_LOG_DEBUG() << "busy_grp4 :"<<  std::hex << function1.busy_grp4<<std::endl; 
     SD_LOG_DEBUG() << "busy_grp5 :"<<  std::hex << function1.busy_grp5<<std::endl; 
     SD_LOG_DEBUG() << "busy_grp6 :"<<  std::hex << function1.busy_grp6<<std::endl; 	 
	



	_sd_status sd_status;
	 if(!get_sd_status(0 , 0 , false , true, &status , &sd_status))
		SD_LOG_DEBUG() << "get_sd_status failed"  <<std::endl; 

	SD_LOG_DEBUG() << "data_bus_width			: " << std::hex <<(uint16_t) sd_status.data_bus_width		  << std::endl ;
	SD_LOG_DEBUG() << "secured_mode              : " << std::hex <<(uint16_t)sd_status.secured_mode            << std::endl ;
	SD_LOG_DEBUG() << "security_funtions		 	: " << std::hex <<(uint16_t)sd_status.security_funtions       << std::endl ;
	SD_LOG_DEBUG() << "sd_card_type              : " << std::hex << sd_status.sd_card_type           		  << std::endl ;
	SD_LOG_DEBUG() << "size_of_protected_area  	: " << std::hex << sd_status.size_of_protected_area 		  << std::endl ;
	SD_LOG_DEBUG() << "speed_class              	: " << std::hex <<(uint16_t)sd_status.speed_class             << std::endl ;
	SD_LOG_DEBUG() << "performance_move          : " << std::hex <<(uint16_t) sd_status.performance_move       << std::endl ;
	SD_LOG_DEBUG() << "au_size                  	: " << std::hex <<(uint16_t) sd_status.au_size                << std::endl ;
	SD_LOG_DEBUG() << "erase_size              	: " << std::hex << sd_status.erase_size               	      << std::endl ;
	SD_LOG_DEBUG() << "erase_timeout             : " << std::hex <<(uint16_t) sd_status.erase_timeout          << std::endl ;
	SD_LOG_DEBUG() << "erase_offset             	: " << std::hex <<(uint16_t) sd_status.erase_offset           << std::endl ;
	SD_LOG_DEBUG() << "uhs_speed_grade           : " << std::hex <<(uint16_t) sd_status.uhs_speed_grade        << std::endl ;
	SD_LOG_DEBUG() << "uhs_au_size               : " << std::hex <<(uint16_t) sd_status.uhs_au_size            << std::endl ;
	 
	 
	 int int_count = 0;
	 uint32_t int_status = 0x00,int_enable = 0x01, force_int = 0x00; 
	 for(int_count = 0 ; int_count < 10 ; int_count++ )
	 {
	 	if(!int_status_enable( 0 , 0 , (uint32_t)(int_enable << (int_count + 16))))
			SD_LOG_DEBUG() << "int_status_enable failed"  <<std::endl; 

	 	if(!force_event( 0 , 0 , (uint32_t)(int_enable << (int_count + 16))))
			SD_LOG_DEBUG() << "force_event failed"  <<std::endl;
			
		if((uint32_t)((int_enable << (int_count + 16)) | 0x00008000) != (uint32_t)((uint32_t)0xFFFF8000 & (uint32_t)get_int_status(0,0)))
			SD_LOG_DEBUG() << "int_enable failed"  <<std::endl; 
	 }
	 
	 if(!int_status_enable( 0 , 0 , (uint32_t)(int_enable << (8 + 16 ))))
		SD_LOG_DEBUG() << "int_status_enable failed"  <<std::endl; 
	 uint16_t acmd_error = 0x00;
	 for(int_count = 0 ; int_count < 9 ; int_count++ )
	 {
	 	if(!force_event( 0 , 0 , (uint32_t)(int_enable << (int_count ))))
			SD_LOG_DEBUG() << "force_event failed"  <<std::endl;
		acmd_error = (uint16_t)(get_autocmd_error(0,0));	
		
		if((uint16_t)((int_enable << (int_count ))) != acmd_error)
			SD_LOG_DEBUG() << "int_enable failed"  <<std::endl; 
			SD_LOG_DEBUG() << "int Status : "<<std::hex<<(uint32_t)get_int_status(0,0)  <<std::endl; 
			SD_LOG_DEBUG() << "int Status : "<<std::hex<<acmd_error  <<std::endl; 
	 }
 #endif	
 #if 0
							env::sys::Device device1,device2,device3,device4,device5,device6;
							env::dma::Resource* resource1;
							env::dma::Resource* resource2;
							env::dma::Resource* resource3;
							env::dma::Resource* resource4;
							env::dma::Resource* resource5;
							env::dma::Resource* resource6;
							 resource1 = device1.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource1->base());
							 
							 resource2 = device2.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource2->base());
							 
							 resource3 = device3.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource3->base());
							 
							 resource4 = device4.palloc(0x6000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource4->base());

 //uint8_t check_data = 0x5A;
//resource1->write(0x2000,1,&check_data);							 
// resource->mPtr = 0x10;
//resource1->mBase
							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0xdeadbeef;
							 resource1->write(i,1,&check);
							 }

							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0xabcdef5b;
							 resource2->write(i,1,&check);
							 }	

							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0x5a5a5a5a;
							 resource3->write(i,1,&check);
							 }
							 
 SD_LOG_DEBUG() << "Base address : "<<std::hex << resource1->base() <<std::endl;
 uint32_t write_address[4] = {resource1->base() , resource2->base() , resource3->base()};
 uint32_t read_address[2] =  {resource4->base()};

 if(!sdma_write( 0 ,  0 , 0x4000 /*card_address*/ , &write_address[0]/*phy_address*/ , 0x04 /*mem size*/, 0x200 /*blk_size*/, 16 /*blk_count*/, true, true, 0x01))
    SD_LOG_DEBUG() << "sdma failed : "<<std::endl;
 
  if(!sdma_read( 0 ,  0 , 0x4000 /*card_address*/ , &read_address[0]/*phy_address*/ , 0x04 /*mem size*/, 0x200 /*blk_size*/, 16 /*blk_count*/, true, true, 0x01))
    SD_LOG_DEBUG() << "sdma failed : "<<std::endl;
	else{
uint8_t data_read;
	int j = 0;
							for(i=0;i<(0x200 * 16); i= i + 1)
							 {
							  resource4->read(i,1,&data_read);
							  printf("DATA [%d] = %X",i,(uint16_t)data_read);
							  printf("\t\t"); 
							  if(j++ == 10)
							  {
								printf("\n");
								j=0;
								
							 }	
							  //SD_LOG_DEBUG() << "Data["<< i <<"] : "<<std::hex<< data_read <<std::endl;
							 }
 }
 #endif
 #if 0
 
 							env::sys::Device device1,device2,device3,device4,device5,device6,device7;
							env::dma::Resource* resource1;
							env::dma::Resource* resource2;
							env::dma::Resource* resource3;
							env::dma::Resource* resource4;
							env::dma::Resource* resource5;
							env::dma::Resource* resource6;
							env::dma::Resource* table;
							 resource1 = device1.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource1->base());
							 
							 resource2 = device2.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource2->base());
							 
							 resource3 = device3.palloc(0x2000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource3->base());
							 
							 resource4 = device4.palloc(0x6000,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",resource4->base());
							 
							 table = device7.palloc(0x20,0,0x3ffffffe,0,env::dma::CacheDefault,1);
							 printf("Base : %X\n",table->base());

 //uint8_t check_data = 0x5A;
//resource1->write(0x2000,1,&check_data);							 
// resource->mPtr = 0x10;
//resource1->mBase
							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0xdeadbeef;
							 resource1->write(i,1,&check);
							 }

							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0xabcdef5b;
							 resource2->write(i,1,&check);
							 }	

							for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0x5a5a5a5a;
							 resource3->write(i,1,&check);
							 }
							 
							 for(i=0;i<0x800; i= i + 1)
							 {
							 check = 0x00000000;
							 resource4->write(i,1,&check);
							 }
 uint64_t table_data = 0x0000000000000000;
 uint32_t rhs = 0x00, lhs = 0x00; 
table_data =(uint64_t) (((uint64_t)(resource1->base() << 32) )| (uint64_t)((512*4) << 16) | (uint64_t)(0x21));
lhs = (uint32_t)((table_data & 0xffffffff00000000) >> 32);
rhs = (uint32_t)((table_data & 0x00000000ffffffff));
table->write(0,1,&rhs);
table->write(1,1,&lhs);
SD_LOG_DEBUG() << "Table 1 : "<< std::hex << table_data <<std::endl;
table_data = (uint64_t)(((uint64_t)(resource2->base() << 32)) | (uint64_t)((512*4) << 16) | (uint64_t)(0x21));
lhs = (uint32_t)((table_data & 0xffffffff00000000) >> 32);
rhs = (uint32_t)((table_data & 0x00000000ffffffff));
SD_LOG_DEBUG() << "Table 1 : "<< std::hex << table_data <<std::endl;
table->write(2,1,&rhs);
table->write(3,1,&lhs);
table_data = (uint64_t)(((uint64_t)(resource3->base() << 32))| (uint64_t)((512*4) << 16) |(uint64_t) (0x23));
SD_LOG_DEBUG() << "Table 1 : "<< std::hex << table_data <<std::endl;
lhs = (uint32_t)((table_data & 0xffffffff00000000) >> 32);
rhs = (uint32_t)((table_data & 0x00000000ffffffff));
table->write(4,1,&rhs);
table->write(5,1,&lhs);
card_address = 0x5000;

 if(!dma2_write(0 , 0 , card_address , table->base() , 0x200, 12, true, true,  0x01, 0x02))
    SD_LOG_DEBUG() << "adma write failed : "<<std::endl; 
	
table_data = (uint64_t)(((uint64_t)(resource4->base() << 32))| (uint64_t)((512*12) << 16) |(uint64_t) (0x23));
SD_LOG_DEBUG() << "Table 1 : "<< std::hex << table_data <<std::endl;
lhs = (uint32_t)((table_data & 0xffffffff00000000) >> 32);
rhs = (uint32_t)((table_data & 0x00000000ffffffff));
table->write(0,1,&rhs);
table->write(1,1,&lhs);
	
 if(!dma2_read(0 , 0 , card_address , table->base() , 0x200, 12, true, true,  0x01, 0x02))
    SD_LOG_DEBUG() << "adma read failed : "<<std::endl; 
	
							 for(i=0;i<0x601; i= i + 1)
							 {
							  resource4->read(i,1,&check);
							  SD_LOG_DEBUG() << "Data["<<std::dec<<i*4<<"] : "<<std::hex<<check<<std::endl;
							  
							 }
	
 #endif
 
 
SD_LOG_DEBUG() << "slot : "<< std::hex<<(uint16_t)get_total_slot(0)<<std::endl;
 #if 0
 card_address = 0x9000 ;
/*----------------- Speed high, width 4bit-------------------*/
  if(!multi_block_write( rca, true , true ,card_address,&buff_write[0], size))
     SD_LOG_DEBUG() << "multi_block_write failed"  <<std::endl;
  
  
  if(!multi_block_read( rca, true, true ,card_address,&buff_read[0], size))
     SD_LOG_DEBUG() << "multi_block_read failed"  <<std::endl;


for(icount = 0; icount <2048 ;icount+=4)
 {
  printf(" Read data from test file [%d]   : %X\n",icount + 3,(((uint16_t)buff_read[icount + 3]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount + 2,(((uint16_t)buff_read[icount + 2]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount + 1,(((uint16_t)buff_read[icount + 1]) & 0x00ff));
  printf(" Read data from test file [%d]   : %X\n",icount,(((uint16_t)buff_read[icount])     & 0x00ff));
 } 

 for(icount = 0; icount <2048 ;icount++)
 {
	if(buff_write[icount] != buff_read[icount])
	SD_LOG_DEBUG() << "PIO transfer failed"<<std::endl;	
 } 






_r2 cid;
_r2 csd;
uint16_t rca;


bool issue_command(_command *pcmd);
bool issue_appcommand(_command *pcmd);
bool go_idle(uint32_t device,uint32_t slot);
bool send_if_cond(uint8_t vhs,_r7* ptr_r7,uint32_t device,uint32_t slot);
bool send_op_cond(bool s18r,bool hcs,_r3* ptr_ocr,uint16_t rca);



if(sw_host_reset(SW_DAT_LINE_RESET,0,0))
{
SD_LOG_DEBUG() << "S/W reset DAT pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "S/W reset DAT Fail"  <<std::endl;
}
if(sw_host_reset(SW_CMD_LINE_RESET,0,0))
{
SD_LOG_DEBUG() << "S/W reset CMD pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "S/W reset CMD Fail"  <<std::endl;
}

if(sw_host_reset(SW_ALL_LINE_RESET,0,0))
{
SD_LOG_DEBUG() << "S/W reset ALL pass" <<std::endl;
}
else
{
SD_LOG_DEBUG() << "S/W reset ALL Fail"  <<std::endl;
}
#endif
//END_LINE:
//result1 = system ("devcon enable *dev_7813");
result1 = system ("devcon enable *dev_7906");
#if 0
sd_host_register_def->sd_reg.value		= 0x00;
sd_host_register_def->sd_reg.offset		= 0x08;
sd_host_register_def->sd_reg.width		= 0x04;
sd_host_register_def->sd_reg.device		= 0x00;
sd_host_register_def->sd_reg.slot		= 0x00;
result = read_host_reg(sd_host_register_def);
SD_LOG_DEBUG() << "SD register 0x08 = " << std::hex << sd_host_register_def->sd_reg.value <<" result  : " << result<<std::endl;


		uint32_t value;
		size_t offset;
		uint8_t width;
		uint32_t device;
		uint32_t slot;
bool init_host(void);
bool read_host_reg(_reg*);
bool write_host_reg(_reg*);
bool read_pcicfg(_reg*);
bool write_pcicfg(_reg*);
uint8_t get_total_pcidevice(void);
bool get_pciheader(_pci_type0_header*, uint32_t);
bool push_command(_command*);
bool send_command(void);

#endif
		
		
		
#if 0
	std::string                 args ;
    //int                         status = 0 ;
    //env::sys::Device            system1;
    //env::pci::Client            client1((*system1.pci().device(env::pci::Location(0,3,0,0)))) ;
    uint32_t                    data = 0x10 ;
    uint32_t                    i ;

	env::sys::Device            system2;
    env::pci::Client            client2((*system2.pci().device(env::pci::Location(0,0,20,7)))) ;
 
	for(i=0; i<10; ++i)
    {
        //data = client2.cfgr32(i); 
        std::cout << "Config space " << std::dec << i*4 << " = " << std::hex << std::showbase << data << std::endl ;
    }
 
	for(i=0; i<=255; i = i+4)
    {
        data = client2.cfgr32(i); 
        std::cout << "Config space " << std::dec << i << " = " << std::hex << std::showbase << data << std::endl ;
    }


unsigned short bus_1 = 0x00,device_1 = 0x00,function = 0x00;	
bool status = false;
		for( bus_1 = 0x00 ; bus_1 <= 0x02 ; bus_1++)
		{
			for(device_1=0x00 ; device_1 <= 0x18 ; device_1++)
			{
				for(function = 0x01 ; function <= 0x07 ; function++)
				{
					status = find_device(bus_1,device_1,function);
					if(status == true)
					{
							{
							try
							{
							 env::sys::Device *system3 = new (env::sys::Device);
							 env::pci::Client *client3 = new env::pci::Client((system3->pci().device(env::pci::Location(0,bus_1,device_1,function))));
							 
							//	new env::sys::Device system3;
								
								//new env::pci::Client client3((*system3.pci().device(env::pci::Location(0,bus_1,device_1,function)))) ;
								data = client3->cfgr8(0xA);
								if(((int32_t)SD_DEVICE_CLASS) == (int32_t)data)
								{
									std::cout << "Device Found Bus : "<< std::hex << bus_1 <<" Device :"<< std::hex << device_1 <<" Function :"<< std::hex << function <<" Class Code :"<< std::hex << data << std::endl ;						
									for(i=0; i<=255; i = i+4)
									{
										data = client3->cfgr32(i); 
										std::cout << "Config space " << std::dec << i << " = " << std::hex << std::showbase << data << std::endl ;
									}
								}
							}
							
							catch(std::runtime_error* e)
							{
							delete system3;
							delete client3;
							continue;
							}
							}
					}
				}
			}
		}

		
	try
    {
		env::pci::Client client3((*system2.pci().device(env::pci::Location(0,3,0,0)))) ;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Catching the exception here "e.what() << std::endl ;
    }




	#if 0


		_pci_handler *ptr_test_handler = new _pci_handler;

		SD_LOG_DEBUG() << "SD Bus vendor_id = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.vendor_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus device_id = " << std::hex << (ptr_test_handler->node[0].pci_header.device_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus command = " << std::hex << (ptr_test_handler->node[0].pci_header.command) << std::endl;
		SD_LOG_DEBUG() << "SD Bus status = " << std::hex << (ptr_test_handler->node[0].pci_header.status) << std::endl;
		SD_LOG_DEBUG() << "SD Bus revision_id = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.revision_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus classcode = " << std::hex << (ptr_test_handler->node[0].pci_header.classcode) << std::endl;
		SD_LOG_DEBUG() << "SD Bus cachelinesize = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.cachelinesize) << std::endl;
		SD_LOG_DEBUG() << "SD Bus latencytimer = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.latencytimer) << std::endl;
		SD_LOG_DEBUG() << "SD Bus headertype = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.headertype) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bist = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.bist) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_test_handler->node[0].pci_header.bar0) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_test_handler->node[0].pci_header.bar1) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_test_handler->node[0].pci_header.bar2) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_test_handler->node[0].pci_header.bar3) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_test_handler->node[0].pci_header.bar4) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_test_handler->node[0].pci_header.bar5) << std::endl;
		SD_LOG_DEBUG() << "SD Bus slotinfo = " << std::hex << (uint16_t)(ptr_test_handler->node[0].pci_header.slotinfo) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bus = " << std::hex << (uint16_t)(ptr_test_handler->node[0].bus) << std::endl;
		SD_LOG_DEBUG() << "SD Bus device = " << std::hex << (uint16_t)(ptr_test_handler->node[0].device) << std::endl;
		SD_LOG_DEBUG() << "SD Bus function = " << std::hex << (uint16_t)(ptr_test_handler->node[0].function) << std::endl;
		SD_LOG_DEBUG() << "SD Bus configured = " << std::hex << (uint16_t)(ptr_test_handler->node[0].configured) << std::endl;
		SD_LOG_DEBUG() << "SD Bus total_pci = " << std::hex << (uint16_t)(ptr_test_handler->total_pci) << std::endl;

		
		_pci_type0_header *ptr_header = new _pci_type0_header;
		result  = ptr_test_handler->get_pci_header(ptr_header, 0);
		if(	result == true)
		{
		SD_LOG_DEBUG() << "SD Bus vendor_id = "   << std::hex << (uint16_t)(ptr_header->vendor_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus device_id = "   << std::hex << (ptr_header->device_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus command = "     << std::hex << (ptr_header->command) << std::endl;
		SD_LOG_DEBUG() << "SD Bus status = "      << std::hex << (ptr_header->status) << std::endl;
		SD_LOG_DEBUG() << "SD Bus revision_id = " << std::hex << (uint16_t)(ptr_header->revision_id) << std::endl;
		SD_LOG_DEBUG() << "SD Bus classcode = "   << std::hex << (ptr_header->classcode) << std::endl;
		SD_LOG_DEBUG() << "SD Bus cachelinesize = " << std::hex << (uint16_t)(ptr_header->cachelinesize) << std::endl;
		SD_LOG_DEBUG() << "SD Bus latencytimer = " << std::hex << (uint16_t)(ptr_header->latencytimer) << std::endl;
		SD_LOG_DEBUG() << "SD Bus headertype = " << std::hex << (uint16_t)(ptr_header->headertype) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bist = " << std::hex << (uint16_t)(ptr_header->bist) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_header->bar0) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_header->bar1) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_header->bar2) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_header->bar3) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_header->bar4) << std::endl;
		SD_LOG_DEBUG() << "SD Bus bar0 = " << std::hex << (ptr_header->bar5) << std::endl;
		SD_LOG_DEBUG() << "SD Bus slotinfo = " << std::hex << (uint16_t)(ptr_header->slotinfo) << std::endl;

		}
	
#endif		


#endif	

       // _pci_handler pci_handler;
		//pci_handler.search_sd();
		//pci_handler.get_pci_node(ptrnode);
		
		//SD_LOG_DEBUG() << "SD Bus = " << std::hex << node1.bus << std::endl;
		//SD_LOG_DEBUG() << "SD Device = " << std::hex << node1.device << std::endl;
		//SD_LOG_DEBUG() << "SD Function = " << std::hex << node1.function << std::endl;
		//SD_LOG_DEBUG() << "SD configured = " << std::hex << node1.configured << std::endl;
        return TestResult;
   }

};

TServerTestInstance(SDTest, SDTest);
#if 0
TServerTestInstance(SBSDSDTest, SDTest);
#endif
#endif
