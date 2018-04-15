#include <iostream>
#include "RTI.hh"
#include "fedtime.hh"

#include "FederateAmbassador.h"
#include "VirtualBusFederate.h"
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

VirtualBusFederate::VirtualBusFederate()
{
}

// [VirtualBus.dut, VirtualBus.refmod], [VB.dut: data1..., VB.refmod: data1]
VirtualBusFederate::VirtualBusFederate(std::vector<std::string> objClassHandle_names, std::vector<std::string>* attributeHandle_names, 
                                       std::vector<std::string>* attributeHandle_names_sub = nullptr)
{
    // assigns to the attributes of the class the arguments of the method
    this->_objClassHandle_names = objClassHandle_names;
    this->_attributeHandle_names = attributeHandle_names;

    // allocates a vector with Object Class...       
    this->_aHandle = new RTI::ObjectClassHandle[ objClassHandle_names.size() ];
    
    // allocates the rows of the attribute vector, each row an Object Class..
    this->_attributeHandle = new RTI::AttributeHandle*[ objClassHandle_names.size() ]    
    for(unsigned int i = 0; i < objClassHandle_names.size(); i++)
        this->attributeHandle[i] = new RTI::AttributeHandle[ attributeHandle_names[i].size() ];

    if(attributeHandle_names_sub != nullptr)
    {
        // aloca a matriz
    }
    else
    {
        // seta o atributo da classe para nullptr
    }
}

VirtualBusFederate::~VirtualBusFederate()
{
	if( this->fedamb )
		delete this->fedamb;
}


///////////////////////////////////////////////////////////////////////////
////////////////////////// Meus Metodos /////////////////////////
///////////////////////////////////////////////////////////////////////////

