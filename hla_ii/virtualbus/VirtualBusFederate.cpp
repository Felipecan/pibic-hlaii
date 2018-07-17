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
	if(fedamb->hasReceivedData_from_src(source)){
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
	
		//writeData((unsigned)federateName[0], address, size, data);
		
		
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
    if(strcmp(federateName, "Env") == 0)
    {
        
	    
        // oHandle_dut = registerObject("VirtualBus.dut");
        // cout << "Registered Object, handle_dut=" << oHandle_dut << endl;
        
        // oHandle_ref = registerObject("VirtualBus.refmod");
        // cout << "Registered Object, handle_ref=" << oHandle_ref << endl;

        oHandle_all = registerObject("VirtualBus.all");
	    cout << "Registered Object, handle_all=" << oHandle_all << endl;
    }
    
    if(strcmp(federateName, "1") == 0)
    {
        //oHandle_all = registerObject("VirtualBus.all");
        oHandle_dut = registerObject("VirtualBus.dut");
        //cout << "Registered Object, handle=" << oHandle_all << endl;
        cout << "Registered Object, handle=" << oHandle_dut << endl;        
    }

    if(strcmp(federateName, "2") == 0)
    {
        //oHandle_all = registerObject("VirtualBus.all");
        oHandle_ref = registerObject("VirtualBus.refmod");
        //cout << "Registered Object, handle=" << oHandle_all << endl;
        cout << "Registered Object, handle=" << oHandle_ref << endl;
    } 
	
}


