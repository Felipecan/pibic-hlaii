//-----------------------------
// SystemC defines and includes
//-----------------------------
#define SC_INCLUDE_FX
#include <systemc.h>
#include "tlm.h"

using namespace tlm;
using namespace std;


//-----------------------------
// C++ includes
//-----------------------------
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
// #include <chrono>
// using namespace std::chrono;
using namespace std;

#include <report_server.h>

//-----------------------------
// Log, Trace and Module ID
//-----------------------------
const char* main_ID  = "ii_MAIN";
const char* log_name = "ii_tb.log";
const char* vcd_name = "ii_trace";
string result_path;
int number_clock;


//-----------------------------
// DUT Instantiation
//-----------------------------

//#include <ii.h>
//#include <rm_ii.h>

//-----------------------------
// Sub-Modules Include
//-----------------------------

#include <ii_define.h>
#include <ii_pkg.h>
#include <ii_subscriber.h>
#include <ii_test_list.h>

// Reset
#include <ii_rst_interface.h>
#include <ii_rst.h>

#include <ii_top_tb.h>

using namespace report_server;
verbosity_type report_server::report::verbosity;

int sc_main(int argc, char *argv[])
{
    // high_resolution_clock::time_point t1 = high_resolution_clock::now();
    //-----------------------------
    // Initial Time
    //-----------------------------
    int startTime;
    startTime = time(NULL);
    
    //-------------------------
    // Connection Signals
    //--------------------------
    //sc_signal<bool> reset("arst_n");
    //is_rst=0;


    // +--------------------------------------------------------------------------
    // | RECEIVEING INPUT ARGUMENTS
    // +--------------------------------------------------------------------------
    int opt;
    string input_argv = "Running command line: ";

    //Setup to dumping the log file
    char* pwd = getenv("PWD");
    string log_fname;
    string result_path_tmp(pwd);
    result_path = result_path_tmp + "/results/";

    string testname  = "ii_test_sanity";
    string seed_input = "1";
    string verbosity_input = "MEDIUM";
    string NI_str = "50";

    verbosity_type verbosity_simiition;
    bool quiet = 0;

    //Setup test configuration
    test_config tst_cfg;

    //Set long options
    static struct option long_options[] =
        {
          /* These options set a flag. */
       //   {"quiet", no_argument,       &quiet, 1},
          /* These options don�t set a flag.
             We distinguish them by their indices. */
          {"quiet",      no_argument,       0, 'q'},
          {"testname",   required_argument, 0, 't'},
          {"result_path",required_argument, 0, 'r'},
          {"seed"       ,required_argument, 0, 's'},
          {"log_str",    required_argument, 0, 'l'},
	  {"verbosity",  required_argument, 0, 'v'},
          {0, 0, 0, 0}
        };
    //!Get input from prompt to select the options: testname, result_path and the seed.
    while ((opt = getopt(argc, argv, "t:l:s:v:q n:")) != -1)
    {
        switch (opt)
        {
        case 't':
            testname = optarg;
            break;
      	case 'l': 
	    result_path = optarg;
            break;
        case 's':
            seed_input = optarg;
            break;
	case 'v':
            verbosity_input = optarg;
            break;
        case 'q':
            quiet = 1;
            break;
	case 'n':
            NI_str = optarg;
            break;
        default:
            break;
        }
    }

    for(int i = 0; i<argc; i++ ){
        input_argv = input_argv + argv[i] + " ";
    }
    //Convert input from prompt string2double
    istringstream convert(NI_str);
    if(!(convert >> tst_cfg.NI)){tst_cfg.NI=10;}
    convert.clear();

    convert.str(seed_input);
    if(!(convert >> tst_cfg.seed)){tst_cfg.seed=1;}
    convert.clear();

    //set testname
    tst_cfg.testname = testname;

    //set verbosity
    if( verbosity_input == "HIGH"){ verbosity_simiition = HIGH; }
    else if( verbosity_input == "MEDIUM"){ verbosity_simiition = MEDIUM; }
    else if( verbosity_input == "LOW"){ verbosity_simiition = LOW; }
    else { SC_REPORT_FATAL("ii_top_tb", "Vebosity format not specify!");}

    
    log_fname = result_path + "/" + testname + "_" + seed_input + ".log";

    report_server::report::set_log_file_name(log_fname.c_str());
    report_server::report::set_verbosity(verbosity_simiition, quiet);
    INFO("main", input_argv.c_str());

    //Used to generated VCD file
    string trace_fname;
    trace_fname = result_path + testname;

    //-------------------------
    // Conect and call Tests
    //--------------------------
    ii_top_tb ii_top_tb("ii_top_tb", testname, trace_fname, &tst_cfg);

    //-----------------------------
    // Start Simiition
    //-----------------------------
    SC_REPORT_INFO(main_ID,"Start Simiition...");
    sc_start(II_TIMEOUT,II_TIMEUNIT);
    SC_REPORT_INFO(main_ID,"End Simiition...");
    
    // high_resolution_clock::time_point t2 = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    // cout << "time: " << duration << endl;

    return 0;
}


