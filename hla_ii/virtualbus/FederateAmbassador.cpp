#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "VirtualBusFederate.h"
#include "FederateAmbassador.h"
#include "fedtime.hh"
#include <sys/time.h>


using namespace std;

FederateAmbassador::FederateAmbassador(char* federateName)
{

	strcpy(this->federateName, federateName);

	// initialize all the variable values
	this->federateTime      = 0.0;
	this->federateLookahead = 1.0;

	this->isRegulating  = false;
	this->isConstrained = false;
	this->isAdvancing   = false;
	this->isAnnounced   = false;
	this->isReadyToRun  = false;
	
	this->receivedData  = false;

}

FederateAmbassador::~FederateAmbassador()
	throw( RTI::FederateInternalError ) {
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Meus Metodos//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool FederateAmbassador::hasReceivedData(){
	return receivedData;
}

void FederateAmbassador::getReceivedData(unsigned& src, unsigned& addr, unsigned& size, unsigned *data){
	src = this->source; 	
	addr = this->address;
	size = this->size;
	data[0] = this->data[0];
	data[1] = this->data[1];
	data[2] = this->data[2];
	data[3] = this->data[3];
	data[4] = this->data[4];
	data[5] = this->data[5];
	data[6] = this->data[6];
	data[7] = this->data[7];
	data[8] = this->data[8];
	data[9] = this->data[9];
	data[10] = this->data[10];
	data[11] = this->data[11];
	data[12] = this->data[12];
	data[13] = this->data[13];
	data[14] = this->data[14];
	data[15] = this->data[15];

	// cout << " Getting Received Data " << data[0] << " " << data[1] << " " << data[2] << " " << endl;

	receivedData =  false;
}


///////////////////////////////////////////////////////////////////////////////


double FederateAmbassador::convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void FederateAmbassador::synchronizationPointRegistrationSucceeded( const char* label )
	throw(RTI::FederateInternalError)
{
	cout << "Successfully registered sync point: " << label << endl;
}

void FederateAmbassador::synchronizationPointRegistrationFailed( const char *label )
	throw(RTI::FederateInternalError)
{
	cout << "Failed to register sync point: " << label << endl;
}

void FederateAmbassador::announceSynchronizationPoint( const char *label, const char *tag )
	throw(RTI::FederateInternalError)
{
	cout << "Synchronization point announced: " << label << endl;
	if( strcmp(label,"ReadyToRun") == 0 )
		this->isAnnounced = true;
}

