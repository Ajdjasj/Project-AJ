#######################################################
##############   SOURCE SOURCE SOURCE    ##############
#######################################################
unset (srcs)
list (APPEND srcs "sd_wrapper.cpp")
list (APPEND srcs "sd_pci.cpp")
list (APPEND srcs "host.cpp")
list (APPEND srcs "card.cpp")


#######################################################
##############      TARGET LIBRARY       ##############
#######################################################
add_diag_library (sddiagdriver EXCLUDE_FROM_ALL ${srcs})


#######################################################
###########      LIBRARY DEPENDENCIES      ############
#######################################################
target_link_libraries (sddiagdriver boost_log-mt)
target_link_libraries (sddiagdriver boost_system-mt)
target_link_libraries (sddiagdriver core_util)
target_link_libraries (sddiagdriver env)
target_link_libraries (sddiagdriver env_device_interface)


#######################################################
##############    INCLUDE DIRECTORIES    ##############
#######################################################
target_include_directories (sddiagdriver SYSTEM PRIVATE "${GPU_DIAGS_SOURCE_DIR}/external/non-amd/lib/common/boost")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/sbridge/common")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/sbridge/sddiagdriver")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/env")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/env_device_interface")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/sbridge/sddiagdriver")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/core_util")
target_include_directories (sddiagdriver PRIVATE "${GPU_DIAGS_SOURCE_DIR}/lib/common/device_info")

#######################################################
##############      COMPILE OPTIONS      ##############
#######################################################
if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
        target_compile_options (sddiagdriver PRIVATE -Wall -Werror -Wextra)
endif ()


#######################################################
###########       API DOCUMENTATION        ############
#######################################################
add_doxygen (sddiagdriver)
