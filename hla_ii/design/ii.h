#define SC_INCLUDE_FX

#include "systemc.h"

#include <string>
#include <cstdio>
#include "VirtualBusFederate.h"

#define DATA_SIZE         16

//Signals typedef used for input and output
typedef sc_uint<8> data_t;

SC_MODULE(ii)
{
  sc_in<bool> clock;
  sc_in<bool> reset_n; //Rest enable at negative edge

  sc_in<bool>   in_data_en;//Flag to say that in_data_a and in_data_b is valid
  sc_in<data_t> in_data[DATA_SIZE]; //Data input
  
  sc_out<data_t> out_data[DATA_SIZE];     //Data output
  sc_out<bool>   out_data_valid; //Flag to say that the out_data is valid
  void func();
  

  //HLA
  char federateName[20];
  VirtualBusFederate *federate;
  unsigned src;
  unsigned addr;
  unsigned int size, data[16];

  ii(sc_module_name nm);

}; // end of SC_MODULE

