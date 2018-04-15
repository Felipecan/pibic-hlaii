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
//using namespace cv;

#include <vector>
#include <map>

VirtualBusFederate::VirtualBusFederate()
{
}

// VirtualBusFederate::VirtualBusFederate(vector<string> obj_class_names, multimap <string, string> c_and_attribute_names_publish, multimap <string, string> c_and_attribute_names_subscribe)
// {
//     multimap<string,string>::iterator it;

//     for(unsigned int i = 0; i < obj_class_names.size(); i++)    
//     {
//         it = c_and_attribute_names_publish.find(obj_class_names[i])
//         if(it != c_and_attribute_names_publish.end())
//         {
//             //valor encontrado
//         }
//         else
//         {
//             //valor não encontrado
//         }
//     }

//     this->obj_class_names = obj_class_names;
//     this->c_and_attribute_names_publish = c_and_attribute_names_publish;
//     this->c_and_attribute_names_subscribe = c_and_attribute_names_subscribe;

//     this->aHandles = new RTI::ObjectHandle[ obj_class_names.size() ];

    
// }


VirtualBusFederate::~VirtualBusFederate()
{
	if( this->fedamb )
		delete this->fedamb;
}


///////////////////////////////////////////////////////////////////////////
////////////////////////// Meus Metodos /////////////////////////
///////////////////////////////////////////////////////////////////////////

void VirtualBusFederate::writeData(string src, string addr, string size)
{
	updateAttributeValues(src, addr, size);
	//advanceTime(fedamb->federateLookahead);
}


bool VirtualBusFederate::readData(string& source, string& addr, string& size)
{
	if(fedamb->hasReceivedData())
	{
		fedamb->getReceivedData(source, addr, size);
        //cout << ">>>> x=" << source << " y=" << addr;
        //cout << endl << ">>> Receiving data: " << data[0] << " " << data[1] << " " << data[2] << endl;
        //cout << "OI" << endl;
		return true;
	}
	else
		return false;

}

bool VirtualBusFederate::hasReceivedData()
{
	return receivedData;
}


