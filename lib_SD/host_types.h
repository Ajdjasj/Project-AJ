
#ifndef _HOST_TYPES_H_
#define _HOST_TYPES_H_
#include <iostream>


#define SD_MMIO 1
#define SD_PCI 0

//#if SD_PCI
	#include "env_pci.h"
//#endif

//#pragma pack(push)
//#pragma pack(1)
#define BYTE  0x01
#define WORD  0x02
#define DWORD 0x04
#define QWORD 0x08


#define GET_CMD_INHIBIT_CMD_LINE(x)				(x.bit.b0)
#define GET_CMD_INHIBIT_DAT_LINE(x)				(x.bit.b1)

#define GET_ERROR_INT(x)				(x.bit.b15)
#define GET_ERROR_INT_STATUS(x)		    (x.reg_val >> 16)
#define GET_COMMAND_COMPLETE(x)			(x.bit.b0)
#define GET_TRANSFER_COMPLETE(x)		(x.bit.b1)
#define GET_DMA_COMPLETE(x)				(x.bit.b3)


#define SW_DAT_LINE_RESET 	0x02
#define SW_CMD_LINE_RESET 	0x01
#define SW_ALL_LINE_RESET 	0x00

#define SD_VOLTAGE_3_3		0x0E
#define SD_VOLTAGE_3_0		0x0C
#define SD_VOLTAGE_1_8		0x0A

#define GET_CARD_INSERTED(x)			(x.bit.b16)
#define GET_CARD_PROTECTED(x)			(x.bit.b19)
#define GET_VOLTAGE_SUPPORT_3_3(x)		(x.bit.b24)
#define GET_VOLTAGE_SUPPORT_3_0(x)		(x.bit.b25)
#define GET_VOLTAGE_SUPPORT_1_8(x)		(x.bit.b26)


#define CMD_COMPLETE_INT 		(0x00000001 << 0)
#define TRNS_COMPLETE_INT 		(0x00000001 << 1)
#define DMA_COMPLETE_INT 		(0x00000001 << 3)
#define BLOCK_GAP_INT 			(0x00000001 << 2)


#define INT_STATUS_ENABLE 0X33FF00FF
#define INT_SIGNAL_ENABLE 0X00000000

#define DEFAULT_SPEED 	0x0001
#define HIGH_SPEED 		0x0002
#define SDR_12			DEFAULT_SPEED
#define SDR_25			HIGH_SPEED
#define SDR_50 			0x0004
#define SDR_104 		0x0008
#define DDR_50			0x0010
#define DEFAULT_FREQUENCY    0xFF  
#define DEFAULT_MODE	0x00
#define DS_MAX_FREQ			25
#define HS_MAX_FREQ			50
#define SDR12_MAX_FREQ		DS_MAX_FREQ	
#define SDR25_MAX_FREQ		HS_MAX_FREQ
#define SDR50_MAX_FREQ		100	
#define SDR104_MAX_FREQ		208
#define DDR50_MAX_FREQ		50

#define HOST_SPEC_VERSION_1_00	0x00
#define HOST_SPEC_VERSION_2_00	0x01
#define HOST_SPEC_VERSION_3_00	0x02

#define PROGRAMABLE_CLK_SELECTION_MODE	0x00
#define DIVIDED_CLK_SELECTION_MODE 		0x01
#define PRESET_CLK_SELECTION_MODE		0x02

//Init status defines
#define CARD_TYPE_MEM 	0x01
#define CARD_TYPE_SDIO 	0x02
#define CARD_TYPE_COMBO 0x03


//Error injection
#define LESS_BLOCK_SIZE 				0x00
#define CARD_ADDRESS_ERROR  			0x01
#define DATA_WIDTH_MISMATCH				0x02
#define AUTO_CMD23_WITHOUT_BLOCK_COUNT 	0x03
#define WRONG_CMD_INDEX					0x04
#define CLOCK_ERROR						0x05


