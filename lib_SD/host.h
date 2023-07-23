#ifndef _HOST_H_
#define _HOST_H_
#include <iostream>
#include <cstdio>
#include <cstdarg>

#include "sd_wrapper.h"

#include "card.h"
#include "host_types.h"
#include "core_logger.h"


	#include "env_pci.h"
	#include "sd_pci.h"
	//#include "pci_bolton.h" /* Include bolton specific register defines */
	#include "pci_kabini.h"


//get_card_status
//erase_data
//go_inactive
//command_complete
#define MAX_DATA_SIZE_PER_BUFFER 0x10000
class _host
{
	public:
	//_cmd *p_cmd;
	_card *p_card;
	
	#if SD_PCI
		_pci_handler *p_pci_handler;
		_host(uint32_t bus, uint32_t device, uint32_t function);
	#endif
	
	_host();
	
	~_host();

	bool reset(uint8_t , uint32_t , uint32_t );
	bool detect(uint32_t, uint32_t);
	bool writeprotected(uint32_t, uint32_t);
	bool set_voltage(uint8_t voltage, uint32_t device, uint32_t slot);
	bool power(bool ,uint32_t , uint32_t );
	bool host_init(uint32_t device, uint32_t slot);
	bool card_init(uint32_t device, uint32_t slot,bool voltage_switch, _init_status *init_status);
	bool set_clk(uint16_t, uint32_t, uint32_t);
	bool card_clk(bool, uint32_t, uint32_t);
	bool host_clk(bool , uint32_t ,uint32_t );
	bool set_buswidth(bool buswidth, uint32_t device, uint32_t slot);
	bool set_highspeed(bool speed, uint32_t device, uint32_t slot);
	bool set_uhspeedmode(uint32_t device, uint32_t slot, uint8_t speed_mode);
	bool set_transfermode(bool mode, bool direction, uint8_t autocmd, bool block_count, bool dma, uint32_t device, uint32_t slot);
	bool set_block_length_n_count(uint16_t length, uint16_t count,uint32_t device, uint32_t slot);
	bool set_timeout_ctrl(uint8_t timeout,uint32_t device, uint32_t slot);
	bool voltage_switch(uint32_t device, uint32_t slot);
	bool get_sd_status(uint32_t device , uint32_t slot , bool speed , bool width, _r1 *status , _sd_status *sd_status);
	bool int_status_enable(uint32_t device , uint32_t slot, uint32_t int_status);
	bool int_signal_enable(uint32_t device , uint32_t slot, uint32_t int_status);
	bool force_event(uint32_t device , uint32_t slot, uint32_t force);
	uint32_t get_int_status(uint32_t device , uint32_t slot);
	uint16_t get_autocmd_error(uint32_t device , uint32_t slot);
	uint8_t get_adma_error(uint32_t device , uint32_t slot);
	bool init_data_transfer(uint32_t device , uint32_t slot ,bool speed, bool bus_width,uint16_t blockcount , uint32_t block_length );
	bool transfer_complete( uint32_t *error,uint32_t retry);
	bool command_complete( uint32_t *error);
	bool dma_complete( uint32_t *error);
	bool block_gap(uint32_t *error);

	bool erase_data(uint32_t start_address,uint32_t end_address,_r1 *status);
	bool issue_switch_function(uint32_t device, uint32_t slot, bool speed,bool width,uint32_t argument,_r1 *status,uint8_t *ptr_buffer );
	bool lock_unlock(uint32_t device, uint32_t slot,uint8_t *lock_card_data,bool speed, bool bus_width ,_r1 *status);
	bool get_scr(uint32_t device , uint32_t slot, _scr *ptr_scr,_r1 *status,bool speed, bool width);
	bool get_supported_functions(uint32_t device,uint32_t slot, bool speed,bool width,_function *func, _r1 *status);
	bool switch_function(uint32_t device , uint32_t slot, bool speed,bool width,_function *func, _r1 *status);


	bool select_card( _r1 *status,uint32_t *error);
	bool get_csd(_r2* ptr_csd,uint32_t *error);
	bool get_ocr(_r3* ptr_ocr,uint32_t *error);
	bool go_inactive(uint32_t *error);
	bool dma_select(uint8_t value,uint32_t device, uint32_t slot);


