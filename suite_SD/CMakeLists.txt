#######################################################
##############   SOURCE SOURCE SOURCE    ##############
#######################################################
unset (srcs)
list (APPEND srcs "sdcard_test.cpp")
list (APPEND srcs "sd_detect.cpp")
#list (APPEND srcs "sd_sleep_state.cpp") // not tested
# list (APPEND srcs "sd_diffrent_speed_mode.cpp")
list (APPEND srcs "sd_init.cpp")
# list (APPEND srcs "sd_host_registers.cpp")
list (APPEND srcs "sd_lock_unlock.cpp")
list (APPEND srcs "sd_force_erase.cpp")
# list (APPEND srcs "sd_sleep_state.cpp")
# list (APPEND srcs "sdhc_sdxc_pio_read_write.cpp")
# list (APPEND srcs "sd_data_erase.cpp")
# list (APPEND srcs "sd_sdma_read_write.cpp")
# list (APPEND srcs "sd_adma_read_write.cpp")
# list (APPEND srcs "sd_software_reset.cpp")
# list (APPEND srcs "sd_card_registers.cpp")
# list (APPEND srcs "sd_write_protect.cpp")
# list (APPEND srcs "sd_switch_function.cpp")
# list (APPEND srcs "sd_force_event.cpp")
# list (APPEND srcs "sd_card_sd_status.cpp")
# list (APPEND srcs "sd_voltage.cpp")
# list (APPEND srcs "sd_card_led_control.cpp")
list (APPEND srcs "sd_power_pin.cpp")
# list (APPEND srcs "sd_tuning_procedure_test.cpp")
# list (APPEND srcs "sd_preset_value.cpp")
# list (APPEND srcs "sd_device_id_change.cpp")
# list (APPEND srcs "sd_card_insert_simulation.cpp")
# list (APPEND srcs "sd_card_backdoor_overwrite.cpp")
# list (APPEND srcs "sd_card_fifo_test.cpp")
# list (APPEND srcs "sd_feed_back_clock.cpp")
# list (APPEND srcs "sd_abort_transaction.cpp")
# list (APPEND srcs "sd_card_misc_command.cpp")
# list (APPEND srcs "sd_pci_deviceclass_rw.cpp")
# list (APPEND srcs "sd_msi_intx.cpp")
# list (APPEND srcs "sd_alink_clockgating.cpp")
# list (APPEND srcs "sd_diffrent_tuning_mode_test.cpp")
# list (APPEND srcs "sd_card_auto_cmd23.cpp")
# list (APPEND srcs "sd_io_test.cpp")
# list (APPEND srcs "sd_error_recovery.cpp")
# list (APPEND srcs "sd_clock_freq_calculation.cpp")

list (APPEND srcs "sd_compliance_1_6_3.cpp")
list (APPEND srcs "sd_compliance_1_6_4.cpp")
list (APPEND srcs "sd_compliance_3_2_7.cpp")
list (APPEND srcs "sd_compliance_2_1_6.cpp")
list (APPEND srcs "sd_compliance_2_1_9.cpp") 
list (APPEND srcs "sd_compliance_2_1_18.cpp")
list (APPEND srcs "sd_compliance_2_1_1.cpp")
list (APPEND srcs "sd_compliance_2_1_2.cpp")
list (APPEND srcs "sd_compliance_2_1_36.cpp")
list (APPEND srcs "sd_compliance_2_2_9_14.cpp")
list (APPEND srcs "sd_compliance_4_1_5.cpp")
list (APPEND srcs "sd_compliance_2_2_4.cpp")
list (APPEND srcs "sd_compliance_2_1_13.cpp")
list (APPEND srcs "sd_compliance_1_1_28.cpp")
list (APPEND srcs "sd_compliance_1_1_29.cpp")
list (APPEND srcs "sd_compliance_1_6_6.cpp")
list (APPEND srcs "sd_compliance_2_2_10.cpp")
list (APPEND srcs "sd_compliance_2_1_17.cpp")
list (APPEND srcs "sd_compliance_2_1_25.cpp")
list (APPEND srcs "sd_compliance_2_1_23.cpp") 
list (APPEND srcs "sd_compliance_3_4_16.cpp")
list (APPEND srcs "sd_compliance_1_2_4.cpp")
list (APPEND srcs "sd_compliance_3_5_4.cpp")
#######################################################
##############      TARGET LIBRARY       ##############
#######################################################
add_suite (sddiag_suite ${srcs})


#######################################################
###########      LIBRARY DEPENDENCIES      ############
#######################################################
target_link_libraries (sddiag_suite boost_log-mt)
target_link_libraries (sddiag_suite boost_system-mt)
target_link_libraries (sddiag_suite core_util)
target_link_libraries (sddiag_suite env)
target_link_libraries (sddiag_suite env_device_interface)
target_link_libraries (sddiag_suite sddiagdriver)


#######################################################
##############    INCLUDE DIRECTORIES    ##############
#######################################################
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/app/common/tserverlite")
target_include_directories (sddiag_suite SYSTEM PRIVATE "${GPU_DIAGS_SOURCE_DIR}/external/non-amd/lib/common/boost")
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/env")
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/target")
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/device_info")
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/sbridge/sddiagdriver")
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/core_util")
target_include_directories (sddiag_suite PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/env_device_interface")

#######################################################
##############      COMPILE OPTIONS      ##############
#######################################################
if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
        target_compile_options (sddiag_suite PRIVATE -Wall -Werror -Wextra)
endif ()



