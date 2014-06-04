#ifndef ___BENClass_h___
#define ___BENClass_h___

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
//#include "BENClass.h"
#include "Utility.hpp"
#include "BENDataPackage.h"
#include "BENNetwork.h"


namespace BEN {
    class BENClass {
    public:

        //FLAGS
        bool ENABLED       ;
        bool INITIALISED   ;
        bool RECEIVING     ;

        //VARIABLES
        BEN* network [ IO_PINS ];

        const static char PREFIX = 0x55;
        
        //FUNCTIONS
        BENClass     (   );
        void init    (   );
        void enable  (   );
        


        void attach  ( int pin, BEN *network );
        void trigger ( int pin );

        
        

    };

    extern BENClass bc;
}

//using BEN;

#endif // ___BENClass_h___