#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

//
//  Let`s do a time modulated slow but always everywhere available 
//  network protocol!
//
//  HIGH +---v-----v--------+
//       |    -----         |
//       |   /     \        |  TODO: This Graph doesn`t have any
//       |---       --------|        meaning yet
//   LOW +---^-----^--------+
//       0   1     2
//

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
	bool ENABLED    ;
	bool INITIALISED;
	
	//VARIABLES
	uintptr_t intFunc           [ IO_PINS    ];
	char      *submissionBuffer [ BUFFERSIZE ];
	char      *receiverBuffer   [ BUFFERSIZE ];
	
	//FUNCTIONS

	BENClass ();
	void init    (   );
	void enable  (   );
	void listen  (   );
	void attach  ( int pin, void (*) (void) );
	void trigger ( int pin );

};
#ifndef __INITBC
#define __INITBC 1

extern BENClass bc;
#endif