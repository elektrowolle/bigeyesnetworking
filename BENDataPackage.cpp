#include "BENDataPackage.h"
//
//  BENDataPackage
//
namespace BEN {
    //
    // CONSTRUCTOR
    //
    BENDataPackage::BENDataPackage (
                     int  sender, 
                     int  receiver, 
                     char message [], 
                     char length) {

        this->messagePosition = 0;

        this->setSender  ( sender  );
        this->setReceiver( receiver);
        this->setMessage ( message, length );



        this->setMessageLength(length > 0 ? length : 0);

    }
    
    //
    // PROPERTIES
    //

    void BENDataPackage::setSender( int   value ) {
        this->sender = value;
    }

    void BENDataPackage::setSenderLSB(char value, bool clear) {
        this->sender &= clear ? 0 : 0xFF00;
        this->sender |= value;
    }

    void BENDataPackage::setSenderMSB(char value, bool clear) {
        this->sender &= clear ? 0 : 0x00FF;
        this->sender |= ((int)value) << 8;
    }

    void BENDataPackage::setReceiver(int value) {
        this->receiver = value;
    }

    void BENDataPackage::setReceiverLSB(char value, bool clear) {
        this->receiver &= clear ? 0 : 0xFF00;
        this->receiver |= value;
    }

    void BENDataPackage::setReceiverMSB(char value, bool clear) {
        this->receiver &= clear ? 0 : 0x00FF;
        this->receiver |= ((int)value) << 8;
    }
    
    //Will remove message
    void BENDataPackage::setMessageLength(char value, bool resetMessage) {
        if(resetMessage && value > 0) {
            delete this->message;
            this->setMessage(new char[value], value);
            this->messagePosition = 0;
        }
        
        this->messageLength = value > 0 ? value : 0;
        this->encodedLength = getMessageLength(true);
        
    }

    void BENDataPackage::setMessage(char value[], char length) {
        this->message = value;
        setMessageLength(length, false);
        this->messagePosition = messageLength;
    }

    bool BENDataPackage::addToMessage(char value) {
	char _bytesOfMessageReceived = this->bytesOfMessageReceived();
	if (_bytesOfMessageReceived >= this->getMessageLength()) {
            return false;
        }
        
        this->message[_bytesOfMessageReceived] = value;
        this->messagePosition++;
        
        return true;
    }

    char BENDataPackage::bytesOfMessageReceived(bool includingCheckSums) {
    	//if (includingCheckSums) {
    		return this->messagePosition;
		//} else {
//
	    	//char _checksums  = (this->messagePosition - OVERHEAD_CHECKSUM_OFFSET) / (CHECKSUM_PERIOD + 1);
	    	//if (this->messagePosition > OVERHEAD_CHECKSUM_OFFSET) {
				//++_checksums;
			//}
			//return this->messagePosition - _checksums;
		//}
    }


	/************************************************************************/
	/* This checksum related stuff turned out to be a major trouble maker   */
	/*  it's gonna be super simplified                                      */
	/************************************************************************/
	
    //bool BENDataPackage::nextByteIsChechSum() {
    	//char _bytesOfMessageWithChecksum    = bytesOfMessageReceived(true);
    	//char _bytesOfMessageWithoutChecksum = bytesOfMessageReceived(false);
//
    	//char _calculated = _bytesOfMessageWithChecksum - OVERHEARD_CHECKSUM_OFFSET;// - _checksums;
//
    	//return _bytesOfMessageWithChecksum == OVERHEARD_CHECKSUM_OFFSET
			//|| (_bytesOfMessageWithChecksum > 0 && _calculated > 0
					//&& (_calculated) % (CHECKSUM_PERIOD) == 0)
    		//||  _bytesOfMessageWithoutChecksum  == this->messageLength;
    //}
//
	//bool BENDataPackage::checkFirstByteChecksum(bool _ret, char value) {
		//char _checkSum = 0;
		//char _senderMSB = (this->getSender() >> 8) & 0xFF;
		//char _senderLSB = this->getSender() & 0xFF;
		//char _receiverMSB = (this->getReceiver() >> 8) & 0xFF;
		//char _receiverLSB = this->getReceiver() & 0xFF;
//
		//_checkSum += _senderMSB + _senderLSB;
		//_checkSum += _receiverMSB + _receiverLSB;
		//_checkSum += this->getMessageLength();
		//_checkSum += checkSum(this->message, OVERHEARD_CHECKSUM_OFFSET, 0);
		//_ret = _checkSum == value;
//
		//return _ret;
	//}
	//
	//
    //bool BENDataPackage::checksumIsValid(char value) {
    	//bool _ret = true;
		//char _bytesOfMessageWithChecksum    = bytesOfMessageReceived(true);
		//char _bytesOfMessageWithoutChecksum = bytesOfMessageReceived(false);
//
		//bool _lastByteOfMessage = _bytesOfMessageWithoutChecksum == this->messageLength;
//
		//if (_bytesOfMessageWithChecksum == OVERHEARD_CHECKSUM_OFFSET) {
			//_ret = checkFirstByteChecksum(_ret, value);
		//} else if (nextByteIsChechSum() && !_lastByteOfMessage) {
			//char _checkSum   = 0;
//
			//char _startByte = _bytesOfMessageWithoutChecksum
							   //- CHECKSUM_PERIOD
							   //+ OVERHEARD_CHECKSUM_OFFSET;
//
			//_checkSum = BENDataPackage::checkSum(this->message,
												 //_bytesOfMessageWithoutChecksum,
												 //_startByte);
//
			//_ret = _checkSum == value;
//
		//} else if (_lastByteOfMessage) {
			//char _checkSum      = 0;
			//char _length        = messagePosition / CHECKSUM_PERIOD;
			//char _startPosition = messagePosition - _length;
//
			//_checkSum = BENDataPackage::checkSum(this->message, _length, _startPosition);
//
			//_ret = _checkSum == value;
		//}
//
    	//return _ret;
    //}

    
	bool BENDataPackage::checksumIsValid(char value) {
		return value == BENDataPackage::checkSum(getSender(), getReceiver(), this->messageLength, this->message);
	}
	