typedef union
{
	struct{
			uint64_t value;
			size_t offset;
			uint8_t width;
			uint32_t device;
		  }pci_or_mmio_reg;
		  
	 
	struct{
			uint64_t value;
			size_t offset;
			uint8_t width;
			uint32_t device;
			uint32_t slot;
		  }sd_reg;	 
}_reg;

typedef union 
{
	struct 
	{
		uint16_t response_type:2;
		uint16_t rsvd1:1;
		uint16_t crc_check:1;
		uint16_t index_check:1;
		uint16_t data_present:1;
		uint16_t cmd_type:2;
		uint16_t index:6;
		uint16_t rsvd2:2;
	}bit;
	uint16_t reg_val;
}_cmd_reg;

typedef struct 
{
	uint64_t total_byte;
	uint32_t address;
}sdma_red_datatype;

typedef union
{
	struct 
	{
		uint8_t check_pattern;
		uint8_t vhs:4;
		uint32_t rsvd1:20;
	}bit;
	uint32_t reg_val;
}_r7;
typedef union
{
	struct 
	{
		uint8_t manufature_testmode:2;
		uint8_t app_specific:1;
		uint8_t ake_seq_error:1;
		uint8_t sdio:1;
		uint8_t app_cmd:1;
		uint8_t rsvd1:2;
		uint8_t rdy_for_data:1;
		uint8_t current_state:4;
		uint8_t erase_reset:1;
		uint8_t card_ecc_disable:1;
		uint8_t wp_erase_skip:1;
		uint8_t csd_overwrite:1;
		uint8_t rsvd2:2;
		uint8_t error:1;
		uint8_t cc_error:1;
		uint8_t card_ecc_failed:1;
		uint8_t illegal_command:1;
		uint8_t com_crc_error:1;
		uint8_t lock_unlock_fail:1;
		uint8_t card_is_locked:1;
		uint8_t wp_violation:1;
		uint8_t erase_param:1;
		uint8_t erase_seq_error:1;
		uint8_t block_len_error:1;
		uint8_t address_error:1;
		uint8_t out_of_range:1;
	}bit;
	uint32_t reg_val;
}_r1;
typedef union
{
	struct 
	{
		uint8_t rsvd1:7;
		uint8_t low_voltage:1;
		uint8_t rsvd2:7;
		uint8_t v2_7v_to_v2_8:1;
		uint8_t v2_8v_to_v2_9:1;
		uint8_t v2_9v_to_v3_0:1;
		uint8_t v3_0v_to_v3_1:1;
		uint8_t v3_1v_to_v3_2:1;
		uint8_t v3_2v_to_v3_3:1;
		uint8_t v3_3v_to_v3_4:1;
		uint8_t v3_4v_to_v3_5:1;
		uint8_t v3_5v_to_v3_6:1;
		uint8_t s18a:1;
		uint8_t rsvd3:5;
		uint8_t ccs:1;
		uint8_t busy:1;
	}bit;
	uint32_t reg_val;
}_r3;
typedef union
{
	struct 
	{
		uint8_t rsvd1;
		uint8_t v2_0v_to_v2_1:1;
		uint8_t v2_1v_to_v2_2:1;
		uint8_t v2_2v_to_v2_3:1;
		uint8_t v2_3v_to_v2_4:1;
		uint8_t v2_4v_to_v2_5:1;
		uint8_t v2_5v_to_v2_6:1;
		uint8_t v2_6v_to_v2_7:1;
		uint8_t v2_7v_to_v2_8:1;
		uint8_t v2_8v_to_v2_9:1;
		uint8_t v2_9v_to_v3_0:1;
		uint8_t v3_0v_to_v3_1:1;
		uint8_t v3_1v_to_v3_2:1;
		uint8_t v3_2v_to_v3_3:1;
		uint8_t v3_3v_to_v3_4:1;
		uint8_t v3_4v_to_v3_5:1;
		uint8_t v3_5v_to_v3_6:1;
		uint8_t s18a:1;
		uint8_t rsvd3:2;
		uint8_t mem_present:1;
		uint8_t no_io_function:3;
		uint8_t c:1;
	}bit;
	uint32_t reg_val;
}_r4;

