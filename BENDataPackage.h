#ifndef ___BENDataPackage_h___
#define ___BENDataPackage_h___

namespace BEN {
    class BENDataPackage {
    public:
        char encodedMessage [];
        int  sender           ;
        int  receiver         ;
        char message        [];

        //
        //  CONTRUCTOR
        //

        BENDataPackage ( int   sender    , 
                         int   receiver  , 
                         char  message [], 
                         char  length = 0 );

        BENDataPackage ( char  message[], 
                         char  length = 0 );

        BENDataPackage (  );

        //
        //  Properties
        //

        void setSender   (  );
        void setReceiver (  );
        void setMessage  (  );

        int  getSender   (  );
        int  getReceiver (  );
        void getMessage ( char message[] );

        //
        //  Methods
        //
        

        //
        //  Utilities
        //
        static bool encode ( int & sender    , 
                             int & receiver  , 
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
