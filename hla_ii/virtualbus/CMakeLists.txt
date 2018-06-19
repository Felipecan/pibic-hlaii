#Add directores for CERTI-3
add_definitions("-DRTI_USES_STD_FSTREAM")
include_directories("$ENV{CERTI_HOME}/include/hla13")
include_directories("$ENV{CERTI_HOME}/include/libhla")
include_directories("$ENV{CERTI_HOME}/lib")
# Set Flags
set(CMAKE_CERTI_FLAGS  "-lRTI-NGd -lFedTimed -lHLAd -lCERTId")
add_definitions(${CMAKE_CERTI_FLAGS})

#DEfinition for virtualbus
add_library(VIRTUALBUS FederateAmbassador.cpp VirtualBusFederate.cpp)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (VIRTUALBUS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
