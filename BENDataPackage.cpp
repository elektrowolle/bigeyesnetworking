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



        this->setMessageLength(length == 0 ? message == NULL ? 0 : sizeof message : length);

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

            //char a[5]
            //a == a[0]
            //*(a+1) == a[1]
            //*(a+i) == a[i]

            this->messagePosition = 0;
        }
        
        this->messageLength = value > 0 ? value : 0;
        
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
    	if (includingCheckSums) {
    		return this->messagePosition;
		} else {
			return this->messagePosition - (this->messagePosition > FIRST_CHECKSUM ? 1 : 0) - ((this->messagePosition + FIRST_CHECKSUM) / CHECKSUM_PERIOD);
		}
    }

    bool BENDataPackage::nextByteIsChechSum() {
    	return this->bytesOfMessageReceived(true) == FIRST_CHECKSUM ||
    		  (this->bytesOfMessageReceived(true) + FIRST_CHECKSUM) % CHECKSUM_PERIOD == 0;
    }

    bool BENDataPackage::checkSumIsValid(char value, bool increaseMessagePositionCounter) {
    	bool _ret = true;

    	if (bytesOfMessageReceived(true) == FIRST_CHECKSUM) {
    		char _checkSum = 0;
    		char _max = FIRST_CHECKSUM;
    		_checkSum += ((char) this->getSender  () << 8) + ((char) this->getSender  () & 0xFF);
    		_checkSum += ((char) this->getReceiver() << 8) + ((char) this->getReceiver() & 0xFF);
    		_checkSum += this->getMessageLength();

    		_checkSum += checkSum(this->message, FIRST_CHECKSUM, 0);

    		_ret = _checkSum == value;

		} else if ((bytesOfMessageReceived(true) + FIRST_CHECKSUM) % CHECKSUM_PERIOD == 0) {
			char _checkSum = 0;
			_checkSum = BENDataPackage::checkSum(this->message, CHECKSUM_PERIOD, messagePosition - CHECKSUM_PERIOD);

			_ret = _checkSum == value;

		} else if (bytesOfMessageReceived(true) == this->messageLength) {
			char _checkSum      = 0;
			char _length        = messagePosition / CHECKSUM_PERIOD;
			char _startPosition = messagePosition - _length;

			_checkSum = BENDataPackage::checkSum(this->message, _length, _startPosition);

			_ret = _checkSum == value;
		}

    	if (increaseMessagePositionCounter) {
    		messagePosition++;
		}

    	return _ret;
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
        
        char _length    = length > 0 ? length : sizeof message;
        char _retLength = calculateEncodedLength(_length);

        *encodedLength = _retLength;

        char * _retValue = new char[_retLength];

        *encodedMessage = _retValue;

        _retValue[0] = 0x55;                  //PREFIX
        _retValue[1] = (char) sender   >> 8;  //MSB sender address
        _retValue[2] = (char) sender;         //LSB sender address
        _retValue[3] = (char) receiver >> 8;  //MSB receiver address
        _retValue[4] = (char) receiver;       //MSB receiver address
        _retValue[5] = _length;               //Length of Message

        for (int _i = 6, _j = 0; _i < _retLength; ++_i) {
            if(_i == 6 + FIRST_CHECKSUM){
                _retValue[_i]  = BENDataPackage::checkSum(_retValue, _i);

            } else if(_i + FIRST_CHECKSUM % CHECKSUM_PERIOD == 0) {
                _retValue[_i] = BENDataPackage::checkSum(_retValue, CHECKSUM_PERIOD, _i - CHECKSUM_PERIOD);

            } else {
                _retValue[_i] = message[_j];
                _j++;

            }
        }
        return _retValue;
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

    char BENDataPackage::calculateEncodedLength(char messageLength) {
        char _retLength = messageLength + 6;
        char _checks = 0;
        _checks  = (messageLength > FIRST_CHECKSUM ? 1 : 0) + (messageLength + FIRST_CHECKSUM) / CHECKSUM_PERIOD;
        _checks += (messageLength + FIRST_CHECKSUM) % CHECKSUM_PERIOD != 0 ? 1 : 0; //Check at the end of the package if there is no check already

        _retLength += _checks;

        return _retLength;
    }

    char BENDataPackage::checkSum(char message[], char messageLength, char startWithByte) {
        char _length = messageLength > 0 ? messageLength : sizeof message;
        char _sum    = 0;
        
        for (int _i = startWithByte; _i < _length; ++_i)
        {
            _sum += message[_i];
        }

        return _sum % 0xff;
    }
}
