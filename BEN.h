#include <stdio.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

##ifndef IO_PINS
#define IO_PINS    13
#define BUFFERSIZE 16
#endif

class BEN{
public:
	BEN         ( int pin, void (*) (void) );
	~BEN        (  );

	bool send   ( int address, byte* message );
	void listen (  );

private:
	int  PIN ;
	static volatile voidFuncPtr intFunc;
};

class BENClass{
public:
	//FLAGS
	static volatile bool ENABLED    ;
	static volatile bool INITIALISED;
	
	//VARIABLES
	static volatile voidFuncPtr intFunc          [ IO_PINS    ];
	static volatile byte        submissionBuffer [ BUFFERSIZE ];
	static volatile byte        receiverBuffer   [ BUFFERSIZE ];
	
	//FUNCTIONS
	static volatile void init   (   );
	static volatile void enable (   );
	static volatile void listen (   );
	static volatile void attach ( int pin, void (*) (void) );
};

