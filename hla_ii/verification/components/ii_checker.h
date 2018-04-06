#ifndef _CHK_H_
#define _CHK_H_

#include <systemc.h>
#include <ii_interface.h>

//+--------------------------------------------------------------------------
//| SC_MODULE: ii_checker
//+--------------------------------------------------------------------------
SC_MODULE(ii_checker)
{
 public:
   sc_in<bool> clk;
   sc_in<bool> clk2;
   sc_in<bool> async_reset;
   
  //-----------------------------
  // TLM Analysis Port
  //-----------------------------
  tlm_analysis_port<ii_sequence_item> chk_port;
  tlm_analysis_port<ii_sequence_item> chk_rm_port;
  tlm_analysis_port<ii_sequence_item> chk_drv_port;

  bool previous_reset;
  bool current_reset;
  bool reset_detected;
  
  int select_checker;

  ii_sequence_item ii_sqi;
  ii_sequence_item ii_drv_sqi;

  ii_subscriber*  ii_dut_sub;
  ii_subscriber*  ii_rm_sub;
  ii_subscriber*  ii_drv_sub;
  
  //-----------------------------
  // Process Declaration
  //-----------------------------
  void check();
  void compare_dut_refmod(ii_sequence_item item);
  void compare_reset(ii_sequence_item item);

  //+--------------------------------------------------------------------------
  //| FUNCTION: is_close_out
  //+--------------------------------------------------------------------------
  bool is_close_out(double x1, double x2,  double tol = exp2(-7)/*1e-10*/){
    bool rv;
    rv = abs(x1-x2)<tol;
    return rv;
  }
  
  //+--------------------------------------------------------------------------
  //| FUNCTION: is_close_in
  //+--------------------------------------------------------------------------
  bool is_close_in(double x1, double x2,  double tol = exp2(-7)/*1e-10*/){
    bool rv;
    rv = abs(x1-x2)<tol;
    return rv;
  }
  
  SC_HAS_PROCESS(ii_checker);
     ii_checker(sc_module_name name): sc_module(name)
  {
    INFO(this->name(), "constructor");
 
    ii_dut_sub = new ii_subscriber("ii_dut_sub");
    chk_port.bind(*ii_dut_sub);
    
    ii_rm_sub = new ii_subscriber("ii_rm_sub");
    chk_rm_port.bind(*ii_rm_sub);
    
    ii_drv_sub = new ii_subscriber("ii_drv_sub");
    chk_drv_port.bind(*ii_drv_sub);
    
    select_checker = 0;
    previous_reset = 1;
    reset_detected = 0;  

    SC_THREAD(check)
       sensitive << clk.pos();
       async_reset_signal_is(async_reset, false); 
  }
};

//+--------------------------------------------------------------------------
//| FUNCTION: check 
//+--------------------------------------------------------------------------
//! Wait to receive a valid clock, and call the function which compare the 
//! input data with the output data. 
//+--------------------------------------------------------------------------
void ii_checker::check(){
  stringstream msg;

  while(true) {
      ii_sqi = ii_dut_sub->fifo.get();

      if(select_checker == 4){
        ii_checker::compare_reset(ii_sqi);
      }

      if(ii_sqi.data_valid){
    	switch(select_checker){
	  case 1: ii_checker::compare_dut_refmod(ii_sqi); break;
          default: break;
    	}
      }
    wait(1); 
  }
}

//+--------------------------------------------------------------------------
//| FUNCTION: compare_reset
//+--------------------------------------------------------------------------
//! Check reset values on dut outputs
//+--------------------------------------------------------------------------
void ii_checker::compare_reset(ii_sequence_item item) {
  stringstream msg;

  INFO(name(), "Check reset is active!!!");
  if(!async_reset.read()){

    if(item.data_valid != 0){
      ERROR(name(), "Mismatch o_rdy is not going to zero after reset");
    }

    if(item.data_out != 0){
      ERROR(name(), "Mismatch data_out is not going to zero after reset");
    }

  }
}


//+--------------------------------------------------------------------------
//| FUNCTION: compare_dut_refmod 
//+--------------------------------------------------------------------------
//! Compare output values between reference model and dut
//+--------------------------------------------------------------------------
void ii_checker::compare_dut_refmod(ii_sequence_item item){
  stringstream msg;
  bool find_valid = 0;
  ii_sequence_item ii_rm_sqi;

    while(!find_valid){
      ii_rm_sqi = ii_rm_sub->fifo.get();
      if(ii_rm_sqi.data_valid){
        find_valid = 1;
      }
    }
    find_valid = 0;

    for (int i = 0; i < DATA_SIZE; ++i) {
      if(ii_rm_sqi.data_out[i] != item.data_out[i]) {
        msg << "MISMATCH between DUT and Reference Model: data_out[" << i << "](DUT) = " << ii_sqi.data_out[i] << " - data_out[" << i << "](RM) = " << ii_rm_sqi.data_out[i];
        ERROR(name(), msg.str().c_str());
        msg.str(""); //clean
      }
      else {
        msg << "MATCH between DUT and Reference Model:data_out[" << i << "](DUT) = " << ii_sqi.data_out[i] << " - data_out[" << i << "](RM) = " << ii_rm_sqi.data_out[i];;
        INFO(name(), msg.str().c_str(), HIGH);
        msg.str(""); //clean
      }
    }

}

#endif /* VIP_SC_CHK_H_ */
