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
	
	this->receivedData_  = false;
	this->receivedData_1  = false;
	this->receivedData_2  = false;

	this->source_ = 0; this->source_1 = 0; this->source_2 = 0;
	this->address_ = 0; this->address_1 = 0; this->address_2 = 0;
	this->size_ = 0; this->size_1 = 0; this->size_2 = 0;

	for(unsigned int i = 0; i < 16; i++)
	{
		this->data_[i] = 0;
		this->data_1[i] = 0;
		this->data_2[i] = 0;
	}

}

FederateAmbassador::~FederateAmbassador()
	throw( RTI::FederateInternalError ) {
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Meus Metodos//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool FederateAmbassador::hasReceivedData(){
	return receivedData_ || receivedData_1 || receivedData_2;
}

bool FederateAmbassador::hasReceivedData_from_src(unsigned src){
	if(src == 0)
	{
		return receivedData_ ;
	}

	if(src == 1)
	{
		return receivedData_1;
	}

	if(src == 2)
	{
		return receivedData_2;
	}
}

void FederateAmbassador::getReceivedData(unsigned& src, unsigned& addr, unsigned& size, unsigned *data){

	// printf("getRD: %i\n", src);
	if(src == 0)
	{
		src = this->source_; 	
		addr = this->address_;
		size = this->size_;
		data[0] = this->data_[0];
		data[1] = this->data_[1];
		data[2] = this->data_[2];
		data[3] = this->data_[3];
		data[4] = this->data_[4];
		data[5] = this->data_[5];
		data[6] = this->data_[6];
		data[7] = this->data_[7];
		data[8] = this->data_[8];
		data[9] = this->data_[9];
		data[10] = this->data_[10];
		data[11] = this->data_[11];
		data[12] = this->data_[12];
		data[13] = this->data_[13];
		data[14] = this->data_[14];
		data[15] = this->data_[15];

		//receivedData_ =  false;
	}

	if(src == 1)
	{
		src = this->source_1; 	
		addr = this->address_1;
		size = this->size_1;
		data[0] = this->data_1[0];
		data[1] = this->data_1[1];
		data[2] = this->data_1[2];
		data[3] = this->data_1[3];
		data[4] = this->data_1[4];
		data[5] = this->data_1[5];
		data[6] = this->data_1[6];
		data[7] = this->data_1[7];
		data[8] = this->data_1[8];
		data[9] = this->data_1[9];
		data[10] = this->data_1[10];
		data[11] = this->data_1[11];
		data[12] = this->data_1[12];
		data[13] = this->data_1[13];
		data[14] = this->data_1[14];
		data[15] = this->data_1[15];

		receivedData_1 =  false;
	}

	if(src == 2)
	{
		src = this->source_2; 	
		addr = this->address_2;
		size = this->size_2;
		data[0] = this->data_2[0];
		data[1] = this->data_2[1];
		data[2] = this->data_2[2];
		data[3] = this->data_2[3];
		data[4] = this->data_2[4];
		data[5] = this->data_2[5];
		data[6] = this->data_2[6];
		data[7] = this->data_2[7];
		data[8] = this->data_2[8];
		data[9] = this->data_2[9];
		data[10] = this->data_2[10];
		data[11] = this->data_2[11];
		data[12] = this->data_2[12];
		data[13] = this->data_2[13];
		data[14] = this->data_2[14];
		data[15] = this->data_2[15];

		receivedData_2 =  false;
	}
	// src = this->source; 	
	// addr = this->address;
	// size = this->size;
	// data[0] = this->data[0];
	// data[1] = this->data[1];
	// data[2] = this->data[2];
	// data[3] = this->data[3];
	// data[4] = this->data[4];
	// data[5] = this->data[5];
	// data[6] = this->data[6];
	// data[7] = this->data[7];
	// data[8] = this->data[8];
	// data[9] = this->data[9];
	// data[10] = this->data[10];
	// data[11] = this->data[11];
	// data[12] = this->data[12];
	// data[13] = this->data[13];
	// data[14] = this->data[14];
	// data[15] = this->data[15];

	// // cout << " Getting Received Data " << data[0] << " " << data[1] << " " << data[2] << " " << endl;

	// receivedData =  false;
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
	// cout << " object=" << theObject << endl;
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

	stringstream ss;
	RTI::ULong length;
	unsigned s;
	
	length = theAttributes.getValueLength(0);
	char* src = theAttributes.getValuePointer(0, length);
	ss << src;
	ss >> s;
	ss.clear();
	ss.str(std::string());
	//printf("AMBASSADOR SEM IF %s\n", src);

	if(s == 0)
	{	
		//printf("AMBASSADOR %i\n", s);
		source_ = s;

		length = theAttributes.getValueLength(1);
		char* addr = theAttributes.getValuePointer(1, length);
		ss << addr;
		ss >> address_;
		ss.clear();
		ss.str(std::string());
						
		length = theAttributes.getValueLength(2);
		char* sz = theAttributes.getValuePointer(2, length);
		ss << sz;
		ss >> size_;
		ss.clear();
		ss.str(std::string());

		// cout << "Recebendo dados " << endl;
		for(unsigned int i = 0; i < 16; i++)
		{
			length = theAttributes.getValueLength(i+3);
			char* dt = theAttributes.getValuePointer(i+3, length);		
			ss << dt;
			ss >> data_[i];
			ss.clear();
			ss.str(std::string());
		}		

		receivedData_ =  true;  
	}

	if(s == 1)
	{
		//printf("AMBASSADOR %i\n", s);
		source_1 = s;

		length = theAttributes.getValueLength(1);
		char* addr = theAttributes.getValuePointer(1, length);
		ss << addr;
		ss >> address_1;
		ss.clear();
		ss.str(std::string());
						
		length = theAttributes.getValueLength(2);
		char* sz = theAttributes.getValuePointer(2, length);
		ss << sz;
		ss >> size_1;
		ss.clear();
		ss.str(std::string());

		// cout << "Recebendo dados " << endl;
		for(unsigned int i = 0; i < 16; i++)
		{
			length = theAttributes.getValueLength(i+3);
			char* dt = theAttributes.getValuePointer(i+3, length);		
			ss << dt;
			ss >> data_1[i];
			ss.clear();
			ss.str(std::string());
		}		

		receivedData_1 =  true;  		
	}

	if(s == 2)
	{
		//printf("AMBASSADOR %i\n", s);
		source_2 = s;

		length = theAttributes.getValueLength(1);
		char* addr = theAttributes.getValuePointer(1, length);
		ss << addr;
		ss >> address_2;
		ss.clear();
		ss.str(std::string());
						
		length = theAttributes.getValueLength(2);
		char* sz = theAttributes.getValuePointer(2, length);
		ss << sz;
		ss >> size_2;
		ss.clear();
		ss.str(std::string());

		// cout << "Recebendo dados " << endl;
		for(unsigned int i = 0; i < 16; i++)
		{
			length = theAttributes.getValueLength(i+3);
			char* dt = theAttributes.getValuePointer(i+3, length);		
			ss << dt;
			ss >> data_2[i];
			ss.clear();
			ss.str(std::string());
		}		

		receivedData_2 =  true;  
	}

	//source = theAttributes.getValuePointer(0,length0);

	

    // cout << " _-_-_ " << data[0] << " " << data[1] << " " << data[2] << " " << endl;
	
	// cout << "Dados recebidos " << endl;
	
	//receivedData =  true;    
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

