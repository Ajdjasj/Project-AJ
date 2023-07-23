#ifndef _SD2_0_CARD_H_
#define _SD2_0_CARD_H_
/*
 *  Secure Digital Host Controller Interface driver
 *  Header file for Card specific registers/Commands.
 *  Copyright (C) AMD, All Rights Reserved.
 *  AMD SB CHPSET : BOLTON, SDHC
 *  SD 2.0 Physical layer specification
 */

/*  
 * SD 2.0 Card register
 */
/*
 * End of controller .
 */

/*  
 * SD 2.0 Card Command
 */

 #include "host_types.h"
 #include "sd_wrapper.h"
 #include "core_logger.h"

 #define NUMBER_OF_WAIT_FOR_CMD_DAT_INHIBIT  0x05
 void FRAME_CMD(_command* pcmd, uint16_t index,uint16_t cmd_type,bool data_present,bool index_check, bool crc_check, uint16_t response_type, uint32_t flag , uint32_t retries,uint32_t arg, uint32_t device, uint32_t slot);
enum CMD{
//Basic Commands
CMD_START = -1, // Start Indicator Dont Delete
CMD_0	= 0,
CMD_2	= 2,
CMD_3	= 3,
CMD_4	= 4,
CMD_5	= 5,
CMD_6	= 6,
CMD_7	= 7,
CMD_8	= 8,
CMD_9	= 9,
CMD_10 	=10,
CMD_11	=11,
CMD_12	=12,
CMD_13	=13,
CMD_14	=14,
CMD_15 	=15,
//Block Oriented Read Command - Class 2
CMD_16,
CMD_17,
CMD_18,
CMD_19,
CMD_20,
CMD_23	=23,
// Block Oriented write command Class 4
CMD_24,
CMD_25,
CMD_27	=27,
// Block Oriented Write protection Command - Class 6
CMD_28,
CMD_29,
CMD_30,
//Erase Command - Class 5
CMD_32	= 32,
CMD_33,
CMD_38	= 38,
//Lock Command - Clas 7
CMD_40	= 40,
CMD_42	= 42,
//SDIO Read Write
CMD_52	= 52,
CMD_53,
CMD_54,
//Application Specific command - Class 9
CMD_55,
CMD_56,
CMD_END //End Indicator Dont delete
};

enum _ACMD
{
ACMD_6 	= 6,
ACMD_13 = 13,
ACMD_22	= 22,
ACMD_23	= 23,
ACMD_41	= 41,
ACMD_42	= 42,
ACMD_51	= 51
};

enum _cmdtype
{
normal_cmd 	= 0,
suspend_cmd = 1,
resume_cmd	= 2,
abort_cmd	= 3
};

enum _responsetype
{
noresponse 		= 0,
response136		= 1,
response48		= 2,
response48_busy	= 3
};
enum current_state
{
idle = 0,
ready,
ident,
stby,
tran,
data,
rcv,prg,
dis,
};
#define R1  0x00
#define R1b 0x01
#define R2  0x02
#define R3  0x03
#define R4  0x04
#define R5  0x05
#define R6  0x06
#define R7  0x07
#define ENABLE	0x01
#define DISABLE 0x00
class _cmd
{
public:
_command *p_cmd_ringbuffer[10];
uint32_t cmd_next;
uint32_t cmd_current;

bool insert_cmd_queue(_command *cmd);
bool send_cmd(uint32_t device , uint32_t slot);

bool cmd_complete(uint32_t device , uint32_t slot , uint32_t *_error);
bool trns_complete(uint32_t device , uint32_t slot , uint32_t *_error,uint32_t retry);
bool dma_complete(uint32_t device , uint32_t slot , uint32_t *_error);
bool block_gap(uint32_t device , uint32_t slot , uint32_t *_error);
bool int_status(uint32_t type , uint32_t device , uint32_t slot , uint32_t *_error , uint32_t retry);
_cmd();
~_cmd();

};

