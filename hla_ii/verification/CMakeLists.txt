cmake_minimum_required (VERSION 2.6)
project (arm_integral_iamgem)


#Add directores from Altera
add_subdirectory("components")
add_subdirectory("tests")

# CERTI Configuration
include_directories("$ENV{CERTI_HOME}/include/hla13")
include_directories("$ENV{CERTI_HOME}/include/libhla")

set(CERTI_FLAGS  "-lRTI-NGd -lFedTimed -lHLAd -lCERTId")
add_definitions(${CERTI_FLAGS})

# SystemC Configuration
target_link_libraries(run $ENV{SYSTEMC_HOME}/lib-linux64/libsystemc.a)

set(SYSTEMC_FLAGS "-DSC_INCLUDE_DYNAMIC_PROCESSES")
add_definitions(${SYSTEMC_FLAGS})

include_directories("$ENV{CERTI_HOME}/include/libhla")

#Include Exacutable
add_executable(run main.cpp tests/ii_test_fixed.h)