typedef union
{
	struct 
	{
		uint8_t data:8;
		uint8_t flag:8;
		uint16_t rsvd:16;
	}bit;
	uint32_t reg_val;
}_r5;



typedef union
{
	struct 
	{
		uint8_t manufature_testmode:2;
		uint8_t app_specific:1;
		uint8_t ake_seq_error:1;
		uint8_t sdio:1;
		uint8_t app_cmd:1;
		uint8_t rsvd1:2;
		uint8_t rdy_for_data:1;
		uint8_t current_state:4;
		uint8_t error:1;
		uint8_t illegal_command:1;
		uint8_t com_crc_error:1;
		uint16_t rca;
	}bit;
	uint32_t reg_val;
}_r6;

class _r2{
public:
	uint64_t rhs64bit;
	uint64_t lhs64bit;
	void insert(uint64_t trhs64bit, uint64_t tlhs64bit){rhs64bit = trhs64bit;lhs64bit = tlhs64bit;}
	uint64_t get_rhs(){return rhs64bit;}
	uint64_t get_lhs(){return lhs64bit;}
	/*CID register bitfields*/
	uint16_t mdt()		{return(rhs64bit & 0x0fff);}
	uint8_t  cid_rsvd1()	{return((rhs64bit >> 12) & 0x000f);}
	uint32_t psn()		{return((rhs64bit >> 16) & 0xffffffff);}
	uint16_t prv()		{return((rhs64bit >> 48) & 0xff);}
	uint64_t pnm()		{return(((rhs64bit >> 56) & 0xff) | ((lhs64bit & 0xffffffff) << 8));}
	uint16_t oid()		{return((lhs64bit >> 32) & (0xffff));}
	uint16_t mid()		{return((lhs64bit >> 48) & (0xff));}
	uint64_t rhs()		{return rhs64bit;}
	uint64_t lhs()		{return lhs64bit;}
	/*CSD register bitfields*/
	uint8_t csd_rsvd1()					{return(rhs64bit & 0x03);}
	uint8_t file_format()			{return((rhs64bit >> 2) & 0x03);}
	uint8_t tmp_write_protect()		{return((rhs64bit >> 4) & 0x01);}
	uint8_t perm_write_protect()	{return((rhs64bit >> 5) & 0x01);}
	uint8_t copy()					{return((rhs64bit >> 6) & 0x01);}
	uint8_t file_format_grp()		{return((rhs64bit >> 7) & 0x01);}
	uint8_t rsvd2()					{return((rhs64bit >> 8) & 0x01F);}
	uint8_t write_bl_partial()		{return((rhs64bit >> 13) & 0x01);}
	uint8_t write_bl_length()		{return((rhs64bit >> 14) & 0x0F);}
	uint8_t r2w_factor()			{return((rhs64bit >> 18) & 0x07);}
	uint8_t rsvd3()					{return((rhs64bit >> 21) & 0x3);}
	uint8_t wp_grp_enable()			{return((rhs64bit >> 23) & 0x01);}
	uint8_t wp_grp_size()			{return((rhs64bit >> 24) & 0x07F);}
	uint8_t sector_size()			{return((rhs64bit >> 31) & 0x07F);}
	uint8_t erase_block_enable()	{return((rhs64bit >> 38) & 0x01);}
	
