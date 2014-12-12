#include "BENDuinoIO.h"
namespace BEN{
	BENDuinoIO::BENDuinoIO (int address, char netPin){
		// funcContainer fc;
		// fc = BEN::transmit;
		ben = new BENNetwork(address, 
												 (arg2FuncContainer<void, BENNetwork*, char>) {BEN::statesChanged});

	}

	BENDuinoIO::~BENDuinoIO (){
		delete ben;
	}

	void statesChanged (BENNetwork* ben, char changedBytes){
		if(GET_ACTIVITY(changedBytes) != 0){
			switch(ben->getActivity()){
				case RECEIVING_PREFIX:

				break;

				case LISTEN_TO_SENDER_ADDRESS:
				break;

				case LISTEN_TO_RECEIVER_ADDRESS:
				break;

				case RECEIVING_MESSAGE_LENGTH :
				break;

				case RECEIVING_MESSAGE:
				break; 
			}

		}

		switch(changedBytes & (!0x1C)) {
			case IN_PROGRESS:
				if(ben->checkState(IN_PROGRESS)){
					benIO->onIN_PROGRESSActivates(ben);
				}else{
					benIO->onIN_PROGRESSDeactivates(ben);
				}
			break;

			case TRIGGER_ACTIVE:
				if(ben->checkState(TRIGGER_ACTIVE)){
					benIO->onTRIGGER_ACTIVEActivates(ben);
				}else{
					benIO->onTRIGGER_ACTIVEDeactivates(ben);
				}
			break;

			case DATA_READY:
				if(ben->checkState(DATA_READY)){
					benIO->onDATA_READYActivates(ben);
				}else{
					benIO->onDATA_READYDeactivates(ben);
				}
			break;

			case ABORTED:
				if(ben->checkState(ABORTED)){
					benIO->onABORTEDActivates(ben);
				}else{
					benIO->onABORTEDDeactivates(ben);
				}
			break;

			case CHECKSUMS_ARE_INCORRECT:
				if(ben->checkState(CHECKSUMS_ARE_INCORRECT)){
					benIO->onCHECKSUMS_ARE_INCORRECTActivates(ben);
				}else{
					benIO->onCHECKSUMS_ARE_INCORRECTDeactivates(ben);
				}
			break;

		}
		benIO->onStatesChanged(ben);
	}

};