	bool pio_write(uint32_t device, uint32_t slot,bool speed, bool bus_width,uint8_t autocmd , uint32_t card_address,uint8_t *ptr_buffer,uint32_t blk_size,uint32_t blk_count,bool block_gap_enable);
	bool pio_read(uint32_t device, uint32_t slot,bool speed, bool bus_width,uint8_t autocmd , uint32_t card_address,uint8_t *ptr_buffer,uint32_t blk_size,uint32_t blk_count,bool block_gap);
	bool sdma_write(uint32_t device , uint32_t slot , uint32_t card_address , uint32_t *phy_address , uint32_t mem_size, uint32_t blk_size, uint32_t blk_count, bool speed, bool width, uint8_t autocmd);
	bool sdma_read(uint32_t device , uint32_t slot , uint32_t card_address , uint32_t *phy_address , uint32_t mem_size, uint32_t blk_size, uint32_t blk_count, bool speed, bool width, uint8_t autocmd, sdma_red_datatype *datastruct);
	bool dma2_write(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_sel , bool block_gap_enable , bool asynchronus_abort);
	bool dma2_write_3_4_16(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_sel , bool block_gap_enable , bool asynchronus_abort, uint8_t* LockData);
	bool dma2_read(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_sel);

	bool set_sdma_buffer_boundry(uint32_t device, uint32_t slot , uint32_t boundry);

	bool asynchronous_abort(uint32_t device,uint32_t slot,_r1 *status,uint32_t *error);
	bool synchronous_abort(uint32_t device,uint32_t slot,_r1 *status,uint32_t *error);
	bool set_normal_mode(uint32_t device, uint32_t slot, uint16_t speed_mode, uint16_t clock_selection_mode);
	uint32_t calculate_freq_select(uint32_t device, uint32_t slot,bool clk_generator, uint16_t speed_mode);
	uint8_t get_spec_version(uint32_t device, uint32_t slot);
	uint8_t get_vendor_version(uint32_t device, uint32_t slot);
	bool set_clock(uint32_t device, uint32_t slot, uint32_t Max_freq, uint32_t clock_selection_mode);
	bool set_UHS_mode(uint32_t device, uint32_t slot, uint16_t speed_mode, uint16_t clock_selection_mode);


	bool check_tuning_valid(uint32_t device, uint32_t slot);
	bool issue_tuning(uint32_t device, uint32_t slot);
	bool tuning_procedure(uint32_t device,uint32_t slot);
	bool error_interrupt_recovery(uint32_t device, uint32_t slot,uint32_t error);
	
	#if SD_PCI
		void get_bus_device_function(uint32_t device , uint16_t *bus_number , uint16_t *device_number , uint16_t *function_number);
	#endif
	
	bool recover_normal_error(uint32_t device , uint32_t slot);
	bool datatransfer_error_inject(uint32_t device, uint32_t slot,bool speed , uint32_t error_type, bool data_transfer_direction,uint32_t *error);
	bool setup_tuning(uint32_t device,uint32_t slot);
	bool tune(uint32_t device,uint32_t slot);
	bool send_num_wr_blocks(_r1 *status, uint32_t *error);
	bool write_csd(uint32_t device, uint32_t slot,uint8_t *csd_data,bool speed, bool bus_width ,_r1 *status);
	bool write_csd_1_1_29(uint32_t device, uint32_t slot,uint8_t *csd_data,bool speed, bool bus_width ,_r1 *status);

	//SDIO functions
	bool direct_io_read(uint32_t function,uint32_t reg_offset, _r5 *status,uint32_t *error);
	bool direct_io_write(bool raw , uint32_t function,uint32_t reg_offset,uint32_t data , _r5 *status,uint32_t *error);
	bool extended_io_read(uint32_t device, uint32_t slot , uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,bool block_mode, uint32_t count,uint8_t *ptr_buffer, _r5 *status,uint32_t *error);
	bool extended_io_write(uint32_t device, uint32_t slot ,uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,bool block_mode, uint32_t count,uint8_t *ptr_buffer, _r5 *status,uint32_t *error);

};
/**********************With in Lib **********************/
bool read_buffer (uint32_t device, uint32_t slot , uint32_t blk_size ,uint32_t blk_count, uint8_t *ptr_buffer);
bool write_buffer(uint32_t device, uint32_t slot,  uint32_t blk_size ,uint32_t blk_count, uint8_t *ptr_buffer);
/**********************With in Lib **********************/

/**************************  API TO EXPORT ********************************/

bool read_host_reg(_reg*);
bool write_host_reg(_reg*);


bool init_driver(uint32_t bus, uint32_t device, uint32_t function);

#if SD_PCI
	
	uint64_t read_pcicfg(uint32_t device , uint32_t offset , uint32_t width);
	void write_pcicfg(uint32_t device , uint32_t offset , uint32_t width , uint64_t value);
	uint8_t get_total_pcidevice(void);
	uint8_t get_total_slot(uint32_t device);
	bool get_pciheader(_pci_type0_header*, uint32_t);
#endif



