#include "BEN.h"

BEN::BEN (int pin, int address, void (*userFunc)(void)) {
	this->ADDRESS = address;
	this->PIN     = pin;
	this->intFunc = userFunc;

	BENClass::attach(this->PIN, this->intFunc);
}

bool BEN::send (int address, char *message[] ) {
	return false;
}

void BENClass::attach (int pin, void (*userFunc)(void)) {
	BENClass::init();
	BENClass::intFunc[pin] = (uintptr_t) &userFunc;
}

void BENClass::init () {
	if(!BENClass::ENABLED) {
		cli();

		if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
    		MCUSR  &= ~_BV(WDRF);                 // Clear the WDT reset flag
	    	WDTCSR |= (_BV(WDCE) | _BV(WDE));     // Enable the WD Change Bit
	    	WDTCSR  = 0x00;                       // Disable the WDT
		} else 
		{
			// Set up Watch Dog Timer for Inactivity
			WDTCSR |= (_BV(WDCE) | _BV(WDE));       // Enable the WD Change Bit
			WDTCSR  =   _BV(WDIE);// |              // Enable WDT Interrupt
			           //_BV(WDP2) | _BV(WDP1);     // Set Timeout to .16 seconds
			sei();
		}
	}
}

void BENClass::trigger(int pin) {
	void (*_funcPtr) (void) = (void (*)(void)) intFunc[pin];
	_funcPtr();
}


ISR (WDT_vect) {
	//sleep_disable();        // Disable Sleep on Wakeup

 	BENClass::listen();

 	//sleep_enable();         // Enable Sleep Mode
}