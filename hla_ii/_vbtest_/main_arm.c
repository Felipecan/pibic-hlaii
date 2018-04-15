#define soc_cv_av

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
#include "led.h"
#include "seg7.h"
#include <stdbool.h>
#include <pthread.h>

#include "VirtualBusFederate.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define BUTTON_MASK     (0x02000000)

#define SWITCH_BASE (0x00010040)


volatile unsigned long *h2p_lw_led_addr=NULL;
volatile unsigned long *h2p_lw_hex_addr=NULL;
volatile unsigned long *h2p_sw_addr=NULL;

void led_blink(void)
{
	int i=0;

	printf("LED ON \r\n");
	for(i=0;i<=3;i++){
			LEDR_LightCount(i);
			usleep(100*1000);
		}
	printf("LED OFF \r\n");
	for(i=0;i<=3;i++){
			LEDR_OffCount(i);
			usleep(100*1000);
		}
	
}

int main(int argc, char **argv)
{
	pthread_t id;
	int ret;
	void *virtual_base;
	int fd;
	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );	
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return(1);
	}

	//aqui são calculados os endereços dos dispositivos que serão acessados
	h2p_lw_led_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + LED_PIO_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_hex_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + SEG7_IF_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_sw_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + SWITCH_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	
	ret=pthread_create(&id,NULL,(void *)led_blink,NULL);
	
	if(ret!=0){
		printf("Creat pthread error!\n");
		exit(1);	
	}


	SEG7_All_Number();


	//***************************************************** HLA *************************************************************************

	

       
	while(1){

		// check to see if we have a federate name
        char* federateName = "exampleFederate";
        if( argc > 1 )
                federateName = argv[1];

		VirtualBus *vb = new VirtualBus(federateName);
		
		vb->write(h2p_lw_hex_addr+0,(unsigned char)data);

		unsigned src, addr=0, size, data;
		if(federate->readData(src,addr,size, data)){
			printf("main: Data %d received from RTI and sent to FPGA\n", data);
			alt_write_word(h2p_lw_hex_addr+0,(unsigned char)data);
		}
		
		//************* Lendo o botão *********************

    	bool bIsPressed;
    	uint32_t scan_input = 0;

    	scan_input = ~alt_read_word( (void *) ((char *)virtual_base + ( ( uint32_t )(  ALT_GPIO1_EXT_PORTA_ADDR ) & ( uint32_t )( HW_REGS_MASK ) ) ) );

    	bIsPressed = (scan_input & BUTTON_MASK);

		if (bIsPressed){
			unsigned src2=1, addr2=0, size2=1, data2=scan_input;
                	federate->writeData(src2,addr2,size2, data2);
		}

		//******************** Lendo chaves ***********************************
 		uint32_t last_input=0, sw_input = 0;
                sw_input = alt_read_word( (void *) ((char *)h2p_sw_addr) );
    		if (last_input != sw_input){
				unsigned src3=1, addr3=0, size3=1;
				last_input = sw_input;

                federate->writeData(src3, addr3, size3, last_input);
				printf("\nLeitura das chaves: %d\n", last_input);

            }

		usleep(1000000);
		federate->advanceTime(1.0);

	}
        
        federate->finalize();

        // clean up
        delete federate;
	//******************************************
	


	pthread_join(id,NULL);
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );

	}
	close( fd );


	return 0;
}
