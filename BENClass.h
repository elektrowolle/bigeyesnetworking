#ifndef ___BENClass_h___
#define ___BENClass_h___

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
        List<BENNetwork*>* network;
                
        //FUNCTIONS
        BENClass     (   );
        void init    (   );
        void enable  (   );

        void attach  (BENNetwork *network );
    };

    extern BENClass * bc;

}



//using BEN;

#endif // ___BENClass_h___
