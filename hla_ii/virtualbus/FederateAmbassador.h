#ifndef FederateAmbassador_H_
#define FederateAmbassador_H_

#include <RTI.hh>
#include <NullFederateAmbassador.hh>

class FederateAmbassador : public NullFederateAmbassador
{
	public:
		// variables //
		double federateTime;
		double federateLookahead;
	
		bool isRegulating;
		bool isConstrained;
		bool isAdvancing;
		bool isAnnounced;
		bool isReadyToRun;

		char federateName[15];


		//meus atributos
	private:
		bool receivedData_ ;        
		bool receivedData_1;        
		bool receivedData_2;

		unsigned source_;		
		unsigned address_;
		unsigned size_;
		unsigned data_[16];

		unsigned source_1;		
		unsigned address_1;
		unsigned size_1;
		unsigned data_1[16];

		unsigned source_2;		
		unsigned address_2;
		unsigned size_2;
		unsigned data_2[16];                

	public:
		//meus metodos
		bool hasReceivedData();
		bool hasReceivedData_from_src(unsigned src);
		void getReceivedData(unsigned& src, unsigned& addr, unsigned& size, unsigned *data);
        


		// methods //
		FederateAmbassador(char* federateName);
		virtual ~FederateAmbassador() throw( RTI::FederateInternalError );
		
		///////////////////////////////////
		// synchronization point methods //
		///////////////////////////////////
		virtual void synchronizationPointRegistrationSucceeded( const char *label )
			throw( RTI::FederateInternalError );

		virtual void synchronizationPointRegistrationFailed( const char *label )
			throw( RTI::FederateInternalError );

		virtual void announceSynchronizationPoint( const char *label, const char *tag )
			throw( RTI::FederateInternalError );

		virtual void federationSynchronized( const char *label )
			throw( RTI::FederateInternalError );
		
		//////////////////////////
		// time related methods //
		//////////////////////////
		virtual void timeRegulationEnabled( const RTI::FedTime& theFederateTime )
			throw( RTI::InvalidFederationTime,
			       RTI::EnableTimeRegulationWasNotPending,
			       RTI::FederateInternalError );

		virtual void timeConstrainedEnabled( const RTI::FedTime& theFederateTime )
			throw( RTI::InvalidFederationTime,
			       RTI::EnableTimeConstrainedWasNotPending,
			       RTI::FederateInternalError );

		virtual void timeAdvanceGrant( const RTI::FedTime& theTime )
			throw( RTI::InvalidFederationTime,
			       RTI::TimeAdvanceWasNotInProgress,
			       RTI::FederateInternalError );
		
		///////////////////////////////
		// object management methods //
		///////////////////////////////
		virtual void discoverObjectInstance( RTI::ObjectHandle theObject,
		                                     RTI::ObjectClassHandle theObjectClass,
		                                     const char* theObjectName )  
			throw( RTI::CouldNotDiscover,
			       RTI::ObjectClassNotKnown,
			       RTI::FederateInternalError );

		virtual void reflectAttributeValues( RTI::ObjectHandle theObject,
		                                     const RTI::AttributeHandleValuePairSet& theAttributes,
		                                     const RTI::FedTime& theTime,
		                                     const char *theTag,
		                                     RTI::EventRetractionHandle theHandle)
			throw( RTI::ObjectNotKnown,
			       RTI::AttributeNotKnown,
			       RTI::FederateOwnsAttributes,
			       RTI::InvalidFederationTime,
			       RTI::FederateInternalError );

		virtual void reflectAttributeValues( RTI::ObjectHandle theObject,
		                                     const RTI::AttributeHandleValuePairSet& theAttributes,
		                                     const char *theTag )
			throw( RTI::ObjectNotKnown,
			       RTI::AttributeNotKnown,
			       RTI::FederateOwnsAttributes,
			       RTI::FederateInternalError );

		virtual void receiveInteraction( RTI::InteractionClassHandle theInteraction,
		                                 const RTI::ParameterHandleValuePairSet& theParameters,
		                                 const RTI::FedTime& theTime,
		                                 const char *theTag,
		                                 RTI::EventRetractionHandle theHandle )
			throw( RTI::InteractionClassNotKnown,
			       RTI::InteractionParameterNotKnown,
			       RTI::InvalidFederationTime,
			       RTI::FederateInternalError );

		virtual void receiveInteraction( RTI::InteractionClassHandle theInteraction,
		                                 const RTI::ParameterHandleValuePairSet& theParameters,
		                                 const char *theTag )
			throw( RTI::InteractionClassNotKnown,
			       RTI::InteractionParameterNotKnown,
			       RTI::FederateInternalError );

		virtual void removeObjectInstance( RTI::ObjectHandle theObject,
		                                   const RTI::FedTime& theTime,
		                                   const char *theTag,
		                                   RTI::EventRetractionHandle theHandle)
			throw( RTI::ObjectNotKnown,
			       RTI::InvalidFederationTime,
			       RTI::FederateInternalError );

		virtual void removeObjectInstance( RTI::ObjectHandle theObject, const char *theTag )
			throw( RTI::ObjectNotKnown, RTI::FederateInternalError );
	
		
	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// Private Section ////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	private:
		double convertTime( const RTI::FedTime& theTime );

};

#endif /*FederateAmbassador_H_*/
