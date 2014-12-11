#include "BENNetwork.h"
//#include "BENDataPackage.h"

namespace BEN {

  BENNetwork::BENNetwork(int localAddress, arg1FuncContainer<void, char> _statesChange) {

      this->LOCAL_ADDRESS = localAddress;

      this->clearMessage();
      // this->intFunc = userFunc;
      this->STATES  = DEFAULT_STATES;
      this->availableData = new BENDataPackage();
      this->statesChange = _statesChange;
  }


  bool BENNetwork::send(int address, char *message[] ) {
      return false;
  }

    

	void BENNetwork::addToBitBuffer(bool receivedBit) {
		this->receivedBitBuffer += (receivedBit ? 1 : 0)
				<< (7 - this->receivedBitBufferPosition);
		this->receivedBitBufferPosition ++;
	}

	void BENNetwork::listenToPrefix(bool receivedBit) {
		if (!BEN::isPrefix(this->receivedBitBuffer,
				this->receivedBitBufferPosition)) {
			this->resetBitBuffer();
			this->addToBitBuffer(receivedBit);

		} else if (this->receivedBitBufferPosition > 7) {
			this->changeActivity(LISTEN_TO_SENDER_ADDRESS);
			this->deactivateState(IN_PROGRESS);
			this->resetBitBuffer();
		}
	}

	/**
	 *
	 * @param receivedBit
	 * @details ' I'll probably forget one day what I've been doing here. So I'll explain about.
          Everytime this method is called we get one new bit. Also this method utilize
          The members [receivedBitBuffer], [receivedBitBuffer], the
          flag [receivedPREFIX] and the mask [PREFIX].

          Basically the purpose of this method is to collect single bits and arange
          them to a byte ([receivedBitBuffer]) to give them to the byte[listen]er
          which will process this byte. At the same time it does some funny but
          necessary checks. Let's do the fun Part!:

          So we start with an empty [receivedBitBuffer] (0x00). As it's documentated
          in the header the first byte to receive at a new message is the header. That
          is to prevent interpreting random data. So what we're looking for at the
          beginning is a sequence with 0x55 (binary: {0101 0101}). As we listen bit by
          bit we have to check if any of these bits are the starting point of a new
          message. So simply spoken we check everytime if the last incoming bits had
          been same as [PREFIX]. If they aren't equal we empty the buffer and try again
          with the next set of bits. As soon as we found a sequence which is equal to
          [PREFIX] we'll set the [receivedPREFIX] Flag, collect more bits to crunch
          them into bytes and forward them to the byte[listen]er. But how does that
          work?

          Our empty Buffer looks like this: {0000 0000}
          Now we get a new bit, say '1'. We want to put this bit to the MSB of the
          Buffer (The first position from left). To do this we will add the bit to the
          buffer to get {0000 0001} and now rotate the buffer by 7 to left. The result
          will be {1000 0000}. This value will be compared with [PREFIX]. To do this
          it's necesarry to mask all bits of [PREFIX] which aren't yet set in the
          buffer. That's done by doing a binary 'and' ('&') operation with a byte full
          of ones {1111 1111} which is rotated by the same amount as the buffer is
          rotated yet (7 times). what is left is {1000 0000}. Doing an and operation
          with the [PREFIX] {0101 0101} will result in {0000 0000}. That looks like a
          complicated way to produce a byte of zeros but it's purpose will be released
          in short time. This result compared to the buffer ({1000 0000} == {0000 0000}}
          Is not the same. That means that the process have to start over. This
          probbably means that something gibberish alien stuff occured on our data
          line. Let us ignore the fact of aliens, listen to the next bit and continue:

          The next bit received in our example is a 0. we do the same process as
          before: rotate the buffer by 7, pick up our [PREFIX] mask and mask which we
          rotate by 7. We compare both ({0000 0000} == {0000 0000}) and
          have a match! Let us continue. next bit is a 1. rotate it by 6, grab the
          [PREFIX] mask it (after rotating the mask by 6). Compare
          ({0100 0000} == {0100 0000}) and it's a match again! We do this routine
          till we have a complete match between [receivedBitBuffer] and [PREFix]
          ({0101 0101} == {0101 0101}). We found the prefix! It's super likely to get
          Usefull data from now on. That's a good thing. Let's celebrate by setting
          the [receivedPREFIX] = true.
          '

     *
     *
	 */

    void BENNetwork::listen(bool receivedBit) {
        
        this->addToBitBuffer(receivedBit);

        if (this->STATES == DEFAULT_STATES) {
        	this->changeActivity(RECEIVING_PREFIX);
        	this->activateState(IN_PROGRESS);

        } else if (this->checkState(DATA_READY)) {
			return ;
		}

        if (this->checkActivity(RECEIVING_PREFIX) && this->checkState(IN_PROGRESS)){
            listenToPrefix(receivedBit);
        }

        if (this->receivedBitBufferPosition > 7) {
            this->listen(receivedBitBuffer);
            this->resetBitBuffer();
        }
    }

