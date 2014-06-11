#ifndef IO_PINS
#define IO_PINS    13
#define BUFFERSIZE 16
#endif

// #ifndef __BEN_PROPERTIES
// #define __BEN_PROPERTIES
// #define DEFAULT_STATES             0x00
// #define IN_PROGRESS                0x01
// #define TRIGGER_ACTIVE             0x02
// #define RECEIVED_PREFIX            0x04
// #define LISTEN_TO_SENDER_ADDRESS   0x08
// #define LISTEN_TO_RECEIVER_ADDRESS 0x10
// #define RECEIVING_MESSAGE_LENGTH   0x20
// #define RECEIVING_MESSAGE          0x40
// #define CHECKSUMS_ARE_INCORRECT    0x80
// #define NULL                       0
// #endif 

//  [0000 0000]
//   |||| |||\___{  1} (ACTIVITY_)IN_PROGRESS
//   |||| ||\____{  2} TRIGGER_ACTIVE
//   |||| ||              
//   |||| |\_____ }                    {  4} RECEIVING_PREFIX          
//   |||| \______ }    Activity Bits   {  8} LISTEN_TO_SENDER_ADDRESS
//   |||\________ }                    { 12} LISTEN_TO_RECIVER_ADDRESS
//   |||                               { 16} RECEIVING_MESSAGE_LENGTH
//   |||                               { 20} RECEIVING_MESSAGE
//   |||                               { 24} RESERVED (NO FUNCTION)
//   |||                               { 28} RESERVED (NO FUNCTION)
//   |||                               
//   |||
//   ||\_________{ 32} DATA_READY
//   |\__________{ 64} ABORTED
//   \___________{128} CHECKSUMS_ARE_INCORRECT

#ifndef __BEN_PROPERTIES
#define __BEN_PROPERTIES
//MASKS
#define DEFAULT_STATES             0x00
#define ACTIVITY_MASK              0x1C

//NETWORK
#define IN_PROGRESS                0x01
#define TRIGGER_ACTIVE             0x02

//ACTIVITY BITSS
#define RECEIVING_PREFIX           0x04
#define LISTEN_TO_SENDER_ADDRESS   0x08
#define LISTEN_TO_RECEIVER_ADDRESS 0x0C
#define RECEIVING_MESSAGE_LENGTH   0x10
#define RECEIVING_MESSAGE          0x14
// #define [undefined]                0x18
// #define [undefined]                0x1C

//DATA HEALTH
#define DATA_READY                 0x20
#define ABORTED                    0x40
#define CHECKSUMS_ARE_INCORRECT    0x80

#define FIRST_CHECKSUM             0x01
#define CHECKSUM_PERIOD            0x04
#define NULL                       0x00
#endif 

#ifndef ___UTILITY_H___
#define ___UTILITY_H___

namespace BEN {

	const static char PREFIX = 0x55;

	static bool isPrefix(char byteToCheck, char length) {
		char mask         = 0xff << (8 - length);
		char maskedPrefix = (PREFIX & mask);
	    return byteToCheck == maskedPrefix;
	}

	//
	//  Pretty Simple  Generic LinkedList. Steal it!
	//

	template <typename Type> class List {
	public:
		Type  value;
		List* previousItem;
		List* nextItem;

		List ( Type value, List *previous = NULL ) {
			this->set (value);
			this->previousItem = NULL;
			this->nextItem     = NULL;
		}

		~List (  ) {
			if(nextItem != NULL)
				delete nextItem;
		}

		List* add ( Type value ) {
			if (next() == NULL) {
				return this->nextItem = new List ( value, this );
			}else {
				return next()->add(value); 
			}
			
		}

		void remove (  ) {
			this->previous->nextItem = this->nextItem;
			this->next->previousItem = this->previousItem;
			this->previousItem = NULL;
			this->nextItem     = NULL;

			delete this;
		}

		List* next (  ) {
			return nextItem;
		}

		List* previous (  ) {
			return previousItem;
		}

		Type get (  ) {
			return value;
		}

		void set (Type value) {
			this->value = value;
		}
	};
}

#endif // ___UTILITY_H___
