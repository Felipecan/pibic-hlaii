#ifndef _ENV_H_
#define _ENV_H_

#include <systemc.h>
#include <ii_pkg.h>
#include <ii_checker.h>
#include <ii_rst_interface.h>
#include <ii_rst.h>

SC_MODULE(ii_environment)
{
  public:
 
    //+--------------------------------------------------------------------------
    //| Components Members
    //+--------------------------------------------------------------------------
    ii_driver*   ii_drv;
    ii_monitor*  ii_dut_mon;
    ii_monitor*  ii_rm_mon;
    ii_checker*  ii_chk;

    ii_rst*      ii_reset;

    //+--------------------------------------------------------------------------
    //| FUNCTION: config_checker
    //+--------------------------------------------------------------------------
    //! Select the type of check for:
    //!   1 - Select compare_in_out
    //+--------------------------------------------------------------------------
    void config_checker(int config_checker){
       ii_chk->select_checker = config_checker;
    }
    
    //+--------------------------------------------------------------------------
    //| FUNCTION: constructor
    //+--------------------------------------------------------------------------
    SC_HAS_PROCESS(ii_environment);
       ii_environment(sc_module_name name, 
                          ii_rst_interface* sc_rst_if, 
                          ii_interface*     ii_vif, 
                          ii_interface*     ii_rm_vif): sc_module(name)
    {
      INFO(this->name(), "constructor");

      ii_reset = new ii_rst("ii_reset", sc_rst_if);
      ii_reset->clock(sc_rst_if->clk);
      ii_reset->clock2(sc_rst_if->clk2);
       
      ii_drv = new ii_driver("ii_drv",ii_vif);
      ii_drv->clk(sc_rst_if->clk);
      ii_drv->async_reset(sc_rst_if->reset_n);

      ii_dut_mon = new ii_monitor("ii_dut_mon",ii_vif);  
      ii_dut_mon->clk(sc_rst_if->clk);
      ii_dut_mon->async_reset(sc_rst_if->reset_n);

      ii_rm_mon = new ii_monitor("ii_rm_mon",ii_rm_vif);  
      ii_rm_mon->clk(sc_rst_if->clk);
      ii_rm_mon->async_reset(sc_rst_if->reset_n);

      ii_chk = new ii_checker("ii_chk");
      ii_chk->clk(sc_rst_if->clk);
      ii_chk->clk2(sc_rst_if->clk2);
      ii_chk->async_reset(sc_rst_if->reset_n);

      ii_drv->drv_port.bind(ii_chk->chk_drv_port);      
      ii_dut_mon->mon_port.bind(ii_chk->chk_port);
      ii_rm_mon->mon_port.bind(ii_chk->chk_rm_port);
    }

};

#endif /* VIP_SC_ENV_H_ */
