#ifndef ARMVirtualBusFederate_H_
#define ARMVirtualBusFederate_H_

#include <string>
#include <cstring>

#include "FederateAmbassador.h"
#include "RTI.hh"

#define READY_TO_RUN "ReadyToRun"

#define SENDER_ID 0
#define FPGA_ID 1
#define OCL_PROCESSOR_ID 2

class VirtualBusFederate
{
	public:
		RTI::RTIambassador *rtiamb;
		FederateAmbassador      *fedamb;
		char federateName[12];

		// fom handles //
		RTI::ObjectHandle      	    oHandle_dut;		
		RTI::ObjectClassHandle      aHandle_dut;

        RTI::ObjectHandle      	    oHandle_ref;		
		RTI::ObjectClassHandle      aHandle_ref;

		RTI::AttributeHandle        srcHandle_dut;
		RTI::AttributeHandle        addrHandle_dut;
		RTI::AttributeHandle        sizeHandle_dut;
		RTI::AttributeHandle        data0Handle_dut;
		RTI::AttributeHandle        data1Handle_dut;
		RTI::AttributeHandle        data2Handle_dut;
		RTI::AttributeHandle        data3Handle_dut;		
		RTI::AttributeHandle        data4Handle_dut;
		RTI::AttributeHandle        data5Handle_dut;
		RTI::AttributeHandle        data6Handle_dut;
		RTI::AttributeHandle        data7Handle_dut;
		RTI::AttributeHandle        data8Handle_dut;
		RTI::AttributeHandle        data9Handle_dut;
		RTI::AttributeHandle        data10Handle_dut;
		RTI::AttributeHandle        data11Handle_dut;	
		RTI::AttributeHandle        data12Handle_dut;
		RTI::AttributeHandle        data13Handle_dut;
		RTI::AttributeHandle        data14Handle_dut;
		RTI::AttributeHandle        data15Handle_dut;

        RTI::AttributeHandle        srcHandle_ref;
		RTI::AttributeHandle        addrHandle_ref;
		RTI::AttributeHandle        sizeHandle_ref;
		RTI::AttributeHandle        data0Handle_ref;
		RTI::AttributeHandle        data1Handle_ref;
		RTI::AttributeHandle        data2Handle_ref;
		RTI::AttributeHandle        data3Handle_ref;		
		RTI::AttributeHandle        data4Handle_ref;
		RTI::AttributeHandle        data5Handle_ref;
		RTI::AttributeHandle        data6Handle_ref;
		RTI::AttributeHandle        data7Handle_ref;
		RTI::AttributeHandle        data8Handle_ref;
		RTI::AttributeHandle        data9Handle_ref;
		RTI::AttributeHandle        data10Handle_ref;
		RTI::AttributeHandle        data11Handle_ref;	
		RTI::AttributeHandle        data12Handle_ref;
		RTI::AttributeHandle        data13Handle_ref;
		RTI::AttributeHandle        data14Handle_ref;
		RTI::AttributeHandle        data15Handle_ref;          
		
		

		// public methods //
		VirtualBusFederate();
		virtual ~VirtualBusFederate();
		void runFederate( char* federateName );
		void loop(int iteracoes);
		void finalize();

		
        void writeData(unsigned src, unsigned addr, unsigned size, unsigned *data);
		bool readData(unsigned& src, unsigned& addr, unsigned& size, unsigned *data);		
		bool hasReceivedData();	
		void advanceTime( double timestep );




	private:

		bool receivedData;
	
		
		void initializeHandles();
		void waitForUser();
		void enableTimePolicy();
		void publishAndSubscribe();
		RTI::ObjectHandle registerObject();
        RTI::ObjectHandle registerObject(std::string s);
        void updateAttributeValues(unsigned src, unsigned addr, unsigned size, unsigned *data);		
        void sendInteraction();
		
		void deleteObject( RTI::ObjectHandle objectHandle );
		double getLbts();
		double getCurrentTime();
};

#endif /*ARMVirtualBusFederate_H_*/
