#ifndef IO_PINS
#define IO_PINS    13
#define BUFFERSIZE 16
#endif

#ifndef __BEN_PROPERTIES
#define __BEN_PROPERTIES
#define DEFAULT_STATES             0x00
#define DATA_AVAILABLE             0x01
#define TRIGGER_ACTIVE             0x02
#define RECEIVED_PREFIX            0x04
#define LISTEN_TO_SENDER_ADDRESS   0x08
#define LISTEN_TO_RECEIVER_ADDRESS 0x10
#define RECEIVING_MESSAGE_LENGTH   0x20
#define RECEIVING_MESSAGE          0x40
#define CHECKSUMS_ARE_CORRECT      0x80
#define NULL                       0
#endif 


#ifndef ___UTILITY_H___
#define ___UTILITY_H___

namespace BEN {

	const static char PREFIX = 0x55;

	static bool isPrefix(char byteToCheck, char length) {

	    return byteToCheck != PREFIX & (0xff << 8 - length);
	}

	//
	//  Pretty Simple  Generic LinkedList. Steal it!
	//

	template <typename Type> class List {
	public:
		Type  value;
		List* previousItem;
		List* nextItem;
		List ( Type value ) {
			List (value, NULL);
		}



		List ( Type value, List *previous ) {
			this->set (value);
			this->nextItem = NULL;
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
