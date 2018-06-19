
//+--------------------------------------------------------------------------
//| CLASS: ii_test_random
//+--------------------------------------------------------------------------
class ii_test_random : public ii_test_base {
  public :
    //+--------------------------------------------------------------------------
    //| FUNCTION: constructor
    //+--------------------------------------------------------------------------
    ii_test_random(const char* _name,
                          ii_rst_interface* sc_rst_if, 
                          ii_interface*     test_sc_if,
			  ii_interface*     rm_test_sc_if,
                          test_config*       tst_cfg) : ii_test_base(_name, sc_rst_if, test_sc_if, rm_test_sc_if, tst_cfg){ 
  }

    //+--------------------------------------------------------------------------
    //| FUNCTION: configure_phase
    //+--------------------------------------------------------------------------
    //! Set the function of the checker for this test
    //---------------------------------------------------------------------------
    virtual void configure_phase(){
      INFO(name, "Configure Phase");
      ii_env->config_checker(1);
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: run_phase
    //+--------------------------------------------------------------------------
    //! Implement the test run
    //---------------------------------------------------------------------------
    void run_phase(){
      INFO(name, " Starting test...");
      
      INFO(name, " Set seed");
      ii_env->ii_drv->set_seed(seed);
      
      
      for(int i = 0; i < NI; i++) {
	INFO(name, " Generate random data");
	ii_env->ii_drv->randomize();
      }
      
      INFO(name, " Finished test!!!");
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: end_of_run_phase
    //+--------------------------------------------------------------------------
    //! Wait time to end of silmuation
    //+--------------------------------------------------------------------------
    virtual void end_of_run_phase(){
      ii_env->ii_reset->wait_posedge(1);  
    }
};