void VirtualBusFederate::writeData(string src, string addr, string size, string data1, string data2, string data3, string data4, string data5, string data6, string data7, string data8, unsigned char* dataFrame){
	updateAttributeValues(src, addr, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
	//advanceTime(fedamb->federateLookahead);
}


bool VirtualBusFederate::readData(string& source, string& addr, string& size, string &data1, string &data2, string &data3, string &data4, string &data5, string &data6, string &data7, string &data8, unsigned char* dataFrame){
	if(fedamb->hasReceivedData())
	{
		fedamb->getReceivedData(source, addr, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
        //cout << ">>>> x=" << source << " y=" << addr;
        //cout << endl << ">>> Receiving data: " << data[0] << " " << data[1] << " " << data[2] << endl;
        //cout << "OI" << endl;
		return true;
	}
	else
		return false;

}

bool VirtualBusFederate::hasReceivedData(){
	return receivedData;
}


void VirtualBusFederate::loop (int iteracoes, char srcVideo[])
{
	//stringstream ss;
	int i;
    //ref = atoi(srcVideo);
	while(true)
	{   
        if(srcVideo[0] == '0')
        {
            //std::cout << "Entrou na merda do if" << std::endl;
            source = "9999";
            address = "96";
            size = "99";
            data1 = "1";
            data2 = "2";
            data3 = "3";
            data4 = "4";
            data5 = "5";
            data6 = "6";
            data7 = "7";
            data8 = "8";
            //ss.str(std::string());
            
            writeData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
            
        }
        else
        {
            //std::cout << "Entrou na merda do else" << std::endl;
            if(readData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame))
		    {
                cout << "recebendo dados" << endl;
				cout << "source " << source << endl;
				cout << "address " << address << endl;
				cout << "size " << size << endl;
				cout << "data1 " << data1 << endl;
				cout << "data2 " << data2 << endl;
				cout << "data3 " << data3 << endl;
				cout << "data4 " << data4 << endl;
				cout << "data5 " << data5 << endl;
				cout << "data6 " << data6 << endl;
				cout << "data7 " << data7 << endl;
				cout << "data8 " << data8 << endl;
				//cout << "dataFrame[0] " << dataFrame[0] << endl;
				//cout << "dataFrame[1] " << dataFrame[1] << endl;
				cout << "dados recebidos" << endl;
            }
            else
            {
                cout << "Nao chegou nada, aparentemente" << endl;
            }
        }
        advanceTime(1.0);
		// if(readData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame))
		// {
		// 	if(source == "0" && address == "66")
		// 	{
		// 		cout << "recebendo dados" << endl;
		// 		cout << "source " << source << endl;
		// 		cout << "address " << address << endl;
		// 		cout << "size " << size << endl;
		// 		cout << "data1 " << data1 << endl;
		// 		cout << "data2 " << data2 << endl;
		// 		cout << "data3 " << data3 << endl;
		// 		cout << "data4 " << data4 << endl;
		// 		cout << "data5 " << data5 << endl;
		// 		cout << "data6 " << data6 << endl;
		// 		cout << "data7 " << data7 << endl;
		// 		cout << "data8 " << data8 << endl;
		// 		//cout << "dataFrame[0] " << dataFrame[0] << endl;
		// 		//cout << "dataFrame[1] " << dataFrame[1] << endl;
		// 		cout << "dados recebidos" << endl;

		// 		source = federateName;
		// 		address = "96";
		// 		size = "99";
		// 		data1 = "1";
		// 		data2 = "2";
		// 		data3 = "3";
		// 		data4 = "4";
		// 		data5 = "5";
		// 		data6 = "6";
		// 		data7 = "7";
		// 		data8 = "8";
		// 		//ss.str(std::string());
				
		// 		writeData(source, address, size, data1, data2, data3, data4, data5, data6, data7, data8, dataFrame);
				
		// 	}
		// }
		// else
		// {
		// 	cout << "Nao chegou nada, aparentemente" << endl;
		// }

		// //ss << i;
		// //data8 = ss.str();
		// advanceTime(1.0);
		// //cin >> i;
	}
}

double VirtualBusFederate::getCurrentTime()
{
	
	return fedamb->federateTime;
	
}


///////////////////////////////////////////////////////////////////////////
////////////////////////// Main Simulation Method /////////////////////////
///////////////////////////////////////////////////////////////////////////
void VirtualBusFederate::runFederate( char* federateName )
{
	//this->federateName = 
	strcpy(this->federateName, federateName);

	/////////////////////////////////
	// 1. create the RTIambassador //
	/////////////////////////////////
	this->rtiamb = new RTI::RTIambassador();

	//////////////////////////////////////////
	// 2. create and join to the federation //
	//////////////////////////////////////////
	// create
	// NOTE: some other federate may have already created the federation,
	//       in that case, we'll just try and join it
	try
	{
		rtiamb->createFederationExecution( "ARMVirtualBusFederation", "virtualbus.fed" );
		cout << "Created Federation" << endl;
	}
	catch( RTI::FederationExecutionAlreadyExists exists )
	{
		cout << "Didn't create federation, it already existed" << endl;
	}

	////////////////////////////
	// 3. join the federation //
	////////////////////////////
	// create the federate ambassador and join the federation
	this->fedamb = new FederateAmbassador(federateName);
	rtiamb->joinFederationExecution( federateName, "ARMVirtualBusFederation", fedamb );
	cout << "Joined Federation as " << federateName << endl;

	// initialize the handles - have to wait until we are joined
	initializeHandles();

	cout << "initializeHandles "  << endl;


	////////////////////////////////
	// 4. announce the sync point //
	////////////////////////////////
	// announce a sync point to get everyone on the same page. if the point
	// has already been registered, we'll get a callback saying it failed,
	// but we don't care about that, as long as someone registered it
    // if(strcmp( federateName, "2" ) == 0)
    // {
	//     rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, "" );
    // }
    // else
    // {
    //     // rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, "" );
    // }
    rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, "" );
	while( fedamb->isAnnounced == false )
	{
		rtiamb->tick();
	}
	cout << "registerFederationSynchronizationPoint "  << endl;

	// WAIT FOR USER TO KICK US OFF
	// So that there is time to add other federates, we will wait until the
	// user hits enter before proceeding. That was, you have time to start
	// other federates.

	waitForUser();

	///////////////////////////////////////////////////////
	// 5. achieve the point and wait for synchronization //
	///////////////////////////////////////////////////////
	// tell the RTI we are ready to move past the sync point and then wait
	// until the federation has synchronized on
	rtiamb->synchronizationPointAchieved( READY_TO_RUN );
	cout << "Achieved sync point: " << READY_TO_RUN << ", waiting for federation..." << endl;
	while( fedamb->isReadyToRun == false )
	{
		rtiamb->tick();
	}

	/////////////////////////////
	// 6. enable time policies //
	/////////////////////////////
	// in this section we enable/disable all time policies
	// note that this step is optional!
	enableTimePolicy();
	cout << "Time Policy Enabled" << endl;

	//////////////////////////////
	// 7. publish and subscribe //
	//////////////////////////////
	// in this section we tell the RTI of all the data we are going to
	// produce, and all the data we want to know about
	publishAndSubscribe();
	cout << "Published and Subscribed" << endl;

	/////////////////////////////////////
	// 8. register an object to update //
	/////////////////////////////////////
	//oHandle = registerObject();
    for(unsigned int i = 0; i < _objClassHandle_names; i++)
        oHandle = registerObject(_objClassHandle_names[i]);
	cout << "Registered Object, handle=" << oHandle << endl;

	


	
}

