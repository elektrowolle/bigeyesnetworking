#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>


#ifndef IO_PINS
#define IO_PINS    13
#define BUFFERSIZE 16
#endif

class BEN{
public:
	int   PIN ;
	int   ADDRESS;
	void  (*intFunc) (void);

	BEN         ( int pin, int address, void (*) (void) );
	~BEN        (  );

	bool send   ( int address, char *message[] );
};

class BENClass{
public:
	//FLAGS
	static bool ENABLED    ;
	static bool INITIALISED;
	
	//VARIABLES
	static uintptr_t intFunc           [ IO_PINS    ];
	static char      *submissionBuffer [ BUFFERSIZE ];
	static char      *receiverBuffer   [ BUFFERSIZE ];
	
	//FUNCTIONS
	static void init    (   );
	static void enable  (   );
	static void listen  (   );
	static void attach  ( int pin, void (*) (void) );
	static void trigger ( int pin );
};

