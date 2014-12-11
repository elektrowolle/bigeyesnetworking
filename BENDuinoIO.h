#ifndef ___BENDuinoIO_h__
#define ___BENDuinoIO_h__ 

#include "BENClass.h"

/// time in cycles between a scan
#define scandt             5
/// time in scandt to match a logic LOW
#define benLOWv            3
/// accurancy in fractions of cycles for logic LOW
#define benLOWa            2
/// accurancy in fractions of cycles for logic HIGH
#define benHIGHa           1
/// time in scandt to match a logic HIGH
#define benHIGHv           9
/// time in scandt to match a Seperator
#define benSEPTv           1
/// accurancy in fractions of cycles for seperator
#define benSEPTa           2
/// time in scandt to claim line as free
#define benFREEv          12
/// determine if signal is logic LOW
#define benLOWt(t0,tn)     (abs((tn - t0) -  benLOWv  * scandt) <= scandt / benLOWa )
/// determine if signal is logic HIGH
#define benHIGHt(t0,tn)    (abs((tn - t0) -  benHIGHv * scandt) <= scandt / benHIGHa)
/// determine if signal is a seperator
#define benSept(t0,tn)     (abs((tn - t0) -  benSEPTv * scandt) <= scandt / benSEPTa)
/// determine if signal (v)alue fits to signal length
#define benValuet(t0,tn,v) (v?benHIGHt(t0,tn):benLOWt(t0,tn))
/// determine if signal line is free
#define benFreet(t0,tn)    (tn - t0 > benFREEv * scandt)

namespace BEN{
	class BENDuinoIO
	{
	public:
		BENNetwork * ben;
		
		
		List< funcContainer                    > * onLineBecameFreeF    ;
		List< funcContainer                    > * onReadyForNextBitF   ;
		List< funcContainer                    > * onLineBecameOccupiedF;
		List< funcContainer                    > * onIsOccupiedF        ;
		//List< BENNetwork::voidBENDataPackageFuncPtr > * onReceivedMessageF;

		BENDuinoIO  ( int id = 0, char netPin = 2 );
		~BENDuinoIO ( );

		void loop ( );
	private:
		unsigned long lastBitT       = 0;
		unsigned long lastScanT      = 0;
		unsigned long lastTransmiT   = 0;
		unsigned long occupiedSinceT = 0;
		bool          bufferedBit    = 0;
		char          netPin;
        
        bool seperator    = false;
		bool transmitting = false;

	};
	extern BENDuinoIO benIO;
	void statesChanged(char changedBytes);
}
#endif //___BENDuino_h__