void VirtualBusFederate::finalize(){

	//////////////////////////////////////
	// 10. delete the object we created //
	//////////////////////////////////////
	deleteObject(oHandle);
	cout << "Deleted Object, handle=" << oHandle << endl;

	////////////////////////////////////
	// 11. resign from the federation //
	////////////////////////////////////
	rtiamb->resignFederationExecution( RTI::NO_ACTION );
	cout << "Resigned from Federation" << endl;

	////////////////////////////////////////
	// 12. try and destroy the federation //
	////////////////////////////////////////
	// NOTE: we won't die if we can't do this because other federates
	//       remain. in that case we'll leave it for them to clean up
	try
	{
		rtiamb->destroyFederationExecution( "ARMVirtualBusFederation" );
		cout << "Destroyed Federation" << endl;
	}
	catch( RTI::FederationExecutionDoesNotExist dne )
	{
		cout << "No need to destroy federation, it doesn't exist" << endl;
	}
	catch( RTI::FederatesCurrentlyJoined fcj )
	{
		cout << "Didn't destroy federation, federates still joined" << endl;
	}

	//////////////////
	// 13. clean up //
	//////////////////
	delete this->rtiamb;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Helper Methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
 * This method will get all the relevant handle information from the RTIambassador
 */
void VirtualBusFederate::initializeHandles()
{
	// this->aHandle[0]  = rtiamb->getObjectClassHandle( "VirtualBus.dut" );
    // this->aHandle[1]  = rtiamb->getObjectClassHandle( "VirtualBus.refmod" );

    // get all Object Class Handle. Ex: VirtualBus.dut, VirtualBus.refmod, VirtualBus.port...
    for(unsigned int i = 0; i < _objClassHandle_names.size(); i ++)
        _aHandle[i] = rtiamb->getObjectClassHandle(_objClassHandle_names[i].c_str());

    // For each Object Class
    for(unsigned int i = 0; i < _objClassHandle_names.size(); i++)
    {   
        // For each attribute of Object Class. Ex: data1, data2...
        for(unsigned int j = 0; j < _attributeHandle_names[i].size(); j++)
        {
            _attributeHandle[i][j] = rtiamb->getAttributeHandle( _attributeHandle_names[i][j], _aHandle[i] );
        }
    }

	// this->srcHandle = rtiamb->getAttributeHandle( "source", aHandle[0] );
	// this->addrHandle = rtiamb->getAttributeHandle( "address", aHandle[0] );
	// this->sizeHandle = rtiamb->getAttributeHandle( "size", aHandle[0] );
	// this->data1Handle = rtiamb->getAttributeHandle( "data1", aHandle[0] );
	// this->data2Handle = rtiamb->getAttributeHandle( "data2", aHandle[0] );
	// this->data3Handle = rtiamb->getAttributeHandle( "data3", aHandle[0] );
	// this->data4Handle = rtiamb->getAttributeHandle( "data4", aHandle[0] );
	// this->data5Handle = rtiamb->getAttributeHandle( "data5", aHandle[0] );
	// this->data6Handle = rtiamb->getAttributeHandle( "data6", aHandle[0] );
	// this->data7Handle = rtiamb->getAttributeHandle( "data7", aHandle[0] );
	// this->data8Handle = rtiamb->getAttributeHandle( "data8", aHandle[0] );
    // this->dataFrameHandle = rtiamb->getAttributeHandle( "dataFrame", aHandle[0] );

    // this->testHandle = rtiamb->getAttributeHandle( "refmodTest", aHandle[1] );

	
}

/*
 * Blocks until the user hits enter
 */
void VirtualBusFederate::waitForUser()
{
	cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << endl;
	string line;
	getline( cin, line );
}

/*
 * This method will attempt to enable the various time related properties for
 * the federate
 */
void VirtualBusFederate::enableTimePolicy()
{
	////////////////////////////
	// enable time regulation //
	////////////////////////////
	RTIfedTime federateTime = fedamb->federateTime;
	RTIfedTime lookahead = fedamb->federateLookahead;
	rtiamb->enableTimeRegulation( federateTime, lookahead );

	// tick until we get the callback
	while( fedamb->isRegulating == false )
	{
		rtiamb->tick();
	}

	/////////////////////////////
	// enable time constrained //
	/////////////////////////////
	rtiamb->enableTimeConstrained();

	// tick until we get the callback
	while( fedamb->isConstrained == false )
	{
		rtiamb->tick();
	}
}

/*
 * This method will inform the RTI about the types of data that the federate will
 * be creating, and the types of data we are interested in hearing about as other
 * federates produce it.
 */
void VirtualBusFederate::publishAndSubscribe()
{
	////////////////////////////////////////////
	// publish all attributes of ObjectRoot.robot //
	////////////////////////////////////////////
	// before we can register instance of the object class ObjectRoot.robot and
	// update the values of the various attributes, we need to tell the RTI
	// that we intend to publish this information

	// package the information into a handle set
	// RTI::AttributeHandleSet *attributes1 = RTI::AttributeHandleSetFactory::create( 12 );
	// RTI::AttributeHandleSet *attributes2 = RTI::AttributeHandleSetFactory::create( 2 );
    for(unsigned int i = 0; i < _objClassHandle_names.size(); i++)
    {
        RTI::AttributeHandleSet *attributes = RTI::AttributeHandleSetFactory::create( _attributeHandle_names[i].size()+1 );

        for(unsigned int j = 0; j < _attributeHandle_names[i].size(); j++)
        {
            attributes->add(_attributeHandle[i][j]);
        }

        rtiamb->publishObjectClass( _aHandle[i], *attributes );
        rtiamb->subscribeObjectClassAttributes( _aHandle[i], *attributes );

        delete attributes;
    }

    // RTI::AttributeHandleSet *attributes = RTI::AttributeHandleSetFactory::create( 12 );

	// attributes1->add( this->srcHandle );
	// attributes1->add( this->addrHandle );	
	// attributes1->add( this->sizeHandle );
	// attributes1->add( this->data1Handle );
	// attributes1->add( this->data2Handle );
	// attributes1->add( this->data3Handle );
	// attributes1->add( this->data4Handle );
	// attributes1->add( this->data5Handle );
	// attributes1->add( this->data6Handle );
	// attributes1->add( this->data7Handle );
	// attributes1->add( this->data8Handle );    
    // attributes1->add( this->dataFrameHandle );

    // attributes2->add( this->testHandle );


	
	
	// do the actual publication
	// rtiamb->publishObjectClass( this->aHandle[0], *attributes1 );
    // rtiamb->publishObjectClass( this->aHandle[1], *attributes2 );
	//cout << " >>>>>>>>>> Atributos publicados <<<<<<<<<<" << endl;
	


	/////////////////////////////////////////////////
	// subscribe to all attributes of ObjectRoot.Robot //
	/////////////////////////////////////////////////
	// we also want to hear about the same sort of information as it is
	// created and altered in other federates, so we need to subscribe to it
	// rtiamb->subscribeObjectClassAttributes( this->aHandle[0], *attributes1 );
    // rtiamb->subscribeObjectClassAttributes( this->aHandle[1], *attributes2 );
	//cout << " >>>>>>>>>> Atributos subinscritos <<<<<<<<<<" << endl;
	

	/////////////////////////////////////////////////////
	// publish the interaction class InteractionRoot.X //
	/////////////////////////////////////////////////////
	// we want to send interactions of type InteractionRoot.X, so we need
	// to tell the RTI that we're publishing it first. We don't need to
	// inform it of the parameters, only the class, making it much simpler

	// do the publication
	//rtiamb->publishInteractionClass( this->xHandle );

	////////////////////////////////////////////////////
	// subscribe to the InteractionRoot.X interaction //
	////////////////////////////////////////////////////
	// we also want to receive other interaction of the same type that are
	// sent out by other federates, so we have to subscribe to it first
	//rtiamb->subscribeInteractionClass( this->xHandle );

	// clean up
	// delete attributes1;
    // delete attributes2;
}

/*
 * This method will register an instance of the class ObjectRoot.A and will
 * return the federation-wide unique handle for that instance. Later in the
 * simulation, we will update the attribute values for this instance
 */
RTI::ObjectHandle VirtualBusFederate::registerObject()
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle("VirtualBus.refmod") );
}