	uint8_t c_size_mult()			{return((rhs64bit >> 39) & 0x07);}
	uint8_t vdd_w_curr_max()		{return((rhs64bit >> 42) & 0x07);}
	uint8_t vdd_w_curr_min()		{return((rhs64bit >> 45) & 0x07);}
	uint8_t vdd_r_curr_max()		{return((rhs64bit >> 48) & 0x07);}
	uint8_t vdd_r_curr_min()		{return((rhs64bit >> 51) & 0x07);}
	uint32_t c_size()				{return( 0x00 == csd_structure() ? ((rhs64bit >> 54) | ((lhs64bit & 0x03) << 10)) : (((rhs64bit >> 40) & 0x3FFFFF)));}
	uint8_t rsvd4()					{return((lhs64bit >> 2) & 0x03);}
	
	uint8_t dsr_imp()				{return((lhs64bit >> 4) & 0x01);}
	uint8_t read_blk_misalign()		{return((lhs64bit >> 5) & 0x01);}
	uint8_t write_blk_misalign()	{return((lhs64bit >> 6) & 0x01);}
	uint8_t read_bl_partial()		{return((lhs64bit >> 7) & 0x01);}
	uint8_t read_bl_length()		{return((lhs64bit >> 8) & 0x0F);}
	uint16_t ccc()					{return((lhs64bit >> 12) & 0x0FFF);}
	uint16_t trans_speed()			{return((lhs64bit >> 24) & 0x0FF);}
	uint16_t nsac()					{return((lhs64bit >> 32) & 0x0FF);}
	uint16_t taac()					{return((lhs64bit >> 40) & 0x0FF);}
	uint8_t rsvd5()					{return((lhs64bit >> 48) & 0x03F);}
	uint8_t csd_structure()			{return((lhs64bit >> 54) & 0x03);}
};

class _scr{
public:
	uint64_t scr_value;
	void insert(uint64_t tscr){scr_value = tscr;}
	/*SCR register bitfields*/
	uint8_t scr_structure()				{return((scr_value >> 60) & 0x000000000000000f);}
	uint8_t sd_spec()	    			{return((scr_value >> 56) & 0x000000000000000f);}
	uint8_t data_stst_after_erase()	    {return((scr_value >> 55) & 0x0000000000000001);}
	uint8_t sd_security()				{return((scr_value >> 52) & 0x0000000000000007);}
	uint8_t sd_bus_width()	    		{return((scr_value >> 48) & 0x000000000000000f);}
	uint8_t sd_spec3()	   				{return((scr_value >> 47) & 0x0000000000000001);}
	uint8_t ex_security()				{return((scr_value >> 43) & 0x000000000000000f);}
	uint8_t cmd_support()  		        {return((scr_value >> 32) & 0x0000000000000003);}
};

struct _function
{
	uint16_t maxCurrent;
	uint16_t group1;
	uint16_t group2;
	uint16_t group3;
	uint16_t group4;
	uint16_t group5;
	uint16_t group6;
	uint16_t current_grp1;
	uint16_t current_grp2;
	uint16_t current_grp3;
	uint16_t current_grp4;
	uint16_t current_grp5;
	uint16_t current_grp6;
	uint8_t struct_version;
	uint16_t busy_grp1;
	uint16_t busy_grp2;
	uint16_t busy_grp3;
	uint16_t busy_grp4;
	uint16_t busy_grp5;
	uint16_t busy_grp6;
};

struct _sd_status
{
	uint8_t data_bus_width;
	bool secured_mode;
	uint8_t security_funtions;
	uint16_t sd_card_type;
	uint32_t size_of_protected_area;
	uint8_t speed_class;
	uint8_t performance_move;
	uint8_t au_size;
	uint16_t erase_size;
	uint8_t erase_timeout;
	uint8_t erase_offset;
	uint8_t uhs_speed_grade;
	uint8_t uhs_au_size;
};

typedef struct
{
	_cmd_reg cmd;
	uint32_t arg;
	uint32_t response_flag;
	uint32_t error;
	uint32_t retries;
	uint32_t device_number;
	uint32_t slot;
	_r1 r1;
	_r2 r2;
	_r3 r3;
	_r4 r4;
	_r5 r5;
	_r6 r6;
	_r7 r7;
//	_r1b r1b;
//	_r2b r2b;
}_command;


