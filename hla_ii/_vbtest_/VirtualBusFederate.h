#ifndef ARMVirtualBusFederate_H_
#define ARMVirtualBusFederate_H_

#include "FederateAmbassador.h"
#include "RTI.hh"

// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/opencv.hpp>

// #include "opencv2/highgui.hpp"
// #include "opencv2/core.hpp"
// #include <highgui.h> 
// #include "opencv2/objdetect.hpp"
// #include "opencv2/imgproc.hpp"
// #include <cv.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include <thread>
//#include <mutex>
#include <unistd.h>
#include <time.h>

using namespace std;
// using namespace cv;


#define READY_TO_RUN "ReadyToRun"


#include <map>
#include <vector>


class VirtualBusFederate
{
	public:

		RTI::RTIambassador *rtiamb;
		FederateAmbassador *fedamb;
		char federateName[12];
		


		// fom handles //
		vector<RTI::ObjectHandle>   oHandle1;        
		RTI::ObjectClassHandle      aHandle1;
        RTI::ObjectClassHandle      aHandle2;
		RTI::AttributeHandle        srcHandle1;
		RTI::AttributeHandle        addrHandle1;
		RTI::AttributeHandle        sizeHandle1;
        RTI::AttributeHandle        srcHandle2;
		RTI::AttributeHandle        addrHandle2;
		RTI::AttributeHandle        sizeHandle2;

        // FOM handles //
        // vector<RTI::ObjectHandle>      	    ohandles_publish; // ok ok
        // vector<RTI::ObjectHandle>      	    ohandles_subscribe; // ok ok
        // vector<RTI::ObjectClassHandle>      ahandles_publish; // ok
        // vector<RTI::ObjectClassHandle>      ahandles_subscribe; // ok
        // RTI::AttributeHandle**      attributes_handles_subscribe;
        // RTI::AttributeHandle**      attributes_handles_publish;

        // RTI::ObjectHandle*      	ohandles_publish;
        // RTI::ObjectHandle*      	ohandles_subscribe;
        // RTI::ObjectClassHandle*     ahandles_publish;
        // RTI::ObjectClassHandle*     ahandles_subscribe;
        // RTI::AttributeHandle**      attributes_handles_subscribe;
        // RTI::AttributeHandle**      attributes_handles_publish;


        // multimap <string, string> c_and_attribute_names_subscribe;
		
		

		// public methods //
		VirtualBusFederate();
        //VirtualBusFederate(vector<string> obj_class_names, multimap <string, string> c_and_attribute_names_publish, multimap <string, string> c_and_attribute_names_subscribe);
		virtual ~VirtualBusFederate();
		void runFederate( char* federateName );		
		void loop(int interacoes, char srcVideo[]);
		void finalize();


		void writeData(string src, string addr, string size);		
		bool readData(string& src, string& addr, string& size);		
		bool hasReceivedData();		
		void advanceTime( double timestep );

		//void* detect(std::vector<cv::Rect>* f);

	private:

		bool receivedData;

        // vector<string> obj_class_names; // ok ok
        // multimap <string, string> c_and_attribute_names_publish;
        // multimap <string, string> c_and_attribute_names_subscribe;

		string source;
		string address;
		string size;
		
					
		void initializeHandles();
		void waitForUser();
		void enableTimePolicy();
		void publishAndSubscribe();
		RTI::ObjectHandle registerObject();
        RTI::ObjectHandle registerObject(string s);
		void updateAttributeValues(string src, string addr, string size);
		void sendInteraction();
		
		void deleteObject( RTI::ObjectHandle objectHandle );
		double getLbts();
		double getCurrentTime();

};

#endif /*ARMVirtualBusFederate_H_*/
