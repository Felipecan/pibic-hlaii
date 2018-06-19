cmake_minimum_required (VERSION 2.6)
project (arm_integral_iamgem)

include_directories(../virtualbus)

add_definitions("-DRTI_USES_STD_FSTREAM")
include_directories("$ENV{CERTI_HOME}/include/hla13")
include_directories("$ENV{CERTI_HOME}/include/libhla")
include_directories("$ENV{CERTI_HOME}/lib")
# Set Flags
set(CMAKE_CERTI_FLAGS  "-lRTI-NGd -lFedTimed -lHLAd -lCERTId")
add_definitions(${CMAKE_CERTI_FLAGS})

#Include Exacutable
add_executable(run main.cpp ../virtualbus/FederateAmbassador.cpp ../virtualbus/VirtualBusFederate.cpp)
