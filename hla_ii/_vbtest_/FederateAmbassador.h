#ifndef FederateAmbassador_H_
#define FederateAmbassador_H_

#include <RTI.hh>
#include <NullFederateAmbassador.hh>

#include <string>
using namespace std;

#define NUM_ELEMENTS 2

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
		bool receivedData;
		string source;
		string address;
		string size;
		
		
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

	public:
		//meus metodos
		bool hasReceivedData();
		void getReceivedData(string& src, string& addr, string& size);

	


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
