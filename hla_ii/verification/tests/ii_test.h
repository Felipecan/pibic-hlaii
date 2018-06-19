#include <ii_environment.h>

//+--------------------------------------------------------------------------
//| SC_MODULE: ii_test
//+--------------------------------------------------------------------------
SC_MODULE (ii_test) {
    //+--------------------------------------------------------------------------
    //| Input
    //+--------------------------------------------------------------------------
    sc_in<bool> clock;

    //+--------------------------------------------------------------------------
    //| Running test
    //+--------------------------------------------------------------------------
    ii_test_base* t;

    //+--------------------------------------------------------------------------
    //| Function Members
    //+--------------------------------------------------------------------------
    void select_phase();
  
    //+--------------------------------------------------------------------------
    //| FUNCTION: constructor
    //+--------------------------------------------------------------------------
    SC_HAS_PROCESS(ii_test);
     ii_test(sc_module_name name, 
                 ii_rst_interface* sc_rst_if, 
                 ii_interface*     test_sc_if,
                 ii_interface*     rm_test_sc_if,
                 test_config*       tst_cfg)
   : sc_module(sc_module_name("ii_test"))
   {
      if (string(name) == "ii_test_sanity") {
        t = new ii_test_sanity(name, sc_rst_if, test_sc_if, rm_test_sc_if, tst_cfg);
      }
      else if (string(name) == "ii_test_random") {
        t = new ii_test_random(name, sc_rst_if, test_sc_if, rm_test_sc_if, tst_cfg);
      }
      else if (string(name) == "ii_test_fixed") {
          t = new ii_test_fixed(name, sc_rst_if, test_sc_if, rm_test_sc_if, tst_cfg);
      }
      else  {
        SC_REPORT_FATAL(name, "Test not register");
      }

      SC_CTHREAD(select_phase,clock.pos());
    }
}; 

//+--------------------------------------------------------------------------
//| FUNCTION: select_phase
//+--------------------------------------------------------------------------
//! Function responsible to call the phases of the test
//+-------------------------------------------------------------------------
void ii_test::select_phase() {
   //Call Reset Phase
   t->reset_phase();

   //Call Configure Phase
   t->configure_phase();

   //Call Run Phase
   t->run_phase();

   //Call End of Run Phase
   t->end_of_run_phase();

   //Call Report Phase
   t->report_phase();

   //Call Finish Phase
   t->finish_phase();
}
