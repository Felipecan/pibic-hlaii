// interface definitions for VIP

#ifndef _IF_H_
#define _IF_H_

#include <systemc>

class ii_interface
{

 public:
 
  sc_signal<bool>       in_data_en;    
  sc_signal<dut_data_t> in_data[DATA_SIZE];

  sc_signal<dut_data_t> out_data[DATA_SIZE];
  sc_signal<bool>       out_data_valid;


   ii_interface(){}
   ~ii_interface(){}
};


#endif /* _IF_H_ */
