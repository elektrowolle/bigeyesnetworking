#include "BENDataPackage.h"

//
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
//   8: 009-041 SENDER    [ 00  01h]
//  16: 042-074 RECEIVER  [ 00  A9h]
//  16: 075-083 LENGTH OF [00000110]
//      MESSAGE (bytes)   
//   8: 084-092 MSG   'H' [01001000]
//   8: 093-101 CHECK     [11111000] ((00h+01h+00h+A9h+06h+48h) mod ffh = F8h)
//   8: 102-110 MSG   'e' [01100101]
//   8: 111-119 MSG   'l' [01101100]
//   8: 120-128 MSG   'l' [01101100]
//   8: 129-137 MSG   'o' [01101111]
//   8: 138-146 CHECK     [10101101] ((65h+6Ch+6Ch+6Fh) mod ffh = ADh)
//   8: 147-155 MSG   '!' [00010001]
//   8: 156-164 CHECK     [00010001]
//
//


#ifndef IO_PINS
#define IO_PINS    13
#define BUFFERSIZE 16
#endif

#ifndef __BEN_PROPERTIES
#define __BEN_PROPERTIES 1
#define DEFAULT_STATES             0x00
#define DATA_AVAILABLE             0x01
#define TRIGGER_ACTIVE             0x02
#define RECEIVED_PREFIX            0x04
#define LISTEN_TO_SENDER_ADDRESS   0x08
#define LISTEN_TO_RECEIVER_ADDRESS 0x10
#define RECEIVING_MESSAGE_LENGTH   0x20
#define RECEIVING_MESSAGE          0x40
#define CHECKSUMS_ARE_CORRECT      0x80
#endif 

class BEN {
public:
    int   PIN ;
    int   ADDRESS;

    char  *submissionBuffer [ BUFFERSIZE ];
    char  *receiverBuffer   [];

    char  receivedBitBuffer;
    char  receivedBitBufferPosition;

    BENDataPackage *availableData;
    
    //FLAGS

    //
    //  [0000 0000]
    //   |||| |||\___{  1} DATA_AVAILABLE
    //   |||| ||\____{  2} TRIGGER_ACTIVE
    //   |||| |\_____{  4} RECEIVED_PREFIX
    //   |||| \______{  8} LISTEN_TO_SENDER_ADDRESS
    //   |||\________{ 16} LISTEN_TO_RECIVER_ADDRESS
    //   ||\_________{ 32} RECEIVING_MESSAGE_LENGTH
    //   |\__________{ 64} RECEIVING_MESSAGE
    //   \___________{128} CHECKSUMS_ARE_CORRECT

    
    char messageLength;
    char STATES;

    void  (*intFunc) (void);

    BEN         ( int pin, int address, void (*) (void) );
    BEN         ( int pin, int address );
    ~BEN        (  );

    bool send           ( int address, char *message[] );
    void trigger        (  );

    //DECODING
    void listen         ( bool receivedBit  );
    void listen         ( char receivedByte );
    
    void resetBitBuffer (  );
    void resetFlags     (  );
    void clearMessage   (  );
};

