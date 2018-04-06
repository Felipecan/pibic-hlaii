
#ifndef _DRV_H_
#define _DRV_H_

//-----------------------------
// SystemC defines and includes
//-----------------------------
#include "systemc.h"
#include "tlm.h"
using namespace std;
using namespace tlm;

#include <ii_interface.h>
#include <ii_rst_interface.h>
#include <ii_sequence_item.h>

#include "VirtualBusFederate.h"

SC_MODULE(ii_driver)
{

    VirtualBusFederate *federate;
    unsigned src=0;
    unsigned addr;
    unsigned int size, data[16];

    //-----------------------------
    // Input
    //-----------------------------
    sc_in<bool> clk;
    sc_in<bool> async_reset;

    ii_interface* drv_if;
  
    //Sequence item
    ii_sequence_item* ii_sqi;
 
    int driver_errors;
    
    string fname_h;
    ofstream file_h;
    
    bool update_interface;

    //-----------------------------
    // TLM Analysis Port
    //-----------------------------
    tlm_analysis_port<ii_sequence_item> drv_port;

    //-----------------------------
    // Process Declaration
    //-----------------------------
    void drive();
    void set_seed(int new_seed);
    void set_data(double  data[DATA_SIZE]);
    void randomize();

    SC_HAS_PROCESS(ii_driver);
     ii_driver(sc_module_name name, ii_interface* in_if)
   : sc_module(name),clk("clk"),async_reset("async_reset")
    {
        INFO(this->name(), "construtor");

        driver_errors = 0;
	    update_interface = 0;
        fname_h = result_path + "ii_input.txt";
        file_h.open (fname_h.c_str(), ofstream::out);

        ii_sqi = new ii_sequence_item("ii_sqi");

        drv_if = in_if;

        srand(0);
        //----------------------------
        // Process registration
        //----------------------------
        SC_CTHREAD(drive,clk.pos());
          async_reset_signal_is(async_reset, false); 
	  
    }
     
};

//+--------------------------------------------------------------------------
//| FUNCTION: set_seed
//+--------------------------------------------------------------------------
//! This function randomize with
//! the seed that is put by prompt
//+--------------------------------------------------------------------------
void ii_driver::set_seed(int new_seed)
{
   srand(new_seed);
   update_interface = 1;
}

//+--------------------------------------------------------------------------
//| FUNCTION: drive
//+--------------------------------------------------------------------------
//! Get the data from the sequece item and drive in the DUT. Put the sequence
//! item used to a TLM port to be used in the bypass mode checker.
//+--------------------------------------------------------------------------
void ii_driver::drive()
{
    stringstream msg;
    bool current_reset;

    while(true)
    {            
        INFO(name(), "WHILE", HIGH);  
            std::cout << "WHILE" << std::endl;
                
        current_reset = async_reset.read(); 
        if(current_reset == 0) 
        {   
            src = 0;
            //drv_if->in_data_en.write(0);
            for(int i = 0; i < DATA_SIZE; i++) 
            {
                //drv_if->in_data[i].write(0);
                data[i] = 0;
            federate->writeData(src, addr, size, data);
            }
            std::cout << "Writing data 1 " << src << std::endl;
            federate->advanceTime(1.0);   
            INFO(name(), "Saindo do reset", HIGH);          
        }
        else if(update_interface == 1)
        {
            src = 0;
            //Drive into drive's interface
            //drv_if->in_data_en.write(ii_sqi->data_enable);

            for(int i = 0; i < DATA_SIZE; i++) 
            {
                //drv_if->in_data[i].write(ii_sqi->data_in[i]);
                data[i] = ii_sqi->data_in[i];
                federate->writeData(src, addr, size, data);                 
            }
            
            //save drive at each clock
            //file_h << drv_if->in_data_en << " " <<  drv_if->in_data_a << " " << drv_if->in_data_b << " " << drv_if->in_sel << endl;

            //send the data to checker
            drv_port.write(*ii_sqi);
            update_interface = 0;
            std::cout << "Writing data 2 " << src << std::endl;
            federate->advanceTime(1.0);               
        }           
        wait(1);
    }
}


#endif