void VirtualBusFederate::finalize(){

	//////////////////////////////////////
	// 10. delete the object we created //
	//////////////////////////////////////
	deleteObject(oHandle_dut);
    deleteObject(oHandle_ref);
    deleteObject(oHandle_all);
	cout << "Deleted Object, handle=" << oHandle_dut << endl;
    cout << "Deleted Object, handle=" << oHandle_ref << endl;
    cout << "Deleted Object, handle=" << oHandle_all << endl;

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
    if(strcmp(federateName, "Env") == 0)
    {
        this->aHandle_all      = rtiamb->getObjectClassHandle( "VirtualBus.all" );
        this->srcHandle_all    = rtiamb->getAttributeHandle( "source", aHandle_all );
        this->addrHandle_all   = rtiamb->getAttributeHandle( "address", aHandle_all );
        this->sizeHandle_all   = rtiamb->getAttributeHandle( "size", aHandle_all );
        this->data0Handle_all  = rtiamb->getAttributeHandle( "data0", aHandle_all );
        this->data1Handle_all  = rtiamb->getAttributeHandle( "data1", aHandle_all );
        this->data2Handle_all  = rtiamb->getAttributeHandle( "data2", aHandle_all );
        this->data3Handle_all  = rtiamb->getAttributeHandle( "data3", aHandle_all );
        this->data4Handle_all  = rtiamb->getAttributeHandle( "data4", aHandle_all );
        this->data5Handle_all  = rtiamb->getAttributeHandle( "data5", aHandle_all );
        this->data6Handle_all  = rtiamb->getAttributeHandle( "data6", aHandle_all );
        this->data7Handle_all  = rtiamb->getAttributeHandle( "data7", aHandle_all );
        this->data8Handle_all  = rtiamb->getAttributeHandle( "data8", aHandle_all );
        this->data9Handle_all  = rtiamb->getAttributeHandle( "data9", aHandle_all );
        this->data10Handle_all = rtiamb->getAttributeHandle( "data10", aHandle_all );
        this->data11Handle_all = rtiamb->getAttributeHandle( "data11", aHandle_all );
        this->data12Handle_all = rtiamb->getAttributeHandle( "data12", aHandle_all );
        this->data13Handle_all = rtiamb->getAttributeHandle( "data13", aHandle_all );
        this->data14Handle_all = rtiamb->getAttributeHandle( "data14", aHandle_all );
        this->data15Handle_all = rtiamb->getAttributeHandle( "data15", aHandle_all );

	    this->aHandle_dut      = rtiamb->getObjectClassHandle( "VirtualBus.dut" );
        this->srcHandle_dut    = rtiamb->getAttributeHandle( "source", aHandle_dut );
        this->addrHandle_dut   = rtiamb->getAttributeHandle( "address", aHandle_dut );
        this->sizeHandle_dut   = rtiamb->getAttributeHandle( "size", aHandle_dut );
        this->data0Handle_dut  = rtiamb->getAttributeHandle( "data0", aHandle_dut );
        this->data1Handle_dut  = rtiamb->getAttributeHandle( "data1", aHandle_dut );
        this->data2Handle_dut  = rtiamb->getAttributeHandle( "data2", aHandle_dut );
        this->data3Handle_dut  = rtiamb->getAttributeHandle( "data3", aHandle_dut );
        this->data4Handle_dut  = rtiamb->getAttributeHandle( "data4", aHandle_dut );
        this->data5Handle_dut  = rtiamb->getAttributeHandle( "data5", aHandle_dut );
        this->data6Handle_dut  = rtiamb->getAttributeHandle( "data6", aHandle_dut );
        this->data7Handle_dut  = rtiamb->getAttributeHandle( "data7", aHandle_dut );
        this->data8Handle_dut  = rtiamb->getAttributeHandle( "data8", aHandle_dut );
        this->data9Handle_dut  = rtiamb->getAttributeHandle( "data9", aHandle_dut );
        this->data10Handle_dut = rtiamb->getAttributeHandle( "data10", aHandle_dut );
        this->data11Handle_dut = rtiamb->getAttributeHandle( "data11", aHandle_dut );
        this->data12Handle_dut = rtiamb->getAttributeHandle( "data12", aHandle_dut );
        this->data13Handle_dut = rtiamb->getAttributeHandle( "data13", aHandle_dut );
        this->data14Handle_dut = rtiamb->getAttributeHandle( "data14", aHandle_dut );
        this->data15Handle_dut = rtiamb->getAttributeHandle( "data15", aHandle_dut ); 

        this->aHandle_ref      = rtiamb->getObjectClassHandle( "VirtualBus.refmod" );
        this->srcHandle_ref    = rtiamb->getAttributeHandle( "source", aHandle_ref  );
        this->addrHandle_ref   = rtiamb->getAttributeHandle( "address", aHandle_ref  );
        this->sizeHandle_ref   = rtiamb->getAttributeHandle( "size", aHandle_ref  );
        this->data0Handle_ref  = rtiamb->getAttributeHandle( "data0", aHandle_ref  );
        this->data1Handle_ref  = rtiamb->getAttributeHandle( "data1", aHandle_ref  );
        this->data2Handle_ref  = rtiamb->getAttributeHandle( "data2", aHandle_ref  );
        this->data3Handle_ref  = rtiamb->getAttributeHandle( "data3", aHandle_ref  );
        this->data4Handle_ref  = rtiamb->getAttributeHandle( "data4", aHandle_ref  );
        this->data5Handle_ref  = rtiamb->getAttributeHandle( "data5", aHandle_ref  );
        this->data6Handle_ref  = rtiamb->getAttributeHandle( "data6", aHandle_ref  );
        this->data7Handle_ref  = rtiamb->getAttributeHandle( "data7", aHandle_ref  );
        this->data8Handle_ref  = rtiamb->getAttributeHandle( "data8", aHandle_ref  );
        this->data9Handle_ref  = rtiamb->getAttributeHandle( "data9", aHandle_ref  );
        this->data10Handle_ref = rtiamb->getAttributeHandle( "data10", aHandle_ref  );
        this->data11Handle_ref = rtiamb->getAttributeHandle( "data11", aHandle_ref  );
        this->data12Handle_ref = rtiamb->getAttributeHandle( "data12", aHandle_ref  );
        this->data13Handle_ref = rtiamb->getAttributeHandle( "data13", aHandle_ref  );
        this->data14Handle_ref = rtiamb->getAttributeHandle( "data14", aHandle_ref  );
        this->data15Handle_ref = rtiamb->getAttributeHandle( "data15", aHandle_ref  ); 
    }
     
    if(strcmp(federateName, "1") == 0)
    {
        this->aHandle_all      = rtiamb->getObjectClassHandle( "VirtualBus.all" );
        this->srcHandle_all    = rtiamb->getAttributeHandle( "source", aHandle_all );
        this->addrHandle_all   = rtiamb->getAttributeHandle( "address", aHandle_all );
        this->sizeHandle_all   = rtiamb->getAttributeHandle( "size", aHandle_all );
        this->data0Handle_all  = rtiamb->getAttributeHandle( "data0", aHandle_all );
        this->data1Handle_all  = rtiamb->getAttributeHandle( "data1", aHandle_all );
        this->data2Handle_all  = rtiamb->getAttributeHandle( "data2", aHandle_all );
        this->data3Handle_all  = rtiamb->getAttributeHandle( "data3", aHandle_all );
        this->data4Handle_all  = rtiamb->getAttributeHandle( "data4", aHandle_all );
        this->data5Handle_all  = rtiamb->getAttributeHandle( "data5", aHandle_all );
        this->data6Handle_all  = rtiamb->getAttributeHandle( "data6", aHandle_all );
        this->data7Handle_all  = rtiamb->getAttributeHandle( "data7", aHandle_all );
        this->data8Handle_all  = rtiamb->getAttributeHandle( "data8", aHandle_all );
        this->data9Handle_all  = rtiamb->getAttributeHandle( "data9", aHandle_all );
        this->data10Handle_all = rtiamb->getAttributeHandle( "data10", aHandle_all );
        this->data11Handle_all = rtiamb->getAttributeHandle( "data11", aHandle_all );
        this->data12Handle_all = rtiamb->getAttributeHandle( "data12", aHandle_all );
        this->data13Handle_all = rtiamb->getAttributeHandle( "data13", aHandle_all );
        this->data14Handle_all = rtiamb->getAttributeHandle( "data14", aHandle_all );
        this->data15Handle_all = rtiamb->getAttributeHandle( "data15", aHandle_all );
    
        this->aHandle_dut      = rtiamb->getObjectClassHandle( "VirtualBus.dut" );
        this->srcHandle_dut    = rtiamb->getAttributeHandle( "source", aHandle_dut );
        this->addrHandle_dut   = rtiamb->getAttributeHandle( "address", aHandle_dut );
        this->sizeHandle_dut   = rtiamb->getAttributeHandle( "size", aHandle_dut );
        this->data0Handle_dut  = rtiamb->getAttributeHandle( "data0", aHandle_dut );
        this->data1Handle_dut  = rtiamb->getAttributeHandle( "data1", aHandle_dut );
        this->data2Handle_dut  = rtiamb->getAttributeHandle( "data2", aHandle_dut );
        this->data3Handle_dut  = rtiamb->getAttributeHandle( "data3", aHandle_dut );
        this->data4Handle_dut  = rtiamb->getAttributeHandle( "data4", aHandle_dut );
        this->data5Handle_dut  = rtiamb->getAttributeHandle( "data5", aHandle_dut );
        this->data6Handle_dut  = rtiamb->getAttributeHandle( "data6", aHandle_dut );
        this->data7Handle_dut  = rtiamb->getAttributeHandle( "data7", aHandle_dut );
        this->data8Handle_dut  = rtiamb->getAttributeHandle( "data8", aHandle_dut );
        this->data9Handle_dut  = rtiamb->getAttributeHandle( "data9", aHandle_dut );
        this->data10Handle_dut = rtiamb->getAttributeHandle( "data10", aHandle_dut );
        this->data11Handle_dut = rtiamb->getAttributeHandle( "data11", aHandle_dut );
        this->data12Handle_dut = rtiamb->getAttributeHandle( "data12", aHandle_dut );
        this->data13Handle_dut = rtiamb->getAttributeHandle( "data13", aHandle_dut );
        this->data14Handle_dut = rtiamb->getAttributeHandle( "data14", aHandle_dut );
        this->data15Handle_dut = rtiamb->getAttributeHandle( "data15", aHandle_dut );        
    }

    if(strcmp(federateName, "2") == 0)
    {
        this->aHandle_all      = rtiamb->getObjectClassHandle( "VirtualBus.all" );
        this->srcHandle_all    = rtiamb->getAttributeHandle( "source", aHandle_all );
        this->addrHandle_all   = rtiamb->getAttributeHandle( "address", aHandle_all );
        this->sizeHandle_all   = rtiamb->getAttributeHandle( "size", aHandle_all );
        this->data0Handle_all  = rtiamb->getAttributeHandle( "data0", aHandle_all );
        this->data1Handle_all  = rtiamb->getAttributeHandle( "data1", aHandle_all );
        this->data2Handle_all  = rtiamb->getAttributeHandle( "data2", aHandle_all );
        this->data3Handle_all  = rtiamb->getAttributeHandle( "data3", aHandle_all );
        this->data4Handle_all  = rtiamb->getAttributeHandle( "data4", aHandle_all );
        this->data5Handle_all  = rtiamb->getAttributeHandle( "data5", aHandle_all );
        this->data6Handle_all  = rtiamb->getAttributeHandle( "data6", aHandle_all );
        this->data7Handle_all  = rtiamb->getAttributeHandle( "data7", aHandle_all );
        this->data8Handle_all  = rtiamb->getAttributeHandle( "data8", aHandle_all );
        this->data9Handle_all  = rtiamb->getAttributeHandle( "data9", aHandle_all );
        this->data10Handle_all = rtiamb->getAttributeHandle( "data10", aHandle_all );
        this->data11Handle_all = rtiamb->getAttributeHandle( "data11", aHandle_all );
        this->data12Handle_all = rtiamb->getAttributeHandle( "data12", aHandle_all );
        this->data13Handle_all = rtiamb->getAttributeHandle( "data13", aHandle_all );
        this->data14Handle_all = rtiamb->getAttributeHandle( "data14", aHandle_all );
        this->data15Handle_all = rtiamb->getAttributeHandle( "data15", aHandle_all );

        this->aHandle_ref      = rtiamb->getObjectClassHandle( "VirtualBus.refmod" );
        this->srcHandle_ref    = rtiamb->getAttributeHandle( "source", aHandle_ref  );
        this->addrHandle_ref   = rtiamb->getAttributeHandle( "address", aHandle_ref  );
        this->sizeHandle_ref   = rtiamb->getAttributeHandle( "size", aHandle_ref  );
        this->data0Handle_ref  = rtiamb->getAttributeHandle( "data0", aHandle_ref  );
        this->data1Handle_ref  = rtiamb->getAttributeHandle( "data1", aHandle_ref  );
        this->data2Handle_ref  = rtiamb->getAttributeHandle( "data2", aHandle_ref  );
        this->data3Handle_ref  = rtiamb->getAttributeHandle( "data3", aHandle_ref  );
        this->data4Handle_ref  = rtiamb->getAttributeHandle( "data4", aHandle_ref  );
        this->data5Handle_ref  = rtiamb->getAttributeHandle( "data5", aHandle_ref  );
        this->data6Handle_ref  = rtiamb->getAttributeHandle( "data6", aHandle_ref  );
        this->data7Handle_ref  = rtiamb->getAttributeHandle( "data7", aHandle_ref  );
        this->data8Handle_ref  = rtiamb->getAttributeHandle( "data8", aHandle_ref  );
        this->data9Handle_ref  = rtiamb->getAttributeHandle( "data9", aHandle_ref  );
        this->data10Handle_ref = rtiamb->getAttributeHandle( "data10", aHandle_ref  );
        this->data11Handle_ref = rtiamb->getAttributeHandle( "data11", aHandle_ref  );
        this->data12Handle_ref = rtiamb->getAttributeHandle( "data12", aHandle_ref  );
        this->data13Handle_ref = rtiamb->getAttributeHandle( "data13", aHandle_ref  );
        this->data14Handle_ref = rtiamb->getAttributeHandle( "data14", aHandle_ref  );
        this->data15Handle_ref = rtiamb->getAttributeHandle( "data15", aHandle_ref  ); 
    } 

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
    cout << "Meu ID: " << federateName << endl;
    
    if(strcmp(federateName, "Env") == 0)
    {        
	    RTI::AttributeHandleSet *attributes1 = RTI::AttributeHandleSetFactory::create( 20 );
        RTI::AttributeHandleSet *attributes2 = RTI::AttributeHandleSetFactory::create( 20 );
        RTI::AttributeHandleSet *attributes3 = RTI::AttributeHandleSetFactory::create( 20 );

        attributes3->add( this->srcHandle_all );
        attributes3->add( this->addrHandle_all );	
        attributes3->add( this->sizeHandle_all );
        attributes3->add( this->data0Handle_all );
        attributes3->add( this->data1Handle_all );
        attributes3->add( this->data2Handle_all );
        attributes3->add( this->data3Handle_all );
        attributes3->add( this->data4Handle_all );
        attributes3->add( this->data5Handle_all );
        attributes3->add( this->data6Handle_all );
        attributes3->add( this->data7Handle_all );
        attributes3->add( this->data8Handle_all );
        attributes3->add( this->data9Handle_all );
        attributes3->add( this->data10Handle_all );
        attributes3->add( this->data11Handle_all );
        attributes3->add( this->data12Handle_all );
        attributes3->add( this->data13Handle_all );
        attributes3->add( this->data14Handle_all );
        attributes3->add( this->data15Handle_all );

        attributes1->add( this->srcHandle_dut );
        attributes1->add( this->addrHandle_dut );	
        attributes1->add( this->sizeHandle_dut );
        attributes1->add( this->data0Handle_dut );
        attributes1->add( this->data1Handle_dut );
        attributes1->add( this->data2Handle_dut );
        attributes1->add( this->data3Handle_dut );
        attributes1->add( this->data4Handle_dut );
        attributes1->add( this->data5Handle_dut );
        attributes1->add( this->data6Handle_dut );
        attributes1->add( this->data7Handle_dut );
        attributes1->add( this->data8Handle_dut );
        attributes1->add( this->data9Handle_dut );
        attributes1->add( this->data10Handle_dut );
        attributes1->add( this->data11Handle_dut );
        attributes1->add( this->data12Handle_dut );
        attributes1->add( this->data13Handle_dut );
        attributes1->add( this->data14Handle_dut );
        attributes1->add( this->data15Handle_dut );

        attributes2->add( this->srcHandle_ref );
        attributes2->add( this->addrHandle_ref );	
        attributes2->add( this->sizeHandle_ref );
        attributes2->add( this->data0Handle_ref );
        attributes2->add( this->data1Handle_ref );
        attributes2->add( this->data2Handle_ref );
        attributes2->add( this->data3Handle_ref );
        attributes2->add( this->data4Handle_ref );
        attributes2->add( this->data5Handle_ref );
        attributes2->add( this->data6Handle_ref );
        attributes2->add( this->data7Handle_ref );
        attributes2->add( this->data8Handle_ref );
        attributes2->add( this->data9Handle_ref );
        attributes2->add( this->data10Handle_ref );
        attributes2->add( this->data11Handle_ref );
        attributes2->add( this->data12Handle_ref );
        attributes2->add( this->data13Handle_ref );
        attributes2->add( this->data14Handle_ref );
        attributes2->add( this->data15Handle_ref );

        //rtiamb->publishObjectClass( this->aHandle_dut, *attributes1 );
        std::cout << federateName << " Subscribe dut" << std::endl;
        rtiamb->subscribeObjectClassAttributes( this->aHandle_dut, *attributes1 );
        
        std::cout << federateName << " Subscribe refmod" << std::endl;
        //rtiamb->publishObjectClass( this->aHandle_ref, *attributes2 );
        rtiamb->subscribeObjectClassAttributes( this->aHandle_ref, *attributes2 );
        
        std::cout << federateName << " publish all" << std::endl;
        rtiamb->publishObjectClass( this->aHandle_all, *attributes3 );        
        //rtiamb->subscribeObjectClassAttributes( this->aHandle_all, *attributes3 );
        
        delete attributes1;
        delete attributes2;
        delete attributes3;
    }

    if(strcmp(federateName, "1") == 0)
    {           
        RTI::AttributeHandleSet *attributes1 = RTI::AttributeHandleSetFactory::create( 20 );            
        RTI::AttributeHandleSet *attributes3 = RTI::AttributeHandleSetFactory::create( 20 );

        attributes3->add( this->srcHandle_all );
        attributes3->add( this->addrHandle_all );	
        attributes3->add( this->sizeHandle_all );
        attributes3->add( this->data0Handle_all );
        attributes3->add( this->data1Handle_all );
        attributes3->add( this->data2Handle_all );
        attributes3->add( this->data3Handle_all );
        attributes3->add( this->data4Handle_all );
        attributes3->add( this->data5Handle_all );
        attributes3->add( this->data6Handle_all );
        attributes3->add( this->data7Handle_all );
        attributes3->add( this->data8Handle_all );
        attributes3->add( this->data9Handle_all );
        attributes3->add( this->data10Handle_all );
        attributes3->add( this->data11Handle_all );
        attributes3->add( this->data12Handle_all );
        attributes3->add( this->data13Handle_all );
        attributes3->add( this->data14Handle_all );
        attributes3->add( this->data15Handle_all );

        attributes1->add( this->srcHandle_dut );
        attributes1->add( this->addrHandle_dut );	
        attributes1->add( this->sizeHandle_dut );
        attributes1->add( this->data0Handle_dut );
        attributes1->add( this->data1Handle_dut );
        attributes1->add( this->data2Handle_dut );
        attributes1->add( this->data3Handle_dut );
        attributes1->add( this->data4Handle_dut );
        attributes1->add( this->data5Handle_dut );
        attributes1->add( this->data6Handle_dut );
        attributes1->add( this->data7Handle_dut );
        attributes1->add( this->data8Handle_dut );
        attributes1->add( this->data9Handle_dut );
        attributes1->add( this->data10Handle_dut );
        attributes1->add( this->data11Handle_dut );
        attributes1->add( this->data12Handle_dut );
        attributes1->add( this->data13Handle_dut );
        attributes1->add( this->data14Handle_dut );
        attributes1->add( this->data15Handle_dut );  

        
        rtiamb->publishObjectClass( this->aHandle_dut, *attributes1 ); 
        std::cout << federateName << " publish dut" << std::endl;
        //rtiamb->subscribeObjectClassAttributes( this->aHandle_dut, *attributes1 );  

        
        //rtiamb->publishObjectClass( this->aHandle_all, *attributes3 ); 
        rtiamb->subscribeObjectClassAttributes( this->aHandle_all, *attributes3 ); 
        std::cout << federateName << " subscribe all" << std::endl;

        delete attributes1; 
        delete attributes3;        
                
    }

    if(strcmp(federateName, "2") == 0)
    {
        RTI::AttributeHandleSet *attributes2 = RTI::AttributeHandleSetFactory::create( 20 );
        RTI::AttributeHandleSet *attributes3 = RTI::AttributeHandleSetFactory::create( 20 );

        attributes3->add( this->srcHandle_all );
        attributes3->add( this->addrHandle_all );	
        attributes3->add( this->sizeHandle_all );
        attributes3->add( this->data0Handle_all );
        attributes3->add( this->data1Handle_all );
        attributes3->add( this->data2Handle_all );
        attributes3->add( this->data3Handle_all );
        attributes3->add( this->data4Handle_all );
        attributes3->add( this->data5Handle_all );
        attributes3->add( this->data6Handle_all );
        attributes3->add( this->data7Handle_all );
        attributes3->add( this->data8Handle_all );
        attributes3->add( this->data9Handle_all );
        attributes3->add( this->data10Handle_all );
        attributes3->add( this->data11Handle_all );
        attributes3->add( this->data12Handle_all );
        attributes3->add( this->data13Handle_all );
        attributes3->add( this->data14Handle_all );
        attributes3->add( this->data15Handle_all );

        attributes2->add( this->srcHandle_ref );
        attributes2->add( this->addrHandle_ref );	
        attributes2->add( this->sizeHandle_ref );
        attributes2->add( this->data0Handle_ref );
        attributes2->add( this->data1Handle_ref );
        attributes2->add( this->data2Handle_ref );
        attributes2->add( this->data3Handle_ref );
        attributes2->add( this->data4Handle_ref );
        attributes2->add( this->data5Handle_ref );
        attributes2->add( this->data6Handle_ref );
        attributes2->add( this->data7Handle_ref );
        attributes2->add( this->data8Handle_ref );
        attributes2->add( this->data9Handle_ref );
        attributes2->add( this->data10Handle_ref );
        attributes2->add( this->data11Handle_ref );
        attributes2->add( this->data12Handle_ref );
        attributes2->add( this->data13Handle_ref );
        attributes2->add( this->data14Handle_ref );
        attributes2->add( this->data15Handle_ref );

        std::cout << federateName << " publish ref" << std::endl;
        rtiamb->publishObjectClass( this->aHandle_ref, *attributes2 );
        //rtiamb->subscribeObjectClassAttributes( this->aHandle_ref, *attributes2 );

        std::cout << federateName << " subscribe all" << std::endl;
        //rtiamb->publishObjectClass( this->aHandle_all, *attributes3 );
        rtiamb->subscribeObjectClassAttributes( this->aHandle_all, *attributes3 );

        delete attributes2;
        delete attributes3;
    } 

	
	
	
    
	
	
	// do the actual publication
	// rtiamb->publishObjectClass( this->aHandle, *attributes );
	//cout << " >>>>>>>>>> Atributos publicados <<<<<<<<<<" << endl;
	


	/////////////////////////////////////////////////
	// subscribe to all attributes of ObjectRoot.Robot //
	/////////////////////////////////////////////////
	// we also want to hear about the same sort of information as it is
	// created and altered in other federates, so we need to subscribe to it
	
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
	//delete attributes;
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

RTI::ObjectHandle VirtualBusFederate::registerObject(std::string s)
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle(s.c_str()) );
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

    // cout << "upav Meu ID: " << federateName << endl;

    if(strcmp(federateName, "Env") == 0)
    {
	    RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create( 20 );
        // RTI::AttributeHandleValuePairSet *attributes2 = RTI::AttributeSetFactory::create( 20 );

        attributes->add( srcHandle_all, (char*)&src, (RTI::ULong)sizeof(unsigned));
        attributes->add( addrHandle_all, (char*)&addr, (RTI::ULong)sizeof(unsigned));
        attributes->add( sizeHandle_all, (char*)&size, (RTI::ULong)sizeof(unsigned));
        attributes->add( data0Handle_all, (char*)&data[0], (RTI::ULong)sizeof(unsigned));
        attributes->add( data1Handle_all, (char*)&data[1], (RTI::ULong)sizeof(unsigned));
        attributes->add( data2Handle_all, (char*)&data[2], (RTI::ULong)sizeof(unsigned));
        attributes->add( data3Handle_all, (char*)&data[3], (RTI::ULong)sizeof(unsigned));
        attributes->add( data4Handle_all, (char*)&data[4], (RTI::ULong)sizeof(unsigned));
        attributes->add( data5Handle_all, (char*)&data[5], (RTI::ULong)sizeof(unsigned));
        attributes->add( data6Handle_all, (char*)&data[6], (RTI::ULong)sizeof(unsigned));
        attributes->add( data7Handle_all, (char*)&data[7], (RTI::ULong)sizeof(unsigned));
        attributes->add( data8Handle_all, (char*)&data[8], (RTI::ULong)sizeof(unsigned));
        attributes->add( data9Handle_all, (char*)&data[9], (RTI::ULong)sizeof(unsigned));
        attributes->add( data10Handle_all, (char*)&data[10], (RTI::ULong)sizeof(unsigned));
        attributes->add( data11Handle_all, (char*)&data[11], (RTI::ULong)sizeof(unsigned));
        attributes->add( data12Handle_all, (char*)&data[12], (RTI::ULong)sizeof(unsigned));
        attributes->add( data13Handle_all, (char*)&data[13], (RTI::ULong)sizeof(unsigned));
        attributes->add( data14Handle_all, (char*)&data[14], (RTI::ULong)sizeof(unsigned));
        attributes->add( data15Handle_all, (char*)&data[15], (RTI::ULong)sizeof(unsigned));  

        // attributes2->add( srcHandle_ref, (char*)&src, (RTI::ULong)sizeof(unsigned));
        // attributes2->add( addrHandle_ref, (char*)&addr, (RTI::ULong)sizeof(unsigned));
        // attributes2->add( sizeHandle_ref, (char*)&size, (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data0Handle_ref, (char*)&data[0], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data1Handle_ref, (char*)&data[1], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data2Handle_ref, (char*)&data[2], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data3Handle_ref, (char*)&data[3], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data4Handle_ref, (char*)&data[4], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data5Handle_ref, (char*)&data[5], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data6Handle_ref, (char*)&data[6], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data7Handle_ref, (char*)&data[7], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data8Handle_ref, (char*)&data[8], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data9Handle_ref, (char*)&data[9], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data10Handle_ref, (char*)&data[10], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data11Handle_ref, (char*)&data[11], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data12Handle_ref, (char*)&data[12], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data13Handle_ref, (char*)&data[13], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data14Handle_ref, (char*)&data[14], (RTI::ULong)sizeof(unsigned));
        // attributes2->add( data15Handle_ref, (char*)&data[15], (RTI::ULong)sizeof(unsigned));  

        RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	    rtiamb->updateAttributeValues(oHandle_all, *attributes, time, federateName );
        //rtiamb->updateAttributeValues(oHandle_ref, *attributes2, time, federateName );
        
        delete attributes;
        //delete attributes2;
    }

    if(strcmp(federateName, "1") == 0)
    {           
        RTI::AttributeHandleValuePairSet *attributes1 = RTI::AttributeSetFactory::create( 20 );

        attributes1->add( srcHandle_dut, (char*)&src, (RTI::ULong)sizeof(unsigned));
        attributes1->add( addrHandle_dut, (char*)&addr, (RTI::ULong)sizeof(unsigned));
        attributes1->add( sizeHandle_dut, (char*)&size, (RTI::ULong)sizeof(unsigned));
        attributes1->add( data0Handle_dut, (char*)&data[0], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data1Handle_dut, (char*)&data[1], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data2Handle_dut, (char*)&data[2], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data3Handle_dut, (char*)&data[3], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data4Handle_dut, (char*)&data[4], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data5Handle_dut, (char*)&data[5], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data6Handle_dut, (char*)&data[6], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data7Handle_dut, (char*)&data[7], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data8Handle_dut, (char*)&data[8], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data9Handle_dut, (char*)&data[9], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data10Handle_dut, (char*)&data[10], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data11Handle_dut, (char*)&data[11], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data12Handle_dut, (char*)&data[12], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data13Handle_dut, (char*)&data[13], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data14Handle_dut, (char*)&data[14], (RTI::ULong)sizeof(unsigned));
        attributes1->add( data15Handle_dut, (char*)&data[15], (RTI::ULong)sizeof(unsigned)); 

        RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
        rtiamb->updateAttributeValues(oHandle_dut, *attributes1, time, federateName ); 

        delete attributes1;    
    }

    if(strcmp(federateName, "2") == 0)
    {
        RTI::AttributeHandleValuePairSet *attributes2 = RTI::AttributeSetFactory::create( 20 );

        attributes2->add( srcHandle_ref, (char*)&src, (RTI::ULong)sizeof(unsigned));
        attributes2->add( addrHandle_ref, (char*)&addr, (RTI::ULong)sizeof(unsigned));
        attributes2->add( sizeHandle_ref, (char*)&size, (RTI::ULong)sizeof(unsigned));
        attributes2->add( data0Handle_ref, (char*)&data[0], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data1Handle_ref, (char*)&data[1], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data2Handle_ref, (char*)&data[2], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data3Handle_ref, (char*)&data[3], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data4Handle_ref, (char*)&data[4], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data5Handle_ref, (char*)&data[5], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data6Handle_ref, (char*)&data[6], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data7Handle_ref, (char*)&data[7], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data8Handle_ref, (char*)&data[8], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data9Handle_ref, (char*)&data[9], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data10Handle_ref, (char*)&data[10], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data11Handle_ref, (char*)&data[11], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data12Handle_ref, (char*)&data[12], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data13Handle_ref, (char*)&data[13], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data14Handle_ref, (char*)&data[14], (RTI::ULong)sizeof(unsigned));
        attributes2->add( data15Handle_ref, (char*)&data[15], (RTI::ULong)sizeof(unsigned)); 

        RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
        rtiamb->updateAttributeValues(oHandle_ref, *attributes2, time, federateName ); 

        delete attributes2;
    } 
    

	

	// generate the new values
	// we use EncodingHelpers to make things nice friendly for both Java and C++

	// cout << "**** Adicionando atributos ****" << endl;
 
    
	
	// cout << "3: " << endl;
	
	
	// cout << "**** Atributos atualizados ****" << endl;
	
	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
		
	// cout << "4: " << endl;

	// clean up
	
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
