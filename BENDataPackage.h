class BENDataPackage {
public:
    char encodedMessage [];
    char sender           ;
    char receiver         ;
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
    
    static bool encode ( BENDataPackage *package, char retValue[] );
    
    static BENDataPackage decode ( char  message[], 
                                   char  length = 0 );

    static char calculateEncodedLength ( char messageLength );

    static char checkSum ( char message[], char messageLength );

    bool encode();
};