void VirtualBusFederate::loop (int iteracoes, char srcVideo[])
{
	//stringstream ss;
	int i;
    //ref = atoi(srcVideo);
	while(true)
	{   
        if(federateName[0] == '0')        
        {
            if(readData(source , address, size))
            {
                cout << "recebendo dados f0" << endl;
				cout << "source " << source << endl;
				cout << "address " << address << endl;
				cout << "size " << size << endl;				
				cout << "dados recebidos" << endl;
            }
            else
            {
                cout << "Nao chegou nada, aparentemente" << endl;
            }
            //std::cout << "Entrou na merda do if" << std::endl;
            source = "0000";
            address = "96";
            size = "99";            
            //ss.str(std::string());
            
            writeData(source, address, size);            
        }
        else
        {
            //std::cout << "Entrou na merda do else" << std::endl;
            if(readData(source, address, size))
		    {
                cout << "recebendo dados f!≃0" << endl;
				cout << "source " << source << endl;
				cout << "address " << address << endl;
				cout << "size " << size << endl;				
				cout << "dados recebidos" << endl;
            }
            else
            {
                cout << "Nao chegou nada, aparentemente" << endl;
            }
            //std::cout << "Entrou na merda do if" << std::endl;
            if(federateName[0] == '1')
            {
                source = "1111";
                address = "66";
                size = "99"; 
                writeData(source, address, size);              
            }
            else
            {
                source = "2222";
                address = "12";
                size = "78"; 
                writeData(source, address, size);      
            }
            
             
        }
        advanceTime(1.0);		
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
	// oHandle = registerObject();
	// cout << "Registered Object, handle=" << oHandle << endl;	

	if(federateName[0] == '0')
	{
        cout << "Registrando dois obj, fedName 0" << endl;
        string s1, s2;
        s1 = "VirtualBus.dut";
        s2 = "VirtualBus.refmod";

        oHandle1.push_back(registerObject(s1));
        oHandle1.push_back(registerObject(s2));

        // oHandle1 = registerObject(s1);
        // oHandle2 = registerObject(s2);
        cout << "Registered Object, handle=" << oHandle1[0] << endl;
        cout << "Registered Object, handle=" << oHandle1[1] << endl;
	}
	else
	{        
		string s;
		if(federateName[0] == '1')    
			s = "VirtualBus.dut";
		else
			s = "VirtualBus.refmod";

        cout << "Registrando obj " << s << endl;

		oHandle1.push_back(registerObject(s));
		cout << "Registered Object, handle=" << oHandle1[0] << endl;		
	}
}

// precisa ser ajeitada também ao final
void VirtualBusFederate::finalize(){

	//////////////////////////////////////
	// 10. delete the object we created //
	//////////////////////////////////////
    for(unsigned int i = 0; i < oHandle1.size(); i++)
	    deleteObject(oHandle1[i]);
	cout << "Deleted Object, handle=" << oHandle1[0] << endl; //alterado [0]

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
    // muito provavelmente vou ter que iniciar attributeHandles duplicados, relacionado a quem faz o publish e subscribe.
    /*
        aHandles
    */

    if(federateName[0] == '0')
    {
        this->aHandle1  = rtiamb->getObjectClassHandle( "VirtualBus.dut" );

        this->aHandle2  = rtiamb->getObjectClassHandle( "VirtualBus.refmod" );

        this->srcHandle1 = rtiamb->getAttributeHandle( "source", aHandle1 );
        this->addrHandle1 = rtiamb->getAttributeHandle( "address", aHandle1 );
        this->sizeHandle1 = rtiamb->getAttributeHandle( "size", aHandle1 );

        this->srcHandle2 = rtiamb->getAttributeHandle( "source", aHandle2 );
        this->addrHandle2 = rtiamb->getAttributeHandle( "address", aHandle2 );
        this->sizeHandle2 = rtiamb->getAttributeHandle( "size", aHandle2 );
    }
    else
    {
        if(federateName[0]== '1')
        {
            this->aHandle1  = rtiamb->getObjectClassHandle( "VirtualBus.dut" );
            this->srcHandle1 = rtiamb->getAttributeHandle( "source", aHandle1 );
            this->addrHandle1 = rtiamb->getAttributeHandle( "address", aHandle1 );
            this->sizeHandle1 = rtiamb->getAttributeHandle( "size", aHandle1 );
        }
        else
        {
            this->aHandle1  = rtiamb->getObjectClassHandle( "VirtualBus.refmod" );
            this->srcHandle1 = rtiamb->getAttributeHandle( "source", aHandle1 );
            this->addrHandle1 = rtiamb->getAttributeHandle( "address", aHandle1 );
            this->sizeHandle1 = rtiamb->getAttributeHandle( "size", aHandle1 );
        }
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
    if(federateName[0] == '0')
    {
        RTI::AttributeHandleSet *attributes1 = RTI::AttributeHandleSetFactory::create( 5 );
        RTI::AttributeHandleSet *attributes2 = RTI::AttributeHandleSetFactory::create( 5 );
        
        attributes1->add( this->srcHandle1 );
        attributes1->add( this->addrHandle1 );	
        attributes1->add( this->sizeHandle1 );

        attributes2->add( this->srcHandle2 );
        attributes2->add( this->addrHandle2 );	
        attributes2->add( this->sizeHandle2 );


        
        
        // do the actual publication
        rtiamb->publishObjectClass( this->aHandle1, *attributes1 );
        rtiamb->publishObjectClass( this->aHandle2, *attributes2 );
        //cout << " >>>>>>>>>> Atributos publicados <<<<<<<<<<" << endl;
        


        /////////////////////////////////////////////////
        // subscribe to all attributes of ObjectRoot.Robot //
        /////////////////////////////////////////////////
        // we also want to hear about the same sort of information as it is
        // created and altered in other federates, so we need to subscribe to it
        rtiamb->subscribeObjectClassAttributes( this->aHandle1, *attributes1 );
        rtiamb->subscribeObjectClassAttributes( this->aHandle2, *attributes2 );
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
        delete attributes1;
        delete attributes2;
    }
    else
    {
        RTI::AttributeHandleSet *attributes1 = RTI::AttributeHandleSetFactory::create( 5 );

        attributes1->add( this->srcHandle1 );
        attributes1->add( this->addrHandle1 );	
        attributes1->add( this->sizeHandle1 );

        rtiamb->publishObjectClass( this->aHandle1, *attributes1 );
        rtiamb->subscribeObjectClassAttributes( this->aHandle1, *attributes1 );

        delete attributes1;
    }
}

/*
 * This method will register an instance of the class ObjectRoot.A and will
 * return the federation-wide unique handle for that instance. Later in the
 * simulation, we will update the attribute values for this instance
 */
RTI::ObjectHandle VirtualBusFederate::registerObject()
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle("VirtualBus.dut") );
}

