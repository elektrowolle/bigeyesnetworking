#include "BENDuinoIO.h"
namespace BEN{
	BENDuinoIO::BENDuinoIO (int address, char netPin){
		// funcContainer fc;
		// fc = BEN::transmit;
		ben = new BENNetwork(address, 
												 (arg1FuncContainer<void, char>) {BEN::statesChanged});

	}

	BENDuinoIO::~BENDuinoIO (){
		delete ben;
	}

	void statesChanged (char changedBytes){

	}
};