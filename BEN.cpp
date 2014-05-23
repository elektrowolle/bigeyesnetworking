#include "BEN.h"

BEN::BEN ( int pin, void (*userFunc)(void) ) {
	PIN     = 0       ;
	intFunc = userFunc;
	BENClass.attach(PIN, intFunc);
}

BENClass::attach ( int pin, void (*userFunc)(void) ) {
	BENCLass.init();
	
	intFunc[pin] = userFunc;

}

BENClass::init () {
	if(!BENClass.ENABLED) {
		cli();

		if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
    		MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
	    	WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
	    	WDTCSR = 0x00;                      // Disable the WDT
		} else 
		{
			// Set up Watch Dog Timer for Inactivity
			WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
			WDTCSR =   _BV(WDIE) |              // Enable WDT Interrupt
			           _BV(WDP2) | _BV(WDP1);   // Set Timeout to ~1 seconds
			sei();
		}
	}
}

ISR (WDT_vect) {
	sleep_disable();        // Disable Sleep on Wakeup

 	BENClass.listen();

 	sleep_enable();        // Enable Sleep Mode
}