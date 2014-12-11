#include "BENClass.h"
//
//  BENClass
//
namespace BEN {

	void BENClass::init () {
		network = NULL;
	}

	void BENClass::enable() {
	    ENABLED = true;
	}


	void BENClass::attach(BENNetwork* newNetwork) {
		if (network != NULL) {
			network = new List<BENNetwork*> (newNetwork);

		}else{
			network->push(newNetwork);
		}
	}

	BENClass::BENClass() {
	    this->INITIALISED = false;
	    this->RECEIVING   = false;
	    this->network     = NULL;
	    this->ENABLED     = true;
	}

	BENClass * bc = new BENClass();

}