bool sw_host_reset(uint8_t, uint32_t, uint32_t );
bool detect_card(uint32_t , uint32_t);
bool card_writeprotected(uint32_t , uint32_t );
bool set_card_voltage(uint8_t voltage, uint32_t device, uint32_t slot);
bool card_power(bool bus_power_state, uint32_t device, uint32_t slot);
bool init_host(uint32_t device, uint32_t slot);
bool set_card_clock(uint16_t,uint32_t,uint32_t);
bool card_clock(bool,uint32_t,uint32_t );
bool host_clock(bool clk_state,uint32_t device,uint32_t slot);
bool init_card(uint32_t device, uint32_t slot,bool voltage_switch,_init_status *status);
bool issue_command(_command *pcmd);
bool issue_appcommand(_command *pcmd);
bool get_card_status( _r1 *ptr_cardstatus,uint32_t *error);
bool get_rca(uint16_t* ptr_rca,uint32_t *error);
bool get_all_cid(_r2 *ptr_cid,uint32_t *error);
bool go_idle(uint32_t *error);
bool send_if_cond(uint8_t vhs,_r7* ptr_r7,uint32_t *error);
bool get_ocr( _r3* ptr_ocr,uint32_t *error);
bool send_op_cond(bool s18r,bool hcs,_r3* ptr_ocr,uint32_t *error);
bool get_cid(_r2* ptr_cid,uint32_t *error);
bool get_csd( _r2* ptr_csd,uint32_t *error);
bool select_card(_r1 *status,uint32_t *error);
bool deselect_card(_r1 *status,uint32_t *error);
bool set_card_buswidth(uint8_t width,_r1 *status,uint32_t *error);
bool set_card_blocklength(uint32_t blocklength,_r1 *status, uint32_t *error);
bool set_card_blockcount(uint32_t blockcount,_r1 *status);
bool read_singleblock(uint32_t address,_r1 *status,uint32_t *error);
bool read_multiblock(uint32_t address,_r1 *status,uint32_t *error);
bool write_singleblock(uint32_t address,_r1 *status,uint32_t *error);
bool write_multiblock(uint32_t address,_r1 *status,uint32_t *error);
bool go_inactive(uint32_t *error);
bool set_host_buswidth(bool buswidth, uint32_t device, uint32_t slot);
bool set_speed(bool speed, uint32_t device, uint32_t slot);
bool set_transfermode(bool mode, bool direction, uint8_t autocmd, bool block_count, bool dma, uint32_t device, uint32_t slot);
bool set_host_block_length_n_count(uint16_t length, uint16_t count,uint32_t device, uint32_t slot);
bool set_timeout_ctrl(uint8_t timeout,uint32_t device, uint32_t slot);
bool transfer_complete( uint32_t *error, uint32_t retry);
bool command_complete( uint32_t *error);
bool dma_complete( uint32_t *error);


bool init_data_transfer(uint32_t device , uint32_t slot ,bool speed, bool bus_width ,uint16_t blockcount , uint32_t block_length );
bool erase_start_add(uint32_t address,_r1 *status,uint32_t *error);
bool erase_end_add(uint32_t address,_r1 *status,uint32_t *error);
bool erase(_r1 *status,uint32_t *error);
bool erase_cmd(_r1 *status,uint32_t *error);

bool erase_data(uint32_t start_address,uint32_t end_address,_r1 *status);
bool lock_unlock(uint32_t device, uint32_t slot,uint8_t *lock_card_data,bool speed, bool bus_width ,_r1 *status);
bool issue_lock_unlock_cmd(_r1 *status,uint32_t *error);
bool get_scr(uint32_t device , uint32_t slot, _scr *ptr_scr,_r1 *status,bool speed, bool width);
bool get_supported_functions(uint32_t device , uint32_t slot, bool speed,bool width,_function *func, _r1 *status);

bool issue_switch_function(uint32_t device, uint32_t slot, bool speed,bool width,uint32_t argument,_r1 *status,uint8_t *ptr_buffer );
bool switch_function(uint32_t device , uint32_t slot, bool speed,bool width,_function *func, _r1 *status);
void extract_switch_function_data(_function *func,uint8_t *buffer);
bool get_sd_status(uint32_t device , uint32_t slot , bool speed , bool width, _r1 *status , _sd_status *sd_status);
bool int_status_enable(uint32_t device , uint32_t slot, uint32_t int_status);
bool int_signal_enable(uint32_t device , uint32_t slot, uint32_t int_status);
bool force_event(uint32_t device , uint32_t slot, uint32_t force);
uint32_t get_int_status(uint32_t device , uint32_t slot);
uint16_t get_autocmd_error(uint32_t device , uint32_t slot);

bool tuning_procedure(uint32_t device,uint32_t slot);


bool set_normal_mode(uint32_t device, uint32_t slot, uint16_t speed_mode, uint16_t clock_selection_mode);
bool set_UHS_mode(uint32_t device, uint32_t slot, uint16_t speed_mode, uint16_t clock_selection_mode);

