#include <systemc.h>
#include <assert.h>
#include <ii_driver.h>
#include <ii_sequence_item.h>


//+--------------------------------------------------------------------------
//| FUNCTION: randomize
//+--------------------------------------------------------------------------
//! Generate a randomize data 
//+--------------------------------------------------------------------------
void ii_driver::randomize()
{
  stringstream msg;
  ii_sqi->data_enable = 1;
  for (int i=0; i<DATA_SIZE; i++) {
    ii_sqi->data_in[i] = rand() % 256;
    msg << "Generate: data_a = " << ii_sqi->data_in[i];
    INFO(name(), msg.str().c_str(), HIGH);
    msg.str(""); //clean
  }

  update_interface = 1;
  wait(1);
}
 
//+--------------------------------------------------------------------------
//| FUNCTION: set_data
//+--------------------------------------------------------------------------
//! Receive a double data and passe to in_data_a and in_data_b 
//+--------------------------------------------------------------------------
void ii_driver::set_data(double data[DATA_SIZE])
{
  stringstream msg;
  ii_sqi->data_enable = 1;

  for (int i = 0; i < DATA_SIZE; ++i) {
    ii_sqi->data_in[i] = data[i];
  }

  update_interface = 1;
}

