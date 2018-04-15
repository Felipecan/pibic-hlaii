#include <string.h>
#include "VirtualBusFederate.h"
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
//#include <mutex>

using namespace std;
// using namespace cv;



int main( int argc, char *argv[] )
{
	if( argc > 2 )
	{
		// create and run the federate
		VirtualBusFederate *federate;
		federate = new VirtualBusFederate();
		federate->runFederate( argv[1] );
        // cout << "PRINT" << argv[2] << endl;
        string s;
        // cin >> s;
		federate->loop(0, argv[2]);
		federate->finalize();

		// clean up
		delete federate;
	}
	
	return 0;
}