#if SD_PCI
	void get_bus_device_function(uint32_t device , uint16_t *bus_number , uint16_t *device_number , uint16_t *function_number);
#endif


bool write_multiblock_error(uint32_t address,_r1 *status,uint32_t *error);
/************************** API TO EXPORT END********************************/

bool pio_write	(uint32_t device , uint32_t slot ,bool speed, bool bus_width,uint8_t autocmd , uint32_t card_address,uint8_t *ptr_buffer,uint32_t blk_size,uint32_t blk_count);
bool pio_read	(uint32_t device , uint32_t slot ,bool speed, bool bus_width,uint8_t autocmd , uint32_t card_address,uint8_t *ptr_buffer,uint32_t blk_size,uint32_t blk_count);
bool sdma_read	(uint32_t device , uint32_t slot , uint32_t card_address , uint32_t *phy_address , uint32_t mem_size, uint32_t blk_size, uint32_t blk_count, bool speed, bool width, uint8_t autocmd,sdma_red_datatype *datastruct);
bool sdma_write	(uint32_t device , uint32_t slot , uint32_t card_address , uint32_t *phy_address , uint32_t mem_size, uint32_t blk_size, uint32_t blk_count, bool speed, bool width, uint8_t autocmd);
bool dma2_read(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_sel);
bool dma2_write(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_select);
bool dma2_write_3_4_16(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_sel, uint8_t* LockData);
bool dma2_write_with_block_gap(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_select);
bool asynchronous_abort(uint32_t device,uint32_t slot,_r1 *status,uint32_t *error);
bool synchronous_abort(uint32_t device,uint32_t slot,_r1 *status,uint32_t *error);
bool pio_write_with_block_gap(uint32_t device, uint32_t slot,bool speed, bool bus_width,uint8_t autocmd , uint32_t card_address,uint8_t *ptr_buffer,uint32_t blk_size,uint32_t blk_count);
bool dma2_write_with_asynchabort(uint32_t device , uint32_t slot , uint32_t card_address , uint64_t dma_table , uint32_t blk_size, uint64_t blk_count, bool speed, bool width, uint8_t autocmd, uint8_t dma_select);
bool pio_read_with_block_gap(uint32_t device, uint32_t slot,bool speed, bool bus_width,uint8_t autocmd , uint32_t card_address,uint8_t *ptr_buffer,uint32_t blk_size,uint32_t blk_count);

bool error_interrupt_recovery(uint32_t device, uint32_t slot,uint32_t error);
bool recover_normal_error(uint32_t device , uint32_t slot);
bool datatransfer_error_inject(uint32_t device, uint32_t slot,bool speed , uint32_t error_type, bool data_transfer_direction, uint32_t *error);
bool program_csd(_r1 *status,uint32_t *error);
bool set_csd(uint32_t device, uint32_t slot,uint8_t *csd_data,bool speed, bool bus_width ,_r1 *status);
bool set_csd_1_1_29(uint32_t device, uint32_t slot,uint8_t *csd_data,bool speed, bool bus_width ,_r1 *status);
//SDIO Funtions
bool direct_io_read(uint32_t function,uint32_t reg_offset, _r5 *status,uint32_t *error);
bool direct_io_write(bool raw , uint32_t function,uint32_t reg_offset,uint32_t data , _r5 *status,uint32_t *error);
bool extended_io_byte_read(uint32_t device, uint32_t slot , uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,bool block_mode, uint32_t count,uint8_t *ptr_buffer, _r5 *status,uint32_t *error);
bool extended_io_byte_write(uint32_t device, uint32_t slot ,uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,bool block_mode, uint32_t count,uint8_t *ptr_buffer, _r5 *status,uint32_t *error);
#if 0
	bool extended_io_block_read(uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address,uint32_t count, _r5 *status,uint32_t *error);
	bool extended_io_block_write(uint32_t function, uint32_t reg_offset, bool fixe_or_inc_address, uint32_t count, _r5 *status,uint32_t *error);
#endif

bool set_clock(uint32_t device, uint32_t slot, uint32_t Max_freq, uint32_t clock_selection_mode);
//transfer_complete
class HostLogger
{
public:
    static coreutil::Logger& log() { return mLogger; }

private:
    static coreutil::Logger mLogger;

    HostLogger();
};

#define HOST_LOG_DEBUG() CORE_LOG_DEBUG(HostLogger::log())
#define HOST_LOG_INFO()  CORE_LOG_INFO(HostLogger::log())
#define HOST_LOG_WARN()  CORE_LOG_WARN(HostLogger::log())
#define HOST_LOG_ERROR() CORE_LOG_ERROR(HostLogger::log())

#endif
