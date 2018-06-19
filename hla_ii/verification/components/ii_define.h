#include <report_server.h>

#define II_SEED          1
#define II_TIMEOUT       20000
#define II_TIMEUNIT      SC_NS

#define DATA_NUMBER_BITS  8
#ifndef DATA_SIZE
    #define DATA_SIZE         16
#endif

#ifndef  II_TEST_CONFIG
#define II_TEST_CONFIG
  struct test_config {
    int          NI; // Number of interactions
    unsigned int seed; //Seed used in randomization and PRBS
    string testname; 
  };
#endif

typedef sc_uint<DATA_NUMBER_BITS> dut_data_t;