RTI::ObjectHandle VirtualBusFederate::registerObject(std::string objClass_name)
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle(objClass_name.c_str()) );
}

/*
 * This method will update all the values of the given object instance. It will
 * set each of the values to be a string which is equal to the name of the
 * attribute plus the current time. eg "aa:10.0" if the time is 10.0.
 * <p/>
 * Note that we don't actually have to update all the attributes at once, we
 * could update them individually, in groups or not at all!
 */
void VirtualBusFederate::updateAttributeValues(string src, string addr, string size, string data1, string data2, string data3, 
											   string data4, string data5, string data6, string data7, string data8, unsigned char* dataFrame)
{
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in, as you can see
	// this is quite a lot of work
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create( 12 );

	// generate the new values
	// we use EncodingHelpers to make things nice friendly for both Java and C++

	//cout << "**** Adicionando atributos ****" << endl;
 
	attributes->add( srcHandle, src.c_str(), (RTI::ULong)(src.size()+1));
	attributes->add( addrHandle, addr.c_str(), (RTI::ULong)(addr.size()+1));
	attributes->add( sizeHandle, size.c_str(), (RTI::ULong)(size.size()+1));
	attributes->add( data1Handle, data1.c_str(), (RTI::ULong)(data1.size()+1));
	attributes->add( data2Handle, data2.c_str(), (RTI::ULong)(data2.size()+1));
	attributes->add( data3Handle, data3.c_str(), (RTI::ULong)(data3.size()+1));
	attributes->add( data4Handle, data4.c_str(), (RTI::ULong)(data4.size()+1));
	attributes->add( data5Handle, data5.c_str(), (RTI::ULong)(data5.size()+1));
	attributes->add( data6Handle, data6.c_str(), (RTI::ULong)(data6.size()+1));
	attributes->add( data7Handle, data7.c_str(), (RTI::ULong)(data7.size()+1));
	attributes->add( data8Handle, data8.c_str(), (RTI::ULong)(data8.size()+1));
	attributes->add( dataFrameHandle, (char*)dataFrame, (RTI::ULong)(sizeof(unsigned char)) * NUM_ELEMENTS);


	
	//cout << "**** Atributos adicionados ****" << endl;
	
	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
	RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	rtiamb->updateAttributeValues(oHandle[0], *attributes, time, federateName );
	


	// clean up
	delete attributes;
}




