#include "ii.h"

SC_HAS_PROCESS(ii);
ii::ii(sc_module_name nm):
    sc_module   (nm),
    clock       ("clock"),
    in_data_en  ("in_data_en"),
    reset_n     ("reset_n"),
    out_data_valid("out_data_valid")
{
  //strcpy(federateName, "FPGAFederate");
  sprintf(federateName, "%d", SENDER_ID);

  federate = new VirtualBusFederate();
  federate->runFederate( federateName );

  SC_CTHREAD(func,clock.pos());
  async_reset_signal_is(reset_n, false);    
}



void ii::func()
{
   data_t temp_out_data;
   bool  temp_carryout;
   int temp_sel;
   // reset
   while (true)
   {
      if(reset_n.read() == false) {
          out_data_valid.write(0);
          for (int i = 0; i < DATA_SIZE; ++i) {
              out_data[i].write(0);
          }
      }
      else if (in_data_en.read() == 1){
          for (int i = 0; i < DATA_SIZE; ++i) {
              data[i] = in_data[i].read();

          }
          for(int i =DATA_SIZE ; i<16; i++)
              data[i] = 9;

          src = 1;
          addr = 1;
          federate->writeData(src, addr, size,  data);
          federate->advanceTime(1.0);
          if(federate->readData(src,addr,size, data)){
              for (int i = 0; i < DATA_SIZE; ++i) {
                  temp_out_data = data[i];
                  out_data[i].write(temp_out_data);
              }
              out_data_valid.write(1);
          }
      }
      wait(1);
   }
}
