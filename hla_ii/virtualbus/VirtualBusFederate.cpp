#include <iostream>
#include "RTI.hh"
#include "fedtime.hh"

#include "FederateAmbassador.h"
#include "VirtualBusFederate.h"
#include <string>
#include <unistd.h>



using namespace std;

VirtualBusFederate::VirtualBusFederate()
{
}

VirtualBusFederate::~VirtualBusFederate()
{
	if( this->fedamb )
		delete this->fedamb;
}


///////////////////////////////////////////////////////////////////////////
////////////////////////// Meus Metodos /////////////////////////
///////////////////////////////////////////////////////////////////////////

void VirtualBusFederate::writeData(unsigned src, unsigned addr, unsigned size, unsigned *data){
	
	// cout << "Enviando: x, y - size _ d1, d2, d3" << endl;
	// printf("%d, %d - %d _ %d, %d, %d\n", src, addr, size, data[0], data[1], data[2]);
	// cout << "2.1: " << endl;
	//advanceTime(fedamb->federateLookahead);

	//Força o envio do source (src) como unsigned de valor 0, identificando assim que essa mensagem é do sender.
	// src = FPGA_ID;
	updateAttributeValues(src, addr, size, data);
}


bool VirtualBusFederate::readData(unsigned& source, unsigned& addr, unsigned& size, unsigned *data){
	if(fedamb->hasReceivedData()){
		fedamb->getReceivedData(source, addr, size, data);
		return true;
	}
	else
		return false;
}

bool VirtualBusFederate::hasReceivedData(){
	return receivedData;
}


void VirtualBusFederate::loop (int iteracoes){


	unsigned address = 100;
	unsigned size=4;
	unsigned data[16];
	data[0]=1;
	data[1]=2;
	data[2]=3;
	data[3]=4; 

	for(int i = 0; i < iteracoes; i++ ){
		
		cout << "Sending data... "  << endl;
	
		writeData((unsigned)federateName[0], address, size, data);
		
		
		cout << "Data sent... "  << endl;
				
		unsigned source, add, sz, dt;
		if(readData(source, add, sz, data)){
			// cout << ">>>>>>>>>>>>>>>>>>Data received<<<<<<<<<<<<<<<<<" << endl;			
			// cout << "Address: " << add << endl;
			// cout << "From: " << source << " at: " << fedamb->federateTime << endl;
			// cout << "Size: " << sz << endl;
			// cout << "Data: " << dt << endl;
		}
		
		advanceTime(1.0);
		//sleep (5);
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
    if(strcmp( federateName, "2" ) == 0)
    {
	    rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, "" );
    }
    else
    {
        // rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, "" );
    }
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
	oHandle = registerObject();
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
	this->aHandle  = rtiamb->getObjectClassHandle( "VirtualBus.port" );
	this->srcHandle = rtiamb->getAttributeHandle( "source", aHandle );
	this->addrHandle = rtiamb->getAttributeHandle( "address", aHandle );
	this->sizeHandle = rtiamb->getAttributeHandle( "size", aHandle );
	this->data0Handle = rtiamb->getAttributeHandle( "data0", aHandle );
	this->data1Handle = rtiamb->getAttributeHandle( "data1", aHandle );
	this->data2Handle = rtiamb->getAttributeHandle( "data2", aHandle );
	this->data3Handle = rtiamb->getAttributeHandle( "data3", aHandle );
	this->data4Handle = rtiamb->getAttributeHandle( "data4", aHandle );
	this->data5Handle = rtiamb->getAttributeHandle( "data5", aHandle );
	this->data6Handle = rtiamb->getAttributeHandle( "data6", aHandle );
	this->data7Handle = rtiamb->getAttributeHandle( "data7", aHandle );
	this->data8Handle = rtiamb->getAttributeHandle( "data8", aHandle );
	this->data9Handle = rtiamb->getAttributeHandle( "data9", aHandle );
	this->data10Handle = rtiamb->getAttributeHandle( "data10", aHandle );
	this->data11Handle = rtiamb->getAttributeHandle( "data11", aHandle );
	this->data12Handle = rtiamb->getAttributeHandle( "data12", aHandle );
	this->data13Handle = rtiamb->getAttributeHandle( "data13", aHandle );
	this->data14Handle = rtiamb->getAttributeHandle( "data14", aHandle );
	this->data15Handle = rtiamb->getAttributeHandle( "data15", aHandle );
	
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
	RTI::AttributeHandleSet *attributes = RTI::AttributeHandleSetFactory::create( 19 );
	
	attributes->add( this->srcHandle );
	attributes->add( this->addrHandle );	
	attributes->add( this->sizeHandle );
	attributes->add( this->data0Handle );
	attributes->add( this->data1Handle );
	attributes->add( this->data2Handle );
	attributes->add( this->data3Handle );
	attributes->add( this->data4Handle );
	attributes->add( this->data5Handle );
	attributes->add( this->data6Handle );
	attributes->add( this->data7Handle );
	attributes->add( this->data8Handle );
	attributes->add( this->data9Handle );
	attributes->add( this->data10Handle );
	attributes->add( this->data11Handle );
	attributes->add( this->data12Handle );
	attributes->add( this->data13Handle );
	attributes->add( this->data14Handle );
	attributes->add( this->data15Handle );
	
	
	// do the actual publication
	rtiamb->publishObjectClass( this->aHandle, *attributes );
	//cout << " >>>>>>>>>> Atributos publicados <<<<<<<<<<" << endl;
	


	/////////////////////////////////////////////////
	// subscribe to all attributes of ObjectRoot.Robot //
	/////////////////////////////////////////////////
	// we also want to hear about the same sort of information as it is
	// created and altered in other federates, so we need to subscribe to it
	rtiamb->subscribeObjectClassAttributes( this->aHandle, *attributes );
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
	delete attributes;
}

