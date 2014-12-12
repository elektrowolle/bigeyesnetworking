#ifndef ___BEN_H___
#define ___BEN_H___

#include "BENDataPackage.h"
#include "Utility.hpp"

namespace BEN {
    //  Let`s do a time modulated slow but always everywhere available 
    //  network protocol!
    //       signal
    //       +-----v-----v-----v-----v-----v------+
    //  HIGH |      -----       -----------       |
    //       |     /     \     /           \      |
    //   LOW |-----       -----             ------|
    //   time+-----^-----^-----^-----^-----^------+
    //        0    1     2     3     4     5
    //             \_____/     \___________/
    //                0  |           1    |
    //       short signal|     long signal|
    //
    //  Short signals will be interpreted as a binary 0
    //   Long signals will be interpreted as a binary 1
    //
    //  Data Packages:
    //  
    //  EXAMPLE: A device (0x01) want to say 'Hello!' to another 
    //           device (A9)
    //  
    //   8: 000-008 PREFIX    [01010101]
    //  16: 009-041 SENDER    [ 00  01h]
    //  16: 042-074 RECEIVER  [ 00  A9h]
    //  16: 075-083 LENGTH OF [00000110]
    //      MESSAGE (bytes)   
    //   8: 084-092 MSG   'H' [01001000]
    //   8: 102-110 MSG   'e' [01100101]
    //   8: 111-119 MSG   'l' [01101100]
    //   8: 120-128 MSG   'l' [01101100]
    //   8: 129-137 MSG   'o' [01101111]
    //   8: 147-155 MSG   '!' [00010001]
    //   8: 156-164 CHECK     [00010001]  # Sum(SENDER,RECEIVER,LOM,MSG) % 255
    //
    //  By the way. mod ffh isn't needed in a byte.

    //class BENDataPackage;

    

  class BENNetwork {
  public:
      int   LOCAL_ADDRESS;

      char  *submissionBuffer [ BUFFERSIZE ];
      char  *receiverBuffer   [];

      char  receivedBitBuffer;
      char  receivedBitBufferPosition;

      BENDataPackage* availableData;
      
      char messageLength;

      char STATES;
      // STATES

      //////  OUTDATED ///////////
      //  [0000 0000]
      //   |||| |||\___{  1} IN_PROGRESS
      //   |||| ||\____{  2} TRIGGER_ACTIVE
      //   |||| |\_____{  4} RECEIVED_PREFIX
      //   |||| \______{  8} LISTEN_TO_SENDER_ADDRESS
      //   |||\________{ 16} LISTEN_TO_RECIVER_ADDRESS
      //   ||\_________{ 32} RECEIVING_MESSAGE_LENGTH
      //   |\__________{ 64} RECEIVING_MESSAGE
      //   \___________{128} CHECKSUM_IS_INCORRECT
      //////  \OUTDATED ///////////

      //  [0000 0000]
      //   |||| |||\___{  1} (ACTIVITY_)IN_PROGRESS
      //   |||| ||\____{  2} TRIGGER_ACTIVE
      //   |||| ||              
      //   |||| |\_____ }                    {  4} RECEIVING_PREFIX          
      //   |||| \______ }    Activity Bits   {  8} LISTEN_TO_SENDER_ADDRESS
      //   |||\________ }                    { 12} LISTEN_TO_RECIVER_ADDRESS
      //   |||                               { 16} RECEIVING_MESSAGE_LENGTH
      //   |||                               { 20} RECEIVING_MESSAGE
      //   |||                               { 24} RESERVED (NO FUNCTION)
      //   |||                               { 28} RESERVED (NO FUNCTION)
      //   |||                               
      //   |||
      //   ||\_________{ 32} DATA_READY
      //   |\__________{ 64} ABORTED
      //   \___________{128} CHECKSUMS_IS_INCORRECT

    arg2FuncContainer<void, BENNetwork*, char> __statesChange;
    #define statesChange(_pS) __statesChange(this, this->STATES ^ _pS);

    BENNetwork          ( int address = 0, 
                          arg2FuncContainer<void, BENNetwork*, char> _statesChange = 
                              (arg2FuncContainer<void, BENNetwork*, char>){NULL});
    ~BENNetwork         (  );

    bool send           ( int address, char *message[] );
    

    //DECODING
    void listen         ( bool receivedBit  );
    void listen         ( char receivedByte );
    
    void resetBitBuffer  (  );
    void resetFlags      (  );
    void activateState   ( char stateByte );
    void deactivateState ( char stateByte );
    void changeActivity  ( char stateByte );
    bool checkActivity   ( char activity  );
    char getActivity     ();
    bool checkState      ( char stateByte );
    void clearMessage    (  );

  private:
  	void addToBitBuffer          ( bool receivedBit  );
  	void listenToReceiverAddress ( char receivedByte );
  	void listenToMessageLength   ( char receivedByte );
  	void listenToSenderAddress   ( char receivedByte );
  	void listenToMessage         ( char receivedByte );
  	void listenToPrefix          ( bool receivedBit  );
  };

}

#endif // ___BEN_H___
