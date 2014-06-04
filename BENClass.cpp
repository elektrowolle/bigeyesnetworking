#include "BENClass.h"
//
//  BENClass
//
namespace BEN {

	void BENClass::init () {

	}

	void BENClass::enable() {
	    ENABLED = true;
	}

	void BENClass::trigger(int pin) {
	    network[pin]->trigger();
	}

	void BENClass::attach(int pin, BEN *newNetwork) {
	    network[pin] = newNetwork;
	}

	BENClass::BENClass() {
	    INITIALISED    = false;
	    RECEIVING      = false;
	}

	BENClass bc;

}