/*
 * This method will register an instance of the class ObjectRoot.A and will
 * return the federation-wide unique handle for that instance. Later in the
 * simulation, we will update the attribute values for this instance
 */
RTI::ObjectHandle VirtualBusFederate::registerObject()
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle("VirtualBus.port") );
}

/*
 * This method will update all the values of the given object instance. It will
 * set each of the values to be a string which is equal to the name of the
 * attribute plus the current time. eg "aa:10.0" if the time is 10.0.
 * <p/>
 * Note that we don't actually have to update all the attributes at once, we
 * could update them individually, in groups or not at all!
 */
void VirtualBusFederate::updateAttributeValues(unsigned src, unsigned addr, unsigned size, unsigned *data)
{
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in, as you can see
	// this is quite a lot of work
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create( 19 );

	// generate the new values
	// we use EncodingHelpers to make things nice friendly for both Java and C++

	// cout << "**** Adicionando atributos ****" << endl;

	attributes->add( srcHandle, (char*)&src, (RTI::ULong)sizeof(unsigned));
	attributes->add( addrHandle, (char*)&addr, (RTI::ULong)sizeof(unsigned));
	attributes->add( sizeHandle, (char*)&size, (RTI::ULong)sizeof(unsigned));
	attributes->add( data0Handle, (char*)&data[0], (RTI::ULong)sizeof(unsigned));
	attributes->add( data1Handle, (char*)&data[1], (RTI::ULong)sizeof(unsigned));
	attributes->add( data2Handle, (char*)&data[2], (RTI::ULong)sizeof(unsigned));
	attributes->add( data3Handle, (char*)&data[3], (RTI::ULong)sizeof(unsigned));
	attributes->add( data4Handle, (char*)&data[4], (RTI::ULong)sizeof(unsigned));
	attributes->add( data5Handle, (char*)&data[5], (RTI::ULong)sizeof(unsigned));
	attributes->add( data6Handle, (char*)&data[6], (RTI::ULong)sizeof(unsigned));
	attributes->add( data7Handle, (char*)&data[7], (RTI::ULong)sizeof(unsigned));
	attributes->add( data8Handle, (char*)&data[8], (RTI::ULong)sizeof(unsigned));
	attributes->add( data9Handle, (char*)&data[9], (RTI::ULong)sizeof(unsigned));
	attributes->add( data10Handle, (char*)&data[10], (RTI::ULong)sizeof(unsigned));
	attributes->add( data11Handle, (char*)&data[11], (RTI::ULong)sizeof(unsigned));
	attributes->add( data12Handle, (char*)&data[12], (RTI::ULong)sizeof(unsigned));
	attributes->add( data13Handle, (char*)&data[13], (RTI::ULong)sizeof(unsigned));
	attributes->add( data14Handle, (char*)&data[14], (RTI::ULong)sizeof(unsigned));
	attributes->add( data15Handle, (char*)&data[15], (RTI::ULong)sizeof(unsigned));
	
	// cout << "3: " << endl;
	
	
	// cout << "**** Atributos atualizados ****" << endl;
	
	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
	RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	rtiamb->updateAttributeValues(oHandle, *attributes, time, federateName );
	
	// cout << "4: " << endl;

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
void VirtualBusFederate::deleteObject( RTI::ObjectHandle objectHandle )
{
	rtiamb->deleteObjectInstance( objectHandle, NULL );
}

double VirtualBusFederate::getLbts()
{
	return (fedamb->federateTime + fedamb->federateLookahead);
}
