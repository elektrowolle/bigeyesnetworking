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
	INITIALISED    = false;
	RECEIVING      = false;
	receivedPREFIX = false;

	receivedByteBuffer         = 0;
	receivedByteBufferPosition = 0;
}

void BENClass::listen(bool receivedBit) {
	
	//  I'll probably forget one day what I've been doing here. So I'll explain about.
	//  Everytime this method is called we get one new bit. Also this method utilize
	//  The members [receivedByteBuffer], [receivedByteBufferPosition], the 
	//  flag [receivedPREFIX] and the mask [PREFIX].
	//
	//  Basically the purpose of this method is to collect single bits and arange 
	//  them to a byte ([receivedByteBuffer]) to give them to the byte[listen]er 
	//  which will process this byte. At the same time it does some funny but 
	//  necessary checks. Let's do the fun Part!:
	//
	//  So we start with an empty [receivedByteBuffer] (0x00). As it's documentated
	//  in the header the first byte to receive at a new message is the header. That 
	//  is to prevent interpreting random data. So what we're looking for at the 
	//  beginningis a sequence with 0x55 (binary: {0101 0101}). As we listen bit by 
	//  bit we have to check if any of these bits are the starting point of a new 
	//  message. So simply spoken we check everytime if the last incoming bits had 
	//  been same as [PREFIX]. If they aren't equal we empty the buffer and try again 
	//  with the next set of bits. As soon as we found a sequence which is equal to 
	//  [PREFIX] we'll set the [receivedPREFIX] Flag, collect more bits to crunch 
	//  them into bytes and forward them to the byte[listen]er. But how does that 
	//  work?
	//  
	//  Our empty Buffer looks like this: {0000 0000}
	//  Now we get a new bit, say '1'. We want to put this bit to the MSB of the 
	//  Buffer (The first position from left). To do this we will add the bit to the 
	//  buffer to get {0000 0001} and now rotate the buffer by 7 to left. The result
	//  will be {1000 0000}. This value will be compared with [PREFIX]. To do this 
	//  it's necesarry to mask all bits of [PREFIX] which aren't yet set in the 
	//  buffer. That's done by doing a binary 'and' ('&') operation with a byte full 
	//  of ones {1111 1111} which is rotated by the same amount as the buffer is 
	//  rotated yet (7 times). what is left is {1000 0000}. Doing an and operation 
	//  with the [PREFIX] {0101 0101} will result in {0000 0000}. That looks like a 
	//  complicated way to produce a byte of zeros but it's purpose will be released 
	//  in short time. This result compared to the buffer ({1000 0000} == {0000 0000}}
	//  Is not the same. That means that the process have to start over. This 
	//  probbably means that something gibberish alien stuff occured on our data 
	//  line. Let us ignore the fact of aliens, listen to the next bit and continue:
	//  
	//  The next bit received in our example is a 0. we do the same process as 
	//  before: rotate the buffer by 7, pick up our [PREFIX] mask and mask which we 
	//  rotate by 7. We compare both ({0000 0000} == {0000 0000}) and 
	//  have a match! Let us continue. next bit is a 1. rotate it by 6, grab the
	//  [PREFIX] mask it (after rotating the mask by 6). Compare 
	//  ({0100 0000} == {0100 0000}) and it's a match again! We do this routine 
	//  till we have a complete match between [receivedByteBuffer] and [PREFix]
	//  ({0101 0101} == {0101 0101}). We found the prefix! It's super likely to get
	//  Usefull data from now on. That's a good thing. Let's celebrate by setting 
	//  the [receivedPREFIX] = true.
	//  


	receivedByteBuffer += (receivedByteBuffer ? 1 : 0) << 7 - receivedByteBufferPosition;
	receivedByteBufferPosition++;

	if (!receivedPREFIX && !isPrefix(receivedByteBuffer, receivedByteBufferPosition))
	{
		receivedByteBuffer = 0;
	 	receivedByteBufferPosition = 0;

	}else {
		receivedPREFIX = true;
	}

	// if(!receivedPREFIX && receivedByteBufferPosition > 1
	// 	&& receivedByteBuffer != PREFIX & (0xff << 7 - receivedByteBufferPosition) {
		
	// 	receivedByteBuffer = 0;
	// 	receivedByteBufferPosition = 0;
	// }
	
	if (receivedByteBufferPosition > 7) {
		listen(receivedByteBuffer);

		receivedByteBuffer         = 0;
		receivedByteBufferPosition = 0;
	}
}

void BENClass::listen(char receivedByte) {

}

bool BENClass::isPrefix(char byteToCheck, char length) {

	return byteToCheck != PREFIX & (0xff << 8 - length);
}


BENClass bc;

//
//  BENDataPackage
//

bool BENDataPackage::encode(int   sender    , 
							int   receiver  , 
							char  message [],
							char  retValue[], 
							char  length    ) {
	
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

bool BENDataPackage::encode(BENDataPackage *package, char retValue[]) {
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

char BENDataPackage::checkSum(char message[], char messageLength) {
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
