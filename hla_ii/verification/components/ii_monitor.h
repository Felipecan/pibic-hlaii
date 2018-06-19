#ifndef _MON_H_
#define _MON_H_

//-----------------------------
// SystemC defines and includes
//-----------------------------
#include "systemc.h"
#include <string>
#include "tlm.h"

#include "VirtualBusFederate.h"

using namespace std;
using namespace tlm;

#include "ii_interface.h"
#include "ii_define.h"

//-----------------------------
// Constants
//-----------------------------


SC_MODULE(ii_monitor)
{

    VirtualBusFederate *federate;
    unsigned src;
    unsigned addr;
    unsigned int size, data[16];
    unsigned readsrc;
    unsigned cntrl_write;

    //-----------------------------
    // Input
    //-----------------------------
    sc_in<bool> clk;
    sc_in<bool> async_reset;

    ii_interface* ii_if;
    ii_sequence_item* ii_sqi;
    ii_sequence_item* ii_sqi_clone;

    int monitor_errors;
    string fname_data_out;
    ofstream file_data_out;    

    //-----------------------------
    // TLM Analysis Port
    //-----------------------------
    tlm_analysis_port<ii_sequence_item> mon_port;

    //-----------------------------
    // Process Declaration
    //-----------------------------
    void capture_signals();
    void copy_if_sqi();

    //-----------------------------
    // Constructor
    //-----------------------------
    SC_HAS_PROCESS(ii_monitor);
    ii_monitor(sc_module_name name, ii_interface* sc_if) : sc_module(name)
    {

        INFO(this->name(), "construtor");
        const char *c = name;
        string n(c);
        monitor_errors = 0;
        fname_data_out = result_path + n + "_output.txt";
        file_data_out.open (fname_data_out.c_str(), ofstream::out);

        ii_sqi = new ii_sequence_item("ii_sqi");
        ii_sqi_clone = new ii_sequence_item("ii_sqi_clone"); //Necessary to avoid overwrite
	
	    ii_if = sc_if;
        //----------------------------
        // Process registration
        //----------------------------

        SC_THREAD(capture_signals);
           sensitive <<  async_reset.neg() << clk.pos();
    }
};

//+--------------------------------------------------------------------------
//| FUNCTION: capture_signals
//+--------------------------------------------------------------------------
//! Check if reset happens, copy the DUT interface to a sequence item and 
//! write it in the TLM port.
//+--------------------------------------------------------------------------
void ii_monitor::capture_signals()
{
    //std::cout << "executing this method" << std::endl;
    stringstream msg;  
    while (true) 
    {
        if(!async_reset.read())
        {
            wait(1, ASYNC_RST_TIMEUNIT);
        }
        ii_monitor::copy_if_sqi();
        ii_sqi_clone = ii_sqi;
        mon_port.write(*ii_sqi_clone);
        wait(1); 
    }
}

//+--------------------------------------------------------------------------
//| FUNCTION: copy_if_sqi
//+--------------------------------------------------------------------------
//! Copy interface signals to sequence item
//+--------------------------------------------------------------------------
void ii_monitor::copy_if_sqi()
{
    stringstream msg;

    // ii_sqi->data_valid  = ii_if->out_data_valid.read();
    // for(int i = 0; i < DATA_SIZE; i++) 
    // {
    //     ii_sqi->data_out[i] = ii_if->out_data[i].read();
    //     msg << "Read: out_data = " << ii_sqi->data_out[i];
    //     INFO(name(), msg.str().c_str(), HIGH);
    //     msg.str(""); //clean
    // }

    if(federate->readData(src, addr, size, data))
    {
        if(src == readsrc)
        {
            ii_sqi->data_valid  = 1;
            msg << "data from src=readsrc " << src;
            INFO(name(), msg.str().c_str(), HIGH);
            msg.str(""); //clean

            for(int i = 0; i < DATA_SIZE; i++) 
            {                
                ii_sqi->data_out[i] = data[i]; // ?
                msg << "Read: out_data " << i << " = " << ii_sqi->data_out[i];
                INFO(name(), msg.str().c_str(), HIGH);
                msg.str(""); //clean

                file_data_out << data[i] << " ";
            }

            file_data_out << std::endl;
        }
        else
        {
            msg << "err data from src " << src;
            INFO(name(), msg.str().c_str(), HIGH);
            msg.str(""); //clean
        }
       // federate->advanceTime(1.0);
        
    }
    


  //save output data
  //file_data_out << ii_if->out_data_rdy << " " << ii_if->out_data << " " << ii_if->out_carryout << endl;
}

#endif
