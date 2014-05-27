#include "BEN.h"


BEN::BEN(int pin, int address, void (*userFunc)(void)) {
	BEN(pin, address);

	this->intFunc = userFunc;

	this->TRIGGER_ACTIVE = false;
}

BEN::BEN(int pin, int address) {
	this->ADDRESS = address;
	this->PIN     = pin;

	this->TRIGGER_ACTIVE = false;
	this->DATA_AVAILABLE = false;

	bc.attach(this->PIN, this);
}


bool BEN::send(int address, char *message[] ) {
	return false;
}

void BEN::trigger() {
	if(TRIGGER_ACTIVE) this->intFunc();
}

//
//  BENClass
//

void BENClass::init () {
	if(!INITIALISED) {
		cli();

		if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
    		MCUSR  &= ~_BV(WDRF);                 // Clear the WDT reset flag
	    	WDTCSR |= (_BV(WDCE) | _BV(WDE));     // Enable the WD Change Bit
	    	WDTCSR  = 0x00;                       // Disable the WDT
		} else 

		{
			// Set up Watch Dog Timer for Inactivity
			WDTCSR |= (_BV(WDCE) | _BV(WDE));       // Enable the WD Change Bit
			WDTCSR  =  _BV(WDIE);// |               // Enable WDT Interrupt
			           //_BV(WDP2) | _BV(WDP1);     // Set Timeout to ~1 seconds
			sei();
		}
		INITIALISED = true;
	}
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
	INITIALISED = false;
}

void BENClass::listen() {

}

BENClass bc;

//
//  BENDataPackage
//

bool BENDataPackage::encode(int   sender    , 
							int   receiver  , 
							char  message [],
							char  retValue[], 
							char  length    ){
	

	char _length    = length > 0 ? length : sizeof message;
	char _retLength = calculateEncodedLength(_length);

	retValue = new char[_retLength];

	retValue[0] = 0x55;                  //PREFIX
	retValue[1] = (char) sender >> 8;    //MSB sender address
	retValue[2] = (char) sender;         //LSB sender address
	retValue[3] = (char) receiver >> 8;  //MSB receiver address
	retValue[4] = (char) receiver;       //MSB receiver address
	retValue[5] = _length;               //Length of Message

	for (int _i = 6, _j = 0; _i < _retLength; ++_i)
	{
		if(_i == 7){
			char _checkArray[] = {retValue[1], retValue[2], retValue[3], 
								  retValue[4], retValue[5], retValue[6]};

			retValue[_i] = BENDataPackage::checkSum(_checkArray, 6);

		}else if(_i + 1 % 4 == 0){
			char _checkArray[] = {retValue[_i - 4], retValue[_i - 3], 
								  retValue[_i - 2], retValue[_i - 1]};

			retValue[_i] = BENDataPackage::checkSum(_checkArray, 4);

		}else{
			retValue[_i] = message[_j];
			_j++;
		}

	}

	return false;
}

bool BENDataPackage::encode(BENDataPackage *package, char retValue[]){
	return BENDataPackage::encode(package->sender  , 
								  package->receiver,
								  package->message ,
								  retValue,
								  0);
}

bool BENDataPackage::encode() {
	return BENDataPackage::encode(this, this->encodedMessage);
}

char BENDataPackage::calculateEncodedLength(char messageLength) {
	char _retLength  = messageLength + 6; 
	char _checks     = (_retLength - 2) / 4;
	     _checks    += _checks % 4 > 0 ? 1 : 0; //Check at the end of the package if there is no check already
	     _retLength += _checks;
}

char BENDataPackage::checkSum(char message[], char messageLength){
	char _length = messageLength > 0 ? messageLength : sizeof message;
	char _sum    = 0;
	
	for (int _i = 0; _i < _length; ++_i)
	{
		_sum += message[_i];
	}

	return _sum % 0xff;
}

//
// ISR
//

ISR (WDT_vect) {
	//sleep_disable();        // Disable Sleep on Wakeup

 	//bc.listen();

 	//sleep_enable();         // Enable Sleep Mode
}