void FederateAmbassador::federationSynchronized( const char *label )
	throw(RTI::FederateInternalError)
{
	cout << "Federation Synchronized: " << label << endl;
	if( strcmp(label,"ReadyToRun") == 0 )
		this->isReadyToRun = true;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void FederateAmbassador::timeRegulationEnabled( const RTI::FedTime& theFederateTime )
	throw( RTI::InvalidFederationTime,
	       RTI::EnableTimeRegulationWasNotPending,
	       RTI::FederateInternalError )
{
	this->isRegulating = true;
	this->federateTime = convertTime( theFederateTime );
}

void FederateAmbassador::timeConstrainedEnabled( const RTI::FedTime& theFederateTime )
	throw( RTI::InvalidFederationTime,
	       RTI::EnableTimeConstrainedWasNotPending,
	       RTI::FederateInternalError)
{
	this->isConstrained = true;
	this->federateTime = convertTime( theFederateTime );
}

void FederateAmbassador::timeAdvanceGrant( const RTI::FedTime& theTime )
	throw( RTI::InvalidFederationTime,
	       RTI::TimeAdvanceWasNotInProgress,
	       RTI::FederateInternalError)
{
	this->isAdvancing = false;
	this->federateTime = convertTime( theTime );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////// Object Management Callbacks /////////////////////////
///////////////////////////////////////////////////////////////////////////////

//                         //
// Discover Object Methods //
//                         //
void FederateAmbassador::discoverObjectInstance( RTI::ObjectHandle theObject,
                                            RTI::ObjectClassHandle theObjectClass,
                                            const char* theObjectName )  
	throw( RTI::CouldNotDiscover,
	       RTI::ObjectClassNotKnown,
	       RTI::FederateInternalError )
{
	cout << "Discoverd Object: handle=" << theObject
	     << ", classHandle=" << theObjectClass
	     << ", name=" << theObjectName << endl;
}

//                                 // 
// Reflect Attribute Value Methods //
//                                 // 
void FederateAmbassador::reflectAttributeValues( RTI::ObjectHandle theObject,
                                            const RTI::AttributeHandleValuePairSet& theAttributes,
                                            const char *theTag )
	throw( RTI::ObjectNotKnown,
	       RTI::AttributeNotKnown,
	       RTI::FederateOwnsAttributes,
	       RTI::FederateInternalError )
{



}

void FederateAmbassador::reflectAttributeValues( RTI::ObjectHandle theObject,
                                            const RTI::AttributeHandleValuePairSet& theAttributes,
                                            const RTI::FedTime& theTime,
                                            const char *theTag,
                                            RTI::EventRetractionHandle theHandle )
	throw( RTI::ObjectNotKnown,
	       RTI::AttributeNotKnown,
	       RTI::FederateOwnsAttributes,
	       RTI::InvalidFederationTime,
	       RTI::FederateInternalError )
{
	
	//cout << "1-Reflection Received " << endl;
	
	// print the handle
	cout << " object=" << theObject << endl;
	// print the tag
	//cout << "tag= " << theTag;
	
	//Convert const str to int
	stringstream strTag;
        strTag << theTag;
        unsigned int intTag;
        strTag >> intTag;
	
	//cout << " exp_tag= " << intTag<<endl;

//        if(intTag != SENDER_ID){
//	  return;
//	}

	RTI::ULong length;
	
	unsigned* src = new unsigned[1];
	theAttributes.getValue(0,(char*)src,length);
	source = *src;

	//source = theAttributes.getValuePointer(0,length0);

	unsigned* addr = new unsigned[1];
	theAttributes.getValue(1,(char*)addr,length);

	
	address = *addr;
	
	
	
	unsigned* sz = new unsigned[1];
	theAttributes.getValue(2,(char*)sz,length);
	size = *sz;

	// cout << "Recebendo dados " << endl;

	unsigned* dt0 = new unsigned[1];
	theAttributes.getValue(3,(char*)dt0,length);
	data[0] = *dt0;
    cout << data[0] << endl;
	unsigned* dt1 = new unsigned[1];
	theAttributes.getValue(4,(char*)dt1,length);
	data[1] = *dt1;
	
	unsigned* dt2 = new unsigned[1];
	theAttributes.getValue(5,(char*)dt2,length);
	data[2] = *dt2;
	
	unsigned* dt3 = new unsigned[1];
	theAttributes.getValue(6,(char*)dt3,length);
	data[3] = *dt3;
	
	unsigned* dt4 = new unsigned[1];
	theAttributes.getValue(7,(char*)dt4,length);
	data[4] = *dt4;

	unsigned* dt5 = new unsigned[1];
	theAttributes.getValue(8,(char*)dt5,length);
	data[5] = *dt5;

	unsigned* dt6 = new unsigned[1];
	theAttributes.getValue(9,(char*)dt6,length);
	data[6] = *dt6;

	unsigned* dt7 = new unsigned[1];
	theAttributes.getValue(10,(char*)dt7,length);
	data[7] = *dt7;

	unsigned* dt8 = new unsigned[1];
	theAttributes.getValue(11,(char*)dt8,length);
	data[8] = *dt8;

	unsigned* dt9 = new unsigned[1];
	theAttributes.getValue(12,(char*)dt9,length);
	data[9] = *dt9;

	unsigned* dt10 = new unsigned[1];
	theAttributes.getValue(13,(char*)dt10,length);
	data[10] = *dt10;

	unsigned* dt11 = new unsigned[1];
	theAttributes.getValue(14,(char*)dt11,length);
	data[11] = *dt11;

	unsigned* dt12 = new unsigned[1];
	theAttributes.getValue(15,(char*)dt12,length);
	data[12] = *dt12;

	unsigned* dt13 = new unsigned[1];
	theAttributes.getValue(16,(char*)dt13,length);
	data[13] = *dt13;

	unsigned* dt14 = new unsigned[1];
	theAttributes.getValue(17,(char*)dt14,length);
	data[14] = *dt14;

	unsigned* dt15 = new unsigned[1];
	theAttributes.getValue(18,(char*)dt15,length);
	data[15] = *dt15;

    // cout << " _-_-_ " << data[0] << " " << data[1] << " " << data[2] << " " << endl;
	
	// cout << "Dados recebidos " << endl;
	
	receivedData =  true;    
}

//                             //
// Receive Interaction Methods //
//                             //
void FederateAmbassador::receiveInteraction( RTI::InteractionClassHandle theInteraction,
                                        const RTI::ParameterHandleValuePairSet& theParameters,
                                        const char *theTag )
	throw( RTI::InteractionClassNotKnown,
	       RTI::InteractionParameterNotKnown,
	       RTI::FederateInternalError )
{
	cout << "Interaction Received:";
	
	// print the handle
	cout << " handle=" << theInteraction;
	// print the tag
	cout << ", tag=" << theTag;
	
	// print the attribute information
	cout << ", parameterCount=" << theParameters.size() << endl;
	for( RTI::ULong i = 0; i < theParameters.size(); i++ )
	{
		// print the parameter handle
		cout << "\tparamHandle=" << theParameters.getHandle(i);
		// print the parameter value
		RTI::ULong length = theParameters.getValueLength(i);
		char *value = theParameters.getValuePointer(i,length);
				
		cout << ", paramValue=" << value << endl;
	}
}

void FederateAmbassador::receiveInteraction( RTI::InteractionClassHandle theInteraction,
                                        const RTI::ParameterHandleValuePairSet& theParameters,
                                        const RTI::FedTime& theTime,
                                        const char *theTag,
                                        RTI::EventRetractionHandle theHandle )
	throw( RTI::InteractionClassNotKnown,
	       RTI::InteractionParameterNotKnown,
	       RTI::InvalidFederationTime,
	       RTI::FederateInternalError )
{
	cout << "Interaction Received:";
	
	// print the handle
	cout << " handle=" << theInteraction;
	// print the tag
	cout << ", tag=" << theTag;
	// print the time 
	cout << ", time=" << convertTime( theTime );
	
	// print the attribute information
	cout << ", parameterCount=" << theParameters.size() << endl;
	for( RTI::ULong i = 0; i < theParameters.size(); i++ )
	{
		// print the parameter handle
		cout << "\tparamHandle=" << theParameters.getHandle(i);
		// print the parameter value
		RTI::ULong length = theParameters.getValueLength(i);
		char *value = theParameters.getValuePointer(i,length);
				
		cout << ", paramValue=" << value << endl;
	}
}

//                       //
// Remove Object Methods //
//                       //
void FederateAmbassador::removeObjectInstance( RTI::ObjectHandle theObject, const char *theTag )
	throw( RTI::ObjectNotKnown, RTI::FederateInternalError )
{
	cout << "Object Removed: handle=" << theObject << endl;
}

void FederateAmbassador::removeObjectInstance( RTI::ObjectHandle theObject,
                                          const RTI::FedTime& theTime,
                                          const char *theTag,
                                          RTI::EventRetractionHandle theHandle)
	throw( RTI::ObjectNotKnown,
	       RTI::InvalidFederationTime,
	       RTI::FederateInternalError )
{
	cout << "Object Removed: handle=" << theObject << endl;
}