class _card
{
public:
_r3 *ocr;
_r4 *sdio_ocr;
_r2 *cid;
_r2 *csd;
_r1 *card_status;
uint16_t *rca;
uint16_t *dsr;
_scr *scr;
_cmd *cmd;
uint32_t device;
uint32_t slot;
_card(uint32_t,uint32_t);
~_card();
bool send_command(_command *cmd);
bool send_appcommand(_command *pcmd);
void reset_card_datastruct();
bool cmd_complete(uint32_t *_error);
bool trns_complete(uint32_t *_error,uint32_t retry);
bool dma_complete(uint32_t *_error);
bool block_gap(uint32_t *_error);
bool go_idle(uint32_t *error);
bool get_all_cid(_r2 *ptr_cid,uint32_t *error);
bool get_cid(_r2*,uint32_t *error);
bool get_rca(uint16_t* ptr_rca,uint32_t *error);
bool get_rca_r6(uint16_t* ptr_rca,uint32_t *error,_r6 *rca_status);
bool send_if_cond(uint8_t vhs,_r7* ptr_r7,uint32_t *error);
bool get_ocr(_r3*,uint32_t *error);
bool get_scr(_r1 *status,uint32_t *error);
bool send_op_cond(bool s18r,bool hcs,_r3* ptr_ocr,uint32_t *error);
bool get_csd(_r2* ptr_csd,uint32_t *error);
bool get_card_status(_r1 *ptr_cardstatus,uint32_t *error);
bool select_card(_r1 *status,uint32_t *error);
bool deselect_card(_r1 *status,uint32_t *error);
bool set_buswidth(uint8_t width,_r1 *status,uint32_t *error);
bool set_blocklength(uint32_t blocklength,_r1 *status,uint32_t *error);
bool read_singleblock(uint32_t address,_r1 *status,uint32_t *error);
bool read_multiblock(uint32_t address,_r1 *status,uint32_t *error);
bool write_singleblock(uint32_t address,_r1 *status,uint32_t *error);
bool write_multiblock(uint32_t address,_r1 *status,uint32_t *error);
bool set_blockcount(uint32_t blockcount,_r1 *status,uint32_t *error);
bool go_inactive(uint32_t *error);
bool erase_start_add(uint32_t address,_r1 *status,uint32_t *error);
bool erase_end_add(uint32_t address,_r1 *status,uint32_t *error);
bool erase(_r1 *status,uint32_t *error);
bool lock_unlock(_r1 *status,uint32_t *error);
bool switch_function(_r1 *status, uint32_t argument,uint32_t *error);
bool voltage_switch(_r1 *status,uint32_t *error);
bool get_sd_status(_r1 *status,uint32_t *error);
bool get_sdio_ocr(_r4* ptr_ocr,uint32_t *error);
bool sdio_send_op_cond(bool s18r,_r4* ptr_ocr,uint32_t *error);
bool stop_transmission(_r1 *status,uint32_t *error);
bool send_tuning_block(_r1 *status,uint32_t *error);
bool send_num_wr_blocks(_r1 *status,uint32_t *error);
bool program_csd(_r1 *status,uint32_t *error);
//SDIO functions
bool send_sdio_command(uint32_t arg, uint32_t cmd, _r5 *status,uint32_t *error);
bool direct_io_read(uint32_t function,uint32_t reg_offset, _r5 *status,uint32_t *error);
bool direct_io_write(bool raw , uint32_t function,uint32_t reg_offset, uint32_t data ,_r5 *status,uint32_t *error);
bool extended_io_read(uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,bool block_mode, uint32_t count, _r5 *status,uint32_t *error);
bool extended_io_write(uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,bool block_mode, uint32_t count, _r5 *status,uint32_t *error);
bool write_multiblock_error(uint32_t address,_r1 *status,uint32_t *error);
};
class CardLogger
{
public:
    static coreutil::Logger& log() { return mLogger; }

private:
    static coreutil::Logger mLogger;

    CardLogger();
};

#define CARD_LOG_DEBUG() CORE_LOG_DEBUG(CardLogger::log())
#define CARD_LOG_INFO()  CORE_LOG_INFO(CardLogger::log())
#define CARD_LOG_WARN()  CORE_LOG_WARN(CardLogger::log())
#define CARD_LOG_ERROR() CORE_LOG_ERROR(CardLogger::log())


/*
 * End of controller
 */
#endif
