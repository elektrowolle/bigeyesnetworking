#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

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

class BEN {
public:
	int   PIN ;
	int   ADDRESS;
	
	//FLAGS
	bool  DATA_AVAILABLE;
	bool  TRIGGER_ACTIVE;

	void  (*intFunc) (void);

	BEN         ( int pin, int address, void (*) (void) );
	BEN         ( int pin, int address );
	~BEN        (  );

	bool send    ( int address, char *message[] );
	void trigger (  );
};

class BENClass {
public:
	//FLAGS
	bool ENABLED       ;
	bool INITIALISED   ;
	bool RECEIVING     ;
	bool receivedPREFIX;

	
	//VARIABLES
	BEN   *network          [ IO_PINS    ];
	char  *submissionBuffer [ BUFFERSIZE ];
	char  *receiverBuffer   [ BUFFERSIZE ];
	
	char  receivedByteBuffer;
	char  receivedByteBufferPosition;

	const static char PREFIX = 0x55;
	
	//FUNCTIONS
	BENClass     (   );
	void init    (   );
	void enable  (   );
	
	void listen  ( bool receivedBit  );
	void listen  ( char receivedByte );

	void attach  ( int pin, BEN *network );
	void trigger ( int pin );
	

};

struct BENDataPackage {
public:
	char encodedMessage [];
	char sender           ;
	char receiver         ;
	char message        [];
	//char 

	BENDataPackage  ( int   sender    , 
					  int   receiver  , 
					  char  message [], 
					  char  length = 0 );

	BENDataPackage ( char  message[], 
					 char  length = 0 );

	static bool encode ( int   sender    , 
						 int   receiver  , 
						 char  message [],
						 char  retValue[], 
						 char  length = 0 );
	
	static bool encode ( BENDataPackage *package, char retValue[] );
    
    static BENDataPackage decode ( char  message[], 
						 		   char  length = 0 );

    static char calculateEncodedLength ( char messageLength );

    static char checkSum ( char message[], char messageLength );

    bool encode();
};

extern BENClass bc;