typedef struct
{
	struct _init_host
	{
		uint8_t version;
		bool voltage_switch;
		bool ddr50;
		bool sdr12;
		bool sdr25;
		bool sdr50;
		bool sdr104;
		bool hs;
	}host;
	struct _init_card
	{
		bool voltage_switch;
		bool sdhc_sdxc;;
		_r4 sdio_ocr;
		_r3 mem_ocr;
		_r2 cid;
		uint16_t rca;
	}card;	
	struct _init_result
	{
		bool voltage_switch;
		bool sdio_init;
		bool mem_init;
		bool card_unusable;
	}result;
}_init_status;


typedef union 
{
	struct
	{
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
	}bit;
	uint8_t reg_val;
}reg8;

typedef union 
{
	struct
	{
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
		uint8_t b8:1;
		uint8_t b9:1;
		uint8_t b10:1;
		uint8_t b11:1;
		uint8_t b12:1;
		uint8_t b13:1;
		uint8_t b14:1;
		uint8_t b15:1;
	}bit;
	uint16_t reg_val;
}reg16;

typedef union 
{
	struct
	{
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
		uint8_t b8:1;
		uint8_t b9:1;
		uint8_t b10:1;
		uint8_t b11:1;
		uint8_t b12:1;
		uint8_t b13:1;
		uint8_t b14:1;
		uint8_t b15:1;
		uint8_t b16:1;
		uint8_t b17:1;
		uint8_t b18:1;
		uint8_t b19:1;
		uint8_t b20:1;
		uint8_t b21:1;
		uint8_t b22:1;
		uint8_t b23:1;
		uint8_t b24:1;
		uint8_t b25:1;
		uint8_t b26:1;
		uint8_t b27:1;
		uint8_t b28:1;
		uint8_t b29:1;
		uint8_t b30:1;
		uint8_t b31:1;
	}bit;
	uint32_t reg_val;
}reg32;

typedef union 
{
	struct
	{
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
		uint8_t b8:1;
		uint8_t b9:1;
		uint8_t b10:1;
		uint8_t b11:1;
		uint8_t b12:1;
		uint8_t b13:1;
		uint8_t b14:1;
		uint8_t b15:1;
		uint8_t b16:1;
		uint8_t b17:1;
		uint8_t b18:1;
		uint8_t b19:1;
		uint8_t b20:1;
		uint8_t b21:1;
		uint8_t b22:1;
		uint8_t b23:1;
		uint8_t b24:1;
		uint8_t b25:1;
		uint8_t b26:1;
		uint8_t b27:1;
		uint8_t b28:1;
		uint8_t b29:1;
		uint8_t b30:1;
		uint8_t b31:1;
		uint8_t b32:1;
		uint8_t b33:1;
		uint8_t b34:1;
		uint8_t b35:1;
		uint8_t b36:1;
		uint8_t b37:1;
		uint8_t b38:1;
		uint8_t b39:1;
		uint8_t b40:1;
		uint8_t b41:1;
		uint8_t b42:1;
		uint8_t b43:1;
		uint8_t b44:1;
		uint8_t b45:1;
		uint8_t b46:1;
		uint8_t b47:1;
		uint8_t b48:1;
		uint8_t b49:1;
		uint8_t b50:1;
		uint8_t b51:1;
		uint8_t b52:1;
		uint8_t b53:1;
		uint8_t b54:1;
		uint8_t b55:1;
		uint8_t b56:1;
		uint8_t b57:1;
		uint8_t b58:1;
		uint8_t b59:1;
		uint8_t b60:1;
		uint8_t b61:1;
		uint8_t b62:1;
		uint8_t b63:1;
	}bit;
	uint64_t reg_val;
}reg64;



//#pragma pack(pop)
#endif