/*
 * This method will send out an interaction of the type InteractionRoot.X. Any
 * federates which are subscribed to it will receive a notification the next time
 * they tick(). Here we are passing only two of the three parameters we could be
 * passing, but we don't actually have to pass any at all!
 */
void VirtualBusFederate::sendInteraction()
{
	/*	
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in
	RTI::ParameterHandleValuePairSet *parameters = RTI::ParameterSetFactory::create( 2 );

	// generate the new values
	char xaValue[16], xbValue[16];
	sprintf( xaValue, "xa:%f", getLbts() );
	sprintf( xbValue, "xb:%f", getLbts() );
	parameters->add( xaHandle, xaValue, (RTI::ULong)strlen(xaValue)+1 );
	parameters->add( xbHandle, xbValue, (RTI::ULong)strlen(xbValue)+1 );

	//////////////////////////
	// send the interaction //
	//////////////////////////
	rtiamb->sendInteraction( xHandle, *parameters, "hi!" );

	// if you want to associate a particular timestamp with the
	// interaction, you will have to supply it to the RTI. Here
	// we send another interaction, this time with a timestamp:
	RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	rtiamb->sendInteraction( xHandle, *parameters, time, "hi!" );

	// clean up
	delete parameters;
	*/
}

/*
 * This method will request a time advance to the current time, plus the given
 * timestep. It will then wait until a notification of the time advance grant
 * has been received.
 */
void VirtualBusFederate::advanceTime( double timestep )
{
	
	// request the advance
	fedamb->isAdvancing = true;
	RTIfedTime newTime = (fedamb->federateTime + timestep);
	rtiamb->timeAdvanceRequest( newTime );

	// wait for the time advance to be granted. ticking will tell the
	// LRC to start delivering callbacks to the federate
	while( fedamb->isAdvancing )
	{
		rtiamb->tick();
	}
}




/*
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void VirtualBusFederate::deleteObject( RTI::ObjectHandle* objectHandle )
{
	rtiamb->deleteObjectInstance( objectHandle[0], NULL );
    rtiamb->deleteObjectInstance( objectHandle[1], NULL );
}

double VirtualBusFederate::getLbts()
{
	return (fedamb->federateTime + fedamb->federateLookahead);
}

/*void VirtualBusFederate::writeFrame(unsigned)
{

	data[0] = framesRect[0].cols-1;
	data[1] = framesRect[0].rows-1;

	for (int x = image.cols-1; x >= 0; x--)
	{
	
		for (int y = image.rows-1; y >= 0; y--)
		{

			data[]

		}

	}

}*/

/*void VirtualBusFederate::writeFrame(unsigned (data&)[38])
{



}*/

