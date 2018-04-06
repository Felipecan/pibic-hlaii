#ifndef _RST_H_
#define _RST_H_

//-----------------------------
// SystemC defines and includes
//-----------------------------
#include "systemc.h"
#include "tlm.h"
#include <cstdlib>
#include <ctime>

using namespace tlm;
using namespace std;

SC_MODULE(ii_rst)
{
    //-----------------------------
    // Input
    //-----------------------------
    sc_in<bool>  clock;
    sc_in<bool>  clock2;

    //+--------------------------------------------------------------------------
    //| Local Variables
    //+--------------------------------------------------------------------------

    // Interface
    ii_rst_interface* rst_if;
    
    bool areset;
    bool fast_async_reset;

    //-----------------------------
    // Process Declaration
    //-----------------------------
    void async_reset(bool fast_reset);
    void sync_reset(int time_hi, int time_low);
    void trap_posedge();
    void trap_negedge();
    void wait_posedge(int n_time);
    void wait_negedge(int n_time);
    void wait_clock2();

    // Generate a random number between nLow and nHigh
    unsigned int get_random_number(int number_low, int number_high)
    {
       return (rand() % (number_high - number_low + 1)) + number_low;
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: constructor
    //+--------------------------------------------------------------------------
    SC_HAS_PROCESS(ii_rst);
     ii_rst(sc_module_name name, ii_rst_interface* sc_if) : sc_module(name)
    {
        srand(time(0));  // set initial seed value to system clock
        rst_if = sc_if;

        areset = 0;
        fast_async_reset = 0;

        //----------------------------
        // Process registration
        //----------------------------

        SC_THREAD(trap_posedge)
          sensitive << clock.pos();
	  dont_initialize();

        SC_THREAD(trap_negedge)
          sensitive << clock.neg();
	  dont_initialize();
	  
        SC_THREAD(wait_clock2)
          sensitive << clock2.pos();
	  dont_initialize();
	  
    }
};

//+--------------------------------------------------------------------------
//| FUNCTION: trap_posedge
//+--------------------------------------------------------------------------
//! Wait 1 posedge clock
//+--------------------------------------------------------------------------
void ii_rst::trap_posedge()
{
   wait(1);
}

//+--------------------------------------------------------------------------
//| FUNCTION: trap_negedge
//+--------------------------------------------------------------------------
//! Wait 1 negedge clock
//+--------------------------------------------------------------------------
void ii_rst::trap_negedge()
{  
  wait(1);
}

//+--------------------------------------------------------------------------
//| FUNCTION: wait_posedge
//+--------------------------------------------------------------------------
//! Received a int value of how many posedge clock must wait
//+--------------------------------------------------------------------------
void ii_rst::wait_posedge(int n_time)
{
   for(int i = 0; i < n_time; i++) {
      trap_posedge();
   }
}

//+--------------------------------------------------------------------------
//| FUNCTION: wait_negedge
//+--------------------------------------------------------------------------
//! Received a int value of how many negedge clock must wait
//+--------------------------------------------------------------------------
void ii_rst::wait_negedge(int n_time)
{
   for(int i = 0; i < n_time; i++) {
      trap_negedge();
   }
}

//+--------------------------------------------------------------------------
//| FUNCTION: sync_reset
//+--------------------------------------------------------------------------
//! Pass 2 clocks in with arest_n in high level. Then, put arst_n to low level 
//! for more 2 clocks and spendy 1 clock in high level.
//+--------------------------------------------------------------------------
void ii_rst::sync_reset(int time_hi=5, int time_low=2)
{
    wait(1);
    rst_if->reset_n.write(1);
    wait(time_hi);
    rst_if->reset_n.write(0);
    wait(time_low);
    rst_if->reset_n.write(1);
    wait(1);
}

//+--------------------------------------------------------------------------
//| FUNCTION: async_reset
//+--------------------------------------------------------------------------
//! Pass 2 clocks in with arest_n in high level. Then, put arst_n to low level 
//! for more 2 clocks and spendy 1 clock in high level.
//+--------------------------------------------------------------------------
void ii_rst::async_reset(bool fast_reset=0)
{
   INFO(this->name(), "Asserting async reset signal....");
   areset = 1;
   fast_async_reset = fast_reset;
}

//+--------------------------------------------------------------------------
//| FUNCTION: wait_clock2
//+--------------------------------------------------------------------------
//! Generate a random number to determine the time which reset will be low
//+--------------------------------------------------------------------------
void ii_rst::wait_clock2()
{

  unsigned int rst_dly, fast_dly;
  rst_dly = get_random_number(15,45);
  fast_dly =  get_random_number(5,45);

  while(true) {
    //if(areset.read() == 1) {
    if(areset){
      INFO(this->name(), "Reset signal detected....");
      //is_rst = 1;
      rst_if->reset_n.write(1);
      wait(rst_dly*10);
      rst_if->reset_n.write(0);
      if(fast_async_reset){ wait(fast_dly); }
      else{ wait(rst_dly*10); }
      rst_if->reset_n.write(1);
      areset = 0;

    }
    wait(1);
  }
}

#endif
