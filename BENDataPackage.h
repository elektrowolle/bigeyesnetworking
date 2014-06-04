#ifndef ___BENDataPackage_h___
#define ___BENDataPackage_h___

namespace BEN {
    class BENDataPackage {
    public:
        char encodedMessage [];
        int  sender           ;
        int  receiver         ;
        char *message        [];
        //char 
        BENDataPackage ( int   sender    , 
                         int   receiver  , 
                         char  message [], 
                         char  length = 0 );

        BENDataPackage ( char  message[], 
                         char  length = 0 );

        BENDataPackage (  );

        static bool encode ( int   sender    , 
                             int   receiver  , 
                             char  message [],
                             char  retValue[], 
                             char  length = 0 );
        
        static bool encode ( BENDataPackage* package, char retValue[] );
        
        static BENDataPackage decode ( char  message[], 
                                       char  length = 0 );

        static char calculateEncodedLength ( char messageLength );

        static char checkSum ( char message[], char messageLength );

        bool encode();
    };
}

#endif //___BENDataPackage_h___
