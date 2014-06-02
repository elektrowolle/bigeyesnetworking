#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
//#include "BENClass.h"
#include "BEN.h"

class BENClass {
public:

    //  
    //  TODO:
    //  Some of these flags need to be moved to [BEN] to make multiple
    //  connetions at the same time possible.
    //  

    //FLAGS
    bool ENABLED       ;
    bool INITIALISED   ;
    bool RECEIVING     ;

    //VARIABLES
    BEN   *network          [ IO_PINS ];

    const static char PREFIX = 0x55;
    
    //FUNCTIONS
    BENClass     (   );
    void init    (   );
    void enable  (   );
    


    void attach  ( int pin, BEN *network );
    void trigger ( int pin );

    static bool isPrefix ( char byteToCheck, char length );
    

};

extern BENClass bc;
