
using namespace std;

VirtualBus::VirtualBus(){
	
	 // create and run the federate
        VirtualBusFederate *federate;
        federate = new VirtualBusFederate();
        federate->runFederate( federateName );

        printf("main: Federate started...\n");
}

VirtualBus::~VirtualBus()
{
	if( this->fedamb )
		delete this->fedamb;
}
