//
// Created by daniel on 21/05/17.
//

#ifndef ARM_INTEGRAL_IAMGEM_II_TEST_FIXED_H
#define ARM_INTEGRAL_IAMGEM_II_TEST_FIXED_H
//+--------------------------------------------------------------------------
//| CLASS: ii_test_fixed
//+--------------------------------------------------------------------------
class ii_test_fixed : public ii_test_base {
public :
    //+--------------------------------------------------------------------------
    //| FUNCTION: constructor
    //+--------------------------------------------------------------------------
    ii_test_fixed(const char* _name,
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

        // double data[DATA_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        double data[DATA_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};

        INFO(name, " Send the fixed array");
        ii_env->ii_drv->set_data(data);

        INFO(name, " Finished test!!!");
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: end_of_run_phase
    //+--------------------------------------------------------------------------
    //! Wait time to end of silmuation
    //+--------------------------------------------------------------------------
    virtual void end_of_run_phase(){
        ii_env->ii_reset->wait_posedge(4);
    }
};
#endif //ARM_INTEGRAL_IAMGEM_II_TEST_FIXED_H
