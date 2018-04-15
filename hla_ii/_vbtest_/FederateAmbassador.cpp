#include <fstream>
#include <iostream>
#include <string>
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


void FederateAmbassador::getReceivedData(string& src, string& addr, string& size)
{
	src = this->source;
	addr = this->address;
	size = this->size;

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
	// cout << " object=" << theObject;
	// print the tag
	// cout << ", tag=" << theTag;
	

	RTI::ULong length;
	
	length = theAttributes.getValueLength(0);
	char* src = theAttributes.getValuePointer(0, length);
	source = src;

	length = theAttributes.getValueLength(1);
	char* addr = theAttributes.getValuePointer(1, length);
	address = addr;

	length = theAttributes.getValueLength(2);
	char* sz = theAttributes.getValuePointer(2, length);
	theAttributes.getValue(2, sz, length);
	size = sz;

	

	//cout << "DATA 8 REFLECT " << data8 << endl;

	//theAttributes.getValue(11, (char*)this->dataFrame, length);
	
	//cout << "Dados recebidos " << endl;
	
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

