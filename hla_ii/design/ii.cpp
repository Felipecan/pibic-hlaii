// MUITO PROVAVELMENTE ESSE ARQUIVO NAO ESTA CORRETO. PRECISA AJEITAR, CASO QUEIRA USAR O AMBIENTE COM COMUNICACAO VIA HLA SOMENTE NO DUT.


#include "ii.h"

#define IMAGE_WIDTH 4
#define IMAGE_HEIGHT 4

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

  // federate = new VirtualBusFederate();
  // federate->runFederate( federateName );

  SC_CTHREAD(func,clock.pos());
  async_reset_signal_is(reset_n, false);    
}



void ii::func()
{
   data_t temp_out_data;
   bool  temp_carryout;
   int temp_sel;
   unsigned a[IMAGE_HEIGHT][IMAGE_WIDTH];
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

        a[0][0] = data[0] ;
        a[0][1] = data[1] ;
        a[0][2] = data[2] ;
        a[0][3] = data[3] ;
        a[1][0] = data[4] ;
        a[1][1] = data[5] ;
        a[1][2] = data[6] ;
        a[1][3] = data[7] ;
        a[2][0] = data[8] ;
        a[2][1] = data[9] ;
        a[2][2] = data[10];
        a[2][3] = data[11];
        a[3][0] = data[12];
        a[3][1] = data[13];
        a[3][2] = data[14];
        a[3][3] = data[15];

            //First compute the first horizontal line
        for(int j = 1; j < IMAGE_WIDTH; j++)
            a[0][j] += a[0][j-1];

                    //Then compute the first vertical line
        for(int i = 1; i < IMAGE_HEIGHT; i++)
            a[i][0] += a[i-1][0];

                    //And finally compute the rest of the values
        for(int i = 1; i < IMAGE_HEIGHT; i++)
            for(int j = 1; j < IMAGE_WIDTH; j++)
                a[i][j] += a[i-1][j]+a[i][j-1]-a[i-1][j-1];

        data[0]  = a[0][0];
        data[1]  = a[0][1];
        data[2]  = a[0][2];
        data[3]  = a[0][3];
        data[4]  = a[1][0];
        data[5]  = a[1][1];
        data[6]  = a[1][2];
        data[7]  = a[1][3];
        data[8]  = a[2][0];
        data[9]  = a[2][1];
        data[10] = a[2][2];
        data[11] = a[2][3];
        data[12] = a[3][0];
        data[13] = a[3][1];
        data[14] = a[3][2];
        data[15] = a[3][3];                

        for (int i = 0; i < DATA_SIZE; ++i) 
        {
            temp_out_data = data[i];
            out_data[i].write(temp_out_data);
        }
        out_data_valid.write(1);                

        //   src = 1;
        //   addr = 1;
        //   federate->writeData(src, addr, size,  data);
        //   federate->advanceTime(1.0);
        //   if(federate->readData(src,addr,size, data)){
        //       for (int i = 0; i < DATA_SIZE; ++i) {
        //           temp_out_data = data[i];
        //           out_data[i].write(temp_out_data);
        //       }
        //       out_data_valid.write(1);
        //   }
      }
      wait(1);
   }
}
