#ifndef ARMVirtualBusFederate_H_
#define ARMVirtualBusFederate_H_

#include "FederateAmbassador.h"
#include "RTI.hh"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
//#include <highgui.hpp> 
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
//#include <cv.h>
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include <thread>
//#include <mutex>
#include <unistd.h>
#include <time.h>

using namespace std;
using namespace cv;


#define READY_TO_RUN "ReadyToRun"
#define FRAME_RATE 15
#define NUM_ELEMENTS 2

class VirtualBusFederate
{
	public:

		RTI::RTIambassador *rtiamb;
		FederateAmbassador *fedamb;
		char federateName[12];
		


		// fom handles //
        // RTI::ObjectHandle           oHandle[2];
		// RTI::ObjectClassHandle      aHandle[2];
		
		// RTI::AttributeHandle        srcHandle;
		// RTI::AttributeHandle        addrHandle;
		// RTI::AttributeHandle        sizeHandle;
		// RTI::AttributeHandle        data1Handle;
		// RTI::AttributeHandle        data2Handle;
		// RTI::AttributeHandle        data3Handle;		
		// RTI::AttributeHandle        data4Handle;
		// RTI::AttributeHandle        data5Handle;
		// RTI::AttributeHandle        data6Handle;
		// RTI::AttributeHandle        data7Handle;
		// RTI::AttributeHandle        data8Handle;
		// RTI::AttributeHandle        dataFrameHandle;
        // RTI::AttributeHandle        testHandle;

        RTI::ObjectHandle*           _oHandle;
        RTI::ObjectClassHandle*      _aHandle;
        RTI::AttributeHandle**       _attributeHandle;
        RTI::AttributeHandle**       _attributeHandle_toSubscribe;

        std::vector<std::string> _objClassHandle_names;
        std::vector<std::string>* _attributeHandle_names;
        // std::multimap<std::string, std::string> _class_and_attribute_Hnames
		

		// public methods //
		VirtualBusFederate();
        VirtualBusFederate(std::vector<std::string> objClassHandle_names, std::vector<std::string>* attributeHandle_names, std::vector<std::string>* attributeHandle_names_sub = nullptr); 
        // VirtualBusFederate(std::vector<std::string> objClassHandle_names, std::multimap<std::string, std::string> class_and_attribute_Hnames); 
		virtual ~VirtualBusFederate();
		void runFederate( char* federateName );
		//void loop(int iteracoes, std::vector<cv::Rect>* f);
		void loop(int interacoes, char srcVideo[]);
		void finalize();


		void writeData(string src, string addr, string size, string data1, string data2, string data3, string data4, string data5,string data6, string data7, string data8, unsigned char* dataFrame);
		
		bool readData(string& src, string& addr, string& size, string& data1, string& data2, string& data3, string& data4, string& data5, string& data6, string& data7, string& data8, unsigned char* dataFrame);
		
		bool hasReceivedData();
		
		void advanceTime( double timestep );

		//void* detect(std::vector<cv::Rect>* f);

	private:

		bool receivedData;

		//auxiliares
		bool loopM = true;
		bool draw = false;
		bool sendInfo = false;
		bool sendImg = false;
		stringstream ss;
		int frameCount = 0;
		ofstream log;
		clock_t start, capture;
		double difTime;
		unsigned int countCrop = 0;
		unsigned int countBits = 0;


		//Vaŕiáveis compartilhadas do HLA
		/**
			Para economia de esforço, os nomes variáveis que são utilizadas no HLA foram 
			mantidas, porém os significados são diferentes, abaixo o que indica cada uma.

			source = número do câmera/nome do federado
			address = variável que controla se a câmera ou o "servidor" deve escutar o read()
			size = Tempo do vídeo
			data1 = Frame do vídeo
			data2 = Tempo do HLA (o que é feito pelo advanced time, por exemplo)
			data3 = Quantidade de faces encontradas
			data4 = flag que diz que faces foram encontradas (TALVEZ MUDAR ISSO PRA O QUANTIADE DE FACES)
			data5 = Tempo rodado na máquia (clock)
			data6 = livre... talvez seja o sendCrop (revisar)
			data7 = livre...
			data8 = livre...
			dataFrame = foto.

		*/
		string source;
		string address;
		string size;
		string data1;
		string data2;
		string data3;
		string data4;
		string data5;
		string data6;
		string data7;
		string data8;
		unsigned char dataFrame[NUM_ELEMENTS];
	
		//Variáveis de vídeo
		Mat frame;
		Mat frame_gray;
		bool success;
		double fps;
		std::vector<Rect> faces;
		vector<Mat> croppedImages;
		CascadeClassifier face_cascade;
		VideoCapture v;
 		string face_cascade_name = "/home/felipe/Documentos/OPENCV_3.1/data/haarcascades/haarcascade_frontalface_alt.xml";
	
		
		void initializeHandles();
		void waitForUser();
		void enableTimePolicy();
		void publishAndSubscribe();
		RTI::ObjectHandle registerObject();
        RTI::ObjectHandle registerObject(std::string objClass_name);
		void updateAttributeValues(string src, string addr, string size, string data1, string data2, string data3, string data4, string data5, string data6, string data7, string data8, unsigned char* dataFrame);
		void sendInteraction();
		
		void deleteObject( RTI::ObjectHandle* objectHandle );
		double getLbts();
		double getCurrentTime();

};

#endif /*ARMVirtualBusFederate_H_*/