	void BENNetwork::listenToReceiverAddress(char receivedByte) {
		if (!this->checkState(IN_PROGRESS)) {
			this->availableData->setReceiverMSB(receivedByte);

			this->activateState(IN_PROGRESS);

		} else {
			this->availableData->setReceiverLSB(receivedByte);

			this->deactivateState(IN_PROGRESS);
			this->changeActivity (RECEIVING_MESSAGE_LENGTH);
		}
	}

	void BENNetwork::listenToMessageLength(char receivedByte) {
		this->availableData->setMessageLength(receivedByte, true);

		this->changeActivity(RECEIVING_MESSAGE);
		this->activateState (IN_PROGRESS);
	}

	void BENNetwork::listenToSenderAddress(char receivedByte) {
		if (!this->checkState(IN_PROGRESS)) {
			this->availableData->setSenderMSB(receivedByte);

			this->activateState(IN_PROGRESS);

		} else {
			this->availableData->setSenderLSB(receivedByte);

			this->deactivateState(IN_PROGRESS);
			this->changeActivity (LISTEN_TO_RECEIVER_ADDRESS);
		}
	}

	void BENNetwork::listenToMessage(char receivedByte) {
		if (this->availableData->nextByteIsChechSum()) {
			if(!this->availableData->checksumIsValid(receivedByte)) {
				this->activateState(CHECKSUMS_ARE_INCORRECT);
				this->activateState(DATA_READY);
			}
			this->availableData->messagePosition++;

		} else {
			this->availableData->addToMessage(receivedByte);

		}
		if (this->availableData->bytesOfMessageReceived()
				>= this->availableData->getMessageLength()) {
			this->deactivateState(IN_PROGRESS);
			this->activateState (DATA_READY);
		}
	}

  void BENNetwork::listen(char receivedByte) {
      if (this->checkActivity(RECEIVING_MESSAGE)) {
      	listenToMessage(receivedByte);

      } else if (checkActivity(RECEIVING_MESSAGE_LENGTH)) {
      	listenToMessageLength(receivedByte);

      } else if (this->checkActivity(LISTEN_TO_RECEIVER_ADDRESS)) {
      	listenToReceiverAddress(receivedByte);

      } else if (this->checkActivity(LISTEN_TO_SENDER_ADDRESS)) {
      	listenToSenderAddress(receivedByte);
      }
      
  }

  void BENNetwork::resetBitBuffer() {
    this->receivedBitBuffer         = 0;
    this->receivedBitBufferPosition = 0;
  }

  void BENNetwork::resetFlags() {
    char _previousStates = this->STATES;
    this->STATES = DEFAULT_STATES;
    statesChange(this->STATES ^ _previousStates);
  }

  void BENNetwork::activateState(char stateByte) {
    char _previousStates = this->STATES;
    this->STATES |= stateByte;
    statesChange(this->STATES ^ _previousStates);
  }

  void BENNetwork::deactivateState(char stateByte) {
    char _previousStates = this->STATES;
    this->STATES &= ~stateByte;
    statesChange(this->STATES ^ _previousStates);
  }

  void BENNetwork::changeActivity (char stateByte) {
      /**
       *  //
		// EXAMPLE:
		//   Current activity is RECEIVING_PREFIX. The Trigger is active as well.
		//   The activity is going to be changed to RECEIVING_MESSAGE.
		//
		//   this->changeActivity(0x10);
		//
		// STATES   : [0000 0110]
		// stateByte: [0001 0000]
		//
		// 01: STATES & ~ACTIVITY_MASK
		//     [0000 0110]
		//   &~[1110 0011]
		//   =============
		//     [0000 0010] result 1
		//
		// 02: stateByte & ACTIVITY_MASK
		//     [0001 0000]
		//   & [0001 1100]
		//   =============
		//     [0001 0000] result 2
		//
		// 03: result1 | result 2
		//     [0000 0010]
		//   | [0001 0000]
		//   =============
		//     [0001 0010]
		//
	*
	*/
    char _previousStates = this->STATES;
    this->STATES = (this->STATES & ~ACTIVITY_MASK) | (stateByte & ACTIVITY_MASK);
    statesChange(this->STATES ^ _previousStates);
  }

  bool BENNetwork::checkActivity(char activity) {
    return CHECK_ACTIVITY(this->STATES, activity);
  }

  bool BENNetwork::checkState(char stateByte) {
    return CHECK_STATE(this->STATES, stateByte);
  }

  void BENNetwork::clearMessage() {
    this->receivedBitBuffer         = 0;
    this->receivedBitBufferPosition = 0;
    
    delete availableData;
  }

}

//
// ISR
//

// ISR (WDT_vect) {
//     //sleep_disable();        // Disable Sleep on Wakeup

//     //bc.listen();

//     //sleep_enable();         // Enable Sleep Mode
// }