    int  BENDataPackage::getSender() {
    	return this->sender;
    }

    int  BENDataPackage::getReceiver() {
    	return this->receiver;
    }

    char* BENDataPackage::getMessage() {
        return this->message;
    }

    char BENDataPackage::getMessageLength ( bool includingCheckSums ) {
    	return includingCheckSums ? calculateEncodedLength(this->messageLength) : this->messageLength;
    }

    char* BENDataPackage::getEncodedMessage() {
    	return this->encodedMessage;
    }

    //
    // METHODS
    //

    char* BENDataPackage::encode() {
        BENDataPackage::encode(this);
        return this->getEncodedMessage();
    }

    //
    //  UTILITTIES
    //
    char* BENDataPackage::encode(int    sender           ,
                                 int    receiver         ,
                                 char   message        [],
                                 char   length           ,
                                 char** encodedMessage   ,
                                 char*  encodedLength    ) {
        
        char _length    = length > 0
        			    ? length
        				: sizeof message; // This is pretty stupid.

        char _retLength = *encodedLength > 0
        					? *encodedLength
        					: calculateEncodedLength(_length);

        char _checkSums  = 1;

        *encodedLength = _retLength;

        char * _retValue = new char[_retLength];

        *encodedMessage = _retValue;

        createOverhead(_retValue, sender, receiver, _length);
        

        for (int _i = OVERHEAD, _j = 0; _i < _retLength - 1; ++_i) {
            //if(_i == OVERHEAD + OVERHEAD_CHECKSUM_OFFSET){
                //_retValue[_i]  = BENDataPackage::checkSum(_retValue, _i, 1);
//
            //} else if((_i - OVERHEAD_CHECKSUM_OFFSET - OVERHEAD) % (CHECKSUM_PERIOD) == 0) {
                //_retValue[_i] = BENDataPackage::checkSum(_retValue, _i, _i - CHECKSUM_PERIOD + 1);

            //} else if (_i == _retLength - 1) {
            	//_retValue[_i] = BENDataPackage::checkSum(_retValue, _i, _i - (_i - OVERHEAD_CHECKSUM_OFFSET - OVERHEAD) % (CHECKSUM_PERIOD + 1));

			//} else {
                _retValue[_i] = message[_j];
                _j++;

            //}
        }
		
		_retValue [_retLength - 1] = BENDataPackage::checkSum(sender, receiver, _retLength, message);
		
        return _retValue;
    }
	
	void BENDataPackage::createOverhead(char * _retValue, int sender, int receiver, char _length)
	{
		_retValue[0] = PREFIX         ;       //PREFIX
		_retValue[1] = sender   >> 8  ;       //MSB sender address
		_retValue[2] = (char) sender  ;       //LSB sender address
		_retValue[3] = receiver >> 8  ;       //MSB receiver address
		_retValue[4] = (char) receiver;       //MSB receiver address
		_retValue[5] = _length        ;       //Length of Message
	}

    void BENDataPackage::encode(BENDataPackage* package) {
        BENDataPackage::encode( package->sender       ,
                                package->receiver     ,
                                package->message      ,
                                package->messageLength,
                               &package->encodedMessage,
                               &package->encodedLength);
    }

    BENDataPackage BENDataPackage::decode(char* message, char length) {

    }

	//char BENDataPackage::getNumberOfCheckSums(char messageLength) {
		//char _checkSums = 0;
		//_checkSums = (messageLength > OVERHEARD_CHECKSUM_OFFSET ? 1 : 0)
				//+ (messageLength + OVERHEARD_CHECKSUM_OFFSET) / CHECKSUM_PERIOD;
		//_checkSums +=
				//(messageLength + OVERHEARD_CHECKSUM_OFFSET) % CHECKSUM_PERIOD != 0 ? 1 : 0;
		//return _checkSums;
	//}

    char BENDataPackage::calculateEncodedLength(char messageLength) {
        char _retLength = messageLength + OVERHEAD;
        //char _checkSums = getNumberOfCheckSums(messageLength);
        _retLength += 1;

        return _retLength;
    }

    char BENDataPackage::checkSum(int sender, int receiver, char messageLength, char message[]) {
        char _sum    = 0;
        
		_sum += sender;
		_sum += receiver;
		_sum += messageLength;
		
        for (int _i = 0; _i < messageLength; ++_i)
        {
            _sum += message[_i];
        }

        return _sum;
    }
}
