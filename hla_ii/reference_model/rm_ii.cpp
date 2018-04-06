#include "rm_ii.h"

SC_HAS_PROCESS(rm_ii);
rm_ii::rm_ii(sc_module_name nm):
    sc_module   (nm),
	clock       ("clock"),
	in_data_en  ("in_data_en"),
	reset_n     ("reset_n"),
	out_data_valid("out_data_valid")
{
  SC_CTHREAD(func,clock.pos());
  async_reset_signal_is(reset_n, false);    
}



void rm_ii::func()
{
    sc_uint<NBITS_SIZE+1> temp_out_data;
    int a[IMAGE_HEIGHT][IMAGE_WIDTH];
   // reset
   while (true)
   {
      if(reset_n.read() == false) {
		  for (int i = 0; i < DATA_SIZE; ++i) {
			  out_data[i].write(0);

		  }
		  out_data_valid.write(0);
      }
      else if (in_data_en.read() == 1){
          a[0][0] = in_data[0].read();
          a[0][1] = in_data[1].read();
          a[0][2] = in_data[2].read();
          a[0][3] = in_data[3].read();
          a[1][0] = in_data[4].read();
          a[1][1] = in_data[5].read();
          a[1][2] = in_data[6].read();
          a[1][3] = in_data[7].read();
          a[2][0] = in_data[8].read();
          a[2][1] = in_data[9].read();
          a[2][2] = in_data[10].read();
          a[2][3] = in_data[11].read();
          a[3][0] = in_data[12].read();
          a[3][1] = in_data[13].read();
          a[3][2] = in_data[14].read();
          a[3][3] = in_data[15].read();
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


          out_data[0] .write(a[0][0]);
          out_data[1] .write(a[0][1]);
          out_data[2] .write(a[0][2]);
          out_data[3] .write(a[0][3]);
          out_data[4] .write(a[1][0]);
          out_data[5] .write(a[1][1]);
          out_data[6] .write(a[1][2]);
          out_data[7] .write(a[1][3]);
          out_data[8] .write(a[2][0]);
          out_data[9] .write(a[2][1]);
          out_data[10].write(a[2][2]);
          out_data[11].write(a[2][3]);
          out_data[12].write(a[3][0]);
          out_data[13].write(a[3][1]);
          out_data[14].write(a[3][2]);
          out_data[15].write(a[3][3]);
          out_data_valid.write(1);

      }
      wait(1);
   }
}
