#ifndef ARMVirtualBusFederate_H_
#define ARMVirtualBusFederate_H_

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
		RTI::ObjectHandle      	    oHandle;		
		RTI::ObjectClassHandle      aHandle;
		RTI::AttributeHandle        srcHandle;
		RTI::AttributeHandle        addrHandle;
		RTI::AttributeHandle        sizeHandle;
		RTI::AttributeHandle        data0Handle;
		RTI::AttributeHandle        data1Handle;
		RTI::AttributeHandle        data2Handle;
		RTI::AttributeHandle        data3Handle;
		
		RTI::AttributeHandle        data4Handle;
		RTI::AttributeHandle        data5Handle;
		RTI::AttributeHandle        data6Handle;
		RTI::AttributeHandle        data7Handle;
		RTI::AttributeHandle        data8Handle;
		RTI::AttributeHandle        data9Handle;
		RTI::AttributeHandle        data10Handle;
		RTI::AttributeHandle        data11Handle;	
		RTI::AttributeHandle        data12Handle;
		RTI::AttributeHandle        data13Handle;
		RTI::AttributeHandle        data14Handle;
		RTI::AttributeHandle        data15Handle;
		
		

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
		void updateAttributeValues(unsigned src, unsigned addr, unsigned size, unsigned *data);
		void sendInteraction();
		
		void deleteObject( RTI::ObjectHandle objectHandle );
		double getLbts();
		double getCurrentTime();
};

#endif /*ARMVirtualBusFederate_H_*/
