#define SC_INCLUDE_FX

#include "systemc.h"

#define NBITS_SIZE 8
#ifndef DATA_SIZE
    #define DATA_SIZE   16
#endif


#define IMAGE_WIDTH 4
#define IMAGE_HEIGHT 4

#define WINDOW_WIDTH 3
#define WINDOW_HEIGHT 3

//Signals typedef used for input and output
typedef sc_uint<NBITS_SIZE> data_t;
typedef sc_uint<3> sel_t;

SC_MODULE(rm_ii)
{
  sc_in<bool> clock;
  sc_in<bool> reset_n; //Rest enable at negative edge

  sc_in<bool>   in_data_en; //Flag to say that in_data_a and in_data_b is valid
  sc_in<data_t> in_data[DATA_SIZE];  //Data input

  sc_out<data_t> out_data[DATA_SIZE];     //Data output
  sc_out<bool>   out_data_valid; //data out is valid
  void func();
  
  rm_ii(sc_module_name nm);

}; // end of SC_MODULE

