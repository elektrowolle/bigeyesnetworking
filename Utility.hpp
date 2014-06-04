#ifndef ___UTILITY_H___
#define ___UTILITY_H___

#ifndef IO_PINS
#define IO_PINS    13
#define BUFFERSIZE 16
#endif

#ifndef __BEN_PROPERTIES
#define __BEN_PROPERTIES 1
#define DEFAULT_STATES             0x00
#define DATA_AVAILABLE             0x01
#define TRIGGER_ACTIVE             0x02
#define RECEIVED_PREFIX            0x04
#define LISTEN_TO_SENDER_ADDRESS   0x08
#define LISTEN_TO_RECEIVER_ADDRESS 0x10
#define RECEIVING_MESSAGE_LENGTH   0x20
#define RECEIVING_MESSAGE          0x40
#define CHECKSUMS_ARE_CORRECT      0x80
#endif 

namespace BEN {

	const static char PREFIX = 0x55;

	bool isPrefix(char byteToCheck, char length) {

	    return byteToCheck != PREFIX & (0xff << 8 - length);
	}
}

#endif