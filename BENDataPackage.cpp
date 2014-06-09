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
        this->sender &= clear ? 0 : 0xF0;
        this->sender |= value;
    }

    void BENDataPackage::setSenderMSB(char value, bool clear) {
        this->sender &= clear ? 0 : 0x0F;
        this->sender |= ((int)value) << 4;
    }

    void BENDataPackage::setReceiver(int value) {
        this->receiver = value;
    }

    void BENDataPackage::setReceiverLSB(char value, bool clear) {
        this->receiver &= clear ? 0 : 0xF0;
        this->receiver |= 4;
    }

    void BENDataPackage::setReceiverMSB(char value, bool clear) {
        this->receiver &= clear ? 0 : 0x0F;
        this->receiver |= ((int)value) << 4;
    }
    
    //Will remove message
    void BENDataPackage::setMessageLength(char value, bool resetMessage) {
        if(resetMessage && this->message != NULL && value > 0) {
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
        setMessageLength(length);
        this->messagePosition = messageLength;
    }

    bool BENDataPackage::addToMessage(char value) {
        if (this->messagePosition >= this->getMessageLength()) {
            return false;
        }
        
        this->message[messagePosition] = value;
        this->messagePosition ++;
        
        return true;
    }

    char BENDataPackage::bytesOfMessageReceived(bool includingCheckSums) {

    }

    bool BENDataPackage::nextByteIsChechSum() {

    }

    bool BENDataPackage::checkSumIsValid(char  value) {

    }

    int  BENDataPackage::getSender() {

    }

    int  BENDataPackage::getReceiver() {

    }

    char* BENDataPackage::getMessage() {
        return this->message;
    }

    char BENDataPackage::getMessageLength ( bool includingCheckSums) {

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
            if(_i == 7){
                char _checkArray[] = {_retValue[1], _retValue[2], _retValue[3],
                                      _retValue[4], _retValue[5], _retValue[6]};

                _retValue[_i] = BENDataPackage::checkSum(_checkArray, 6);

            } else if(_i + 1 % 4 == 0) {
                char _checkArray[] = {_retValue[_i - 4], _retValue[_i - 3],
                                      _retValue[_i - 2], _retValue[_i - 1]};

                _retValue[_i] = BENDataPackage::checkSum(_checkArray, 4);
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
        char _retLength  = messageLength + 6; 
        char _checks     = (_retLength - 2) / 4;
             _checks    += _checks % 4 > 0 ? 1 : 0; //Check at the end of the package if there is no check already
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
