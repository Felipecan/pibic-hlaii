// interface definitions for VIP

#ifndef _RST_IF_H_
#define _RST_IF_H_

#include <systemc>

#define ASYNC_RST_TIMEUNIT   SC_PS
#define RST_CLK_TIMEUNIT     SC_NS

class ii_rst_interface
{

   public:
      sc_clock clk;
      sc_clock clk2;
  
      sc_signal<bool> reset_n;

      ii_rst_interface() : clk("clk", 1, RST_CLK_TIMEUNIT, 0.5),clk2("clk2", 2, ASYNC_RST_TIMEUNIT, 0.5) {}
      ~ii_rst_interface() {}
};


#endif
