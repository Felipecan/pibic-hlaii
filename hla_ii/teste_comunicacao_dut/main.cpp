#include <string>
#include <iostream>
#include <cstdio>
#include "VirtualBusFederate.h"

#define VIRTUALBUS_SIZE 16
#define IMAGE_WIDTH 4
#define IMAGE_HEIGHT 4

#define WINDOW_WIDTH 3
#define WINDOW_HEIGHT 36
	//Use to HLA
unsigned int  size;
unsigned int data[VIRTUALBUS_SIZE];


int main( int argc, char **argv )
{
	//****************************************************
	    //*********************** HLA ************************
	//****************************************************
	char federateName[20];
	// check to see if we have a federate name
	//strcpy(federateName, "FPGAFederate");
	//sprintf(federateName, "%d", FPGA_ID);
	sprintf(federateName, "%d", 1);
	//	if( argc > 1 )
	//		strcpy(federateName,argv[1]);

	// create and run the federate
	VirtualBusFederate *federate;
	federate = new VirtualBusFederate();
	federate->runFederate( federateName );

	//Data to receive
    unsigned src;
	unsigned addr;
    unsigned readsrc;
    unsigned cntrl_write;
	
	int a[IMAGE_HEIGHT][IMAGE_WIDTH];

	while(1)
	{
        readsrc = 0;
		if(federate->readData(readsrc, addr, size, data))
		{
            //std::cout << "Leu o if " << readsrc << std::endl;
            if(readsrc == 0)
            {                   
                std::cout << "data from src " << readsrc << ":";
                for (size_t i = 0; i < VIRTUALBUS_SIZE; i++) 
                {
                    std::cout << " " << data[i];
                }
                std::cout << std::endl;
                //for(int aux = 0; aux < 16; aux++) std::cout << "data[" << aux << "] " << data[aux] << std::endl;
                //send data by HLA
                addr = SENDER_ID; //Sender address
                a[0][0] = data[0];
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


                data[0]  = a[0][0]+1;
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

                src = 1;

                federate->writeData(src, addr, size,  data);
                std::cout << "New data (DUT):";
                for(int aux = 0; aux < VIRTUALBUS_SIZE; aux++) 
                    std::cout << " " << data[aux];
                std::cout << std::endl;
            }   
	    }

        federate->advanceTime(1.0);

	  	
	}

}
