#ifndef ___BENDataPackage_h___
#define ___BENDataPackage_h___

#include "Utility.hpp"

namespace BEN {
    class BENDataPackage {
    public:
        char* encodedMessage ;
        char  encodedLength  ;
        int   sender         ;
        int   receiver       ;
        char* message        ;
        char  messagePosition;
        char  messageLength  ;

        //
        //  CONTRUCTOR
        //

        BENDataPackage ( int   sender     = 0, 
                         int   receiver   = 0, 
                         char  message [] = NULL, 
                         char  length     = 0 );

        //
        //  Properties
        //

        void setSender        ( int   value );
        void setSenderLSB     ( char  value, bool clear = false );
        void setSenderMSB     ( char  value, bool clear = false );
        void setReceiver      ( int   value );
        void setReceiverLSB   ( char  value, bool clear = false );
        void setReceiverMSB   ( char  value, bool clear = false );
        void setMessageLength ( char  value, bool resetMessage = false );
        
        void setMessage             ( char value [], char length      );
        bool addToMessage           ( char  value                     );
        char bytesOfMessageReceived ( bool includingCheckSums = false );
        bool nextByteIsChechSum     (  );
        bool checkSumIsValid        ( char  value                     );

        int    getSender         (  );
        int    getReceiver       (  );
        char* getMessage         (  );
        char   getMessageLength  ( bool includingCheckSums = false );
        char* getEncodedMessage  (  );

        //
        //  Methods
        //
        
        char* encode();

        //
        //  Utilities
        //
        static char* encode ( int    sender    ,
                              int    receiver  ,
                              char   message [],
                              char   length    ,
                              char** encodedMessage  = NULL,
                              char*  encodedLength   = NULL );
        
        static void encode ( BENDataPackage* package );
        
        static BENDataPackage decode ( char* message,
                                       char  length = 0 );

        static char calculateEncodedLength ( char messageLength );

        static char checkSum ( char message[], char messageLength = 4, char startWithByte = 0);
        
    };
}

#endif //___BENDataPackage_h___
