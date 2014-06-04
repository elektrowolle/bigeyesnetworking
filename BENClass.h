#ifndef ___BENClass_h___
#define ___BENClass_h___

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

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
        BENNetwork* network [ IO_PINS ];
        
        //FUNCTIONS
        BENClass     (   );
        void init    (   );
        void enable  (   );
        


        void attach  ( int pin, BENNetwork *network );
        void trigger ( int pin );

        
        

    };

    extern BENClass bc;
}

//using BEN;

#endif // ___BENClass_h___