RTI::ObjectHandle VirtualBusFederate::registerObject(string s)
{
	return rtiamb->registerObjectInstance( rtiamb->getObjectClassHandle( s.c_str() ) );
}

/*
 * This method will update all the values of the given object instance. It will
 * set each of the values to be a string which is equal to the name of the
 * attribute plus the current time. eg "aa:10.0" if the time is 10.0.
 * <p/>
 * Note that we don't actually have to update all the attributes at once, we
 * could update them individually, in groups or not at all!
 */
void VirtualBusFederate::updateAttributeValues(string src, string addr, string size)
{
	///////////////////////////////////////////////
	// create the necessary container and values //
	///////////////////////////////////////////////
	// create the collection to store the values in, as you can see
	// this is quite a lot of work
	// RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create( 5 );

	// generate the new values
	// we use EncodingHelpers to make things nice friendly for both Java and C++

	//cout << "**** Adicionando atributos ****" << endl;

	// attributes->add( srcHandle1, src.c_str(), (RTI::ULong)(src.size()+1));
	// attributes->add( addrHandle1, addr.c_str(), (RTI::ULong)(addr.size()+1));
	// attributes->add( sizeHandle1, size.c_str(), (RTI::ULong)(size.size()+1));	
	//attributes->add( dataFrameHandle, (char*)dataFrame, (RTI::ULong)(sizeof(unsigned char)) * NUM_ELEMENTS);


	
	//cout << "**** Atributos adicionados ****" << endl;
	
	// note that if you want to associate a particular timestamp with the
	// update. here we send another update, this time with a timestamp:
	// RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	// rtiamb->updateAttributeValues(oHandle, *attributes, time, federateName );
	
    if(federateName[0] == '0')
    {
        RTI::AttributeHandleValuePairSet *attributes1 = RTI::AttributeSetFactory::create( 5 );
        RTI::AttributeHandleValuePairSet *attributes2 = RTI::AttributeSetFactory::create( 5 );

        attributes1->add( srcHandle1, src.c_str(), (RTI::ULong)(src.size()+1));
        attributes1->add( addrHandle1, addr.c_str(), (RTI::ULong)(addr.size()+1));
        attributes1->add( sizeHandle1, size.c_str(), (RTI::ULong)(size.size()+1));	

        attributes2->add( srcHandle2, src.c_str(), (RTI::ULong)(src.size()+1));
        attributes2->add( addrHandle2, addr.c_str(), (RTI::ULong)(addr.size()+1));
        attributes2->add( sizeHandle2, size.c_str(), (RTI::ULong)(size.size()+1));	

        RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
	    rtiamb->updateAttributeValues(oHandle1[0], *attributes1, time, federateName );
        //rtiamb->updateAttributeValues(oHandle1[1], *attributes2, time, federateName );

        delete attributes1;
        delete attributes2;
    }
    else
    {    
        RTI::AttributeHandleValuePairSet *attributes1 = RTI::AttributeSetFactory::create( 5 );

        attributes1->add( srcHandle1, src.c_str(), (RTI::ULong)(src.size()+1));
        attributes1->add( addrHandle1, addr.c_str(), (RTI::ULong)(addr.size()+1));
        attributes1->add( sizeHandle1, size.c_str(), (RTI::ULong)(size.size()+1));

        RTIfedTime time = fedamb->federateTime + fedamb->federateLookahead;
        rtiamb->updateAttributeValues(oHandle1[0], *attributes1, time, federateName );

        delete attributes1;      
    }


	// clean up
	// delete attributes;
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


