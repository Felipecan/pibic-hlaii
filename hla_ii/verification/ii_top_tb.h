SC_MODULE(ii_top_tb) {
    //+--------------------------------------------------------------------------
    //| Members
    //+--------------------------------------------------------------------------
    // Interface
    ii_rst_interface *rst_if;
    ii_interface     *ii_if;
    ii_interface     *rm_out_if;

    // DUT Module
    //ii    DUT;
    //rm_ii RM;

    // Test Module
    ii_test *test;

    #ifdef _TRACE_
      sc_trace_file *tf;
      string        trace_fname;
    #endif
    //+--------------------------------------------------------------------------
    //| FUNCTION: constructor
    //+--------------------------------------------------------------------------

     ii_top_tb(sc_module_name name, string _testname, string _trace_fname, test_config* tst_cfg)
   : //DUT("DUT"),
     //RM("RM"),
     sc_module(name)
   {
    // Interface
    rst_if     = new ii_rst_interface();   //reset
    ii_if     = new ii_interface();
    rm_out_if  = new ii_interface();

       //-----------------------------
       // DUT Module
       // -----------------------------
       //Input signals
    //    DUT.clock   (rst_if->clk);
    //    DUT.reset_n (rst_if->reset_n);

    //    DUT.in_data_en  (ii_if->in_data_en);
    //    for(int i = 0; i < DATA_SIZE; i++)
    //        DUT.in_data[i](ii_if->in_data[i]);


    //    //Output signals
    //    for(int i = 0; i < DATA_SIZE; i++)
    //        DUT.out_data[i](ii_if->out_data[i]);
    //    DUT.out_data_valid(ii_if->out_data_valid);

    //-----------------------------
    // Reference Module
    //-----------------------------
    //Input signals
    // RM.clock   (rst_if->clk);          
    // RM.reset_n (rst_if->reset_n);

    // RM.in_data_en  (ii_if->in_data_en);
    //    for(int i = 0; i < DATA_SIZE; i++)
    //        RM.in_data[i](ii_if->in_data[i]);

    //    //Output signals
    //    for(int i = 0; i < DATA_SIZE; i++)
    //        RM.out_data[i](rm_out_if->out_data[i]);
    // RM.out_data_valid(rm_out_if->out_data_valid);

    //-----------------------------
    // Test Module
    //-----------------------------
    test = new ii_test(_testname.c_str(),
                            rst_if,
                            ii_if,
                            rm_out_if,
                            tst_cfg);
    test->clock(rst_if->clk);

    #ifdef _TRACE_
      tf = 0;
      trace_fname = _trace_fname;
    #endif

    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: before_end_of_elaboration
    //+--------------------------------------------------------------------------
    //! Used to create the trace file
    //+--------------------------------------------------------------------------
    void before_end_of_elaboration(){
      #ifdef _TRACE_
        tf = sc_create_vcd_trace_file(trace_fname.c_str());
	tf->set_time_unit(0.5, SC_PS);
      #endif
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: end_of_elaboration
    //+--------------------------------------------------------------------------
    //! Used to connect the interface signals to trace file
    //+--------------------------------------------------------------------------
    void end_of_elaboration(){
      #ifdef _TRACE_
        sc_trace(tf, rst_if->clk, "clk");
        sc_trace(tf, rst_if->reset_n, "reset_n");

        sc_trace(tf, ii_if->in_data_en, "in_data_en");
        sc_trace(tf, ii_if->in_data_a,  "in_data_a");
        sc_trace(tf, ii_if->in_data_b,  "in_data_b");
        sc_trace(tf, ii_if->in_sel ,    "in_sel");
        
	sc_trace(tf, ii_if->out_data    , "out_data");
        sc_trace(tf, ii_if->out_data_rdy, "out_data_rdy");
        
	sc_trace(tf, rm_out_if->out_data    , "rm_out_data");
        sc_trace(tf, rm_out_if->out_data_rdy, "rm_out_data_rdy");
        
       #endif
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: end_of_simiition
    //+--------------------------------------------------------------------------
    //! Used to close the trace file
    //+--------------------------------------------------------------------------
    void end_of_simiition() {
      #ifdef _TRACE_
        sc_close_vcd_trace_file(tf);
      #endif
    }
}; 

