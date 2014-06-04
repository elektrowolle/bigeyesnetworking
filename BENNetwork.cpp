#include "BENNetwork.h"
#include "BENDataPackage.h"

namespace BENNetwork {

    BENNetwork::BENNetwork(int pin, int address, void (*userFunc)(void)) {
        BEN(pin, address);

        this->intFunc = userFunc;
    }

    BENNetwork::BENNetwork(int pin, int address) {
        this->ADDRESS = address;
        this->PIN     = pin;

        this->clearMessage();

        //bc.attach(this->PIN, this); Wrong Place to do it.
    }


    bool BENNetwork::send(int address, char *message[] ) {
        return false;
    }

    void BENNetwork::trigger() {
        if(TRIGGER_ACTIVE) this->intFunc();
    }

    void BENNetwork::listen(bool receivedBit) {
        
        //  I'll probably forget one day what I've been doing here. So I'll explain about.
        //  Everytime this method is called we get one new bit. Also this method utilize
        //  The members [receivedBitBuffer], [receivedBitBuffer], the 
        //  flag [receivedPREFIX] and the mask [PREFIX].
        //
        //  Basically the purpose of this method is to collect single bits and arange 
        //  them to a byte ([receivedBitBuffer]) to give them to the byte[listen]er 
        //  which will process this byte. At the same time it does some funny but 
        //  necessary checks. Let's do the fun Part!:
        //
        //  So we start with an empty [receivedBitBuffer] (0x00). As it's documentated
        //  in the header the first byte to receive at a new message is the header. That 
        //  is to prevent interpreting random data. So what we're looking for at the 
        //  beginning is a sequence with 0x55 (binary: {0101 0101}). As we listen bit by 
        //  bit we have to check if any of these bits are the starting point of a new 
        //  message. So simply spoken we check everytime if the last incoming bits had 
        //  been same as [PREFIX]. If they aren't equal we empty the buffer and try again 
        //  with the next set of bits. As soon as we found a sequence which is equal to 
        //  [PREFIX] we'll set the [receivedPREFIX] Flag, collect more bits to crunch 
        //  them into bytes and forward them to the byte[listen]er. But how does that 
        //  work?
        //  
        //  Our empty Buffer looks like this: {0000 0000}
        //  Now we get a new bit, say '1'. We want to put this bit to the MSB of the 
        //  Buffer (The first position from left). To do this we will add the bit to the 
        //  buffer to get {0000 0001} and now rotate the buffer by 7 to left. The result
        //  will be {1000 0000}. This value will be compared with [PREFIX]. To do this 
        //  it's necesarry to mask all bits of [PREFIX] which aren't yet set in the 
        //  buffer. That's done by doing a binary 'and' ('&') operation with a byte full 
        //  of ones {1111 1111} which is rotated by the same amount as the buffer is 
        //  rotated yet (7 times). what is left is {1000 0000}. Doing an and operation 
        //  with the [PREFIX] {0101 0101} will result in {0000 0000}. That looks like a 
        //  complicated way to produce a byte of zeros but it's purpose will be released 
        //  in short time. This result compared to the buffer ({1000 0000} == {0000 0000}}
        //  Is not the same. That means that the process have to start over. This 
        //  probbably means that something gibberish alien stuff occured on our data 
        //  line. Let us ignore the fact of aliens, listen to the next bit and continue:
        //  
        //  The next bit received in our example is a 0. we do the same process as 
        //  before: rotate the buffer by 7, pick up our [PREFIX] mask and mask which we 
        //  rotate by 7. We compare both ({0000 0000} == {0000 0000}) and 
        //  have a match! Let us continue. next bit is a 1. rotate it by 6, grab the
        //  [PREFIX] mask it (after rotating the mask by 6). Compare 
        //  ({0100 0000} == {0100 0000}) and it's a match again! We do this routine 
        //  till we have a complete match between [receivedBitBuffer] and [PREFix]
        //  ({0101 0101} == {0101 0101}). We found the prefix! It's super likely to get
        //  Usefull data from now on. That's a good thing. Let's celebrate by setting 
        //  the [receivedPREFIX] = true.
        //  


        this->receivedBitBuffer += (this->receivedBitBuffer ? 1 : 0) << 7 - this->receivedBitBuffer;
        this->receivedBitBuffer++;

        if (
            (this->STATES | RECEIVED_PREFIX) == 0 
            && !BEN::isPrefix(this->receivedBitBuffer, this->receivedBitBuffer)) {
            
            this->resetBitBuffer();

        } else if (receivedBitBuffer > 7){
            this->STATES |= RECEIVED_PREFIX;
            this->resetBitBuffer();
        }
        
        if (receivedBitBuffer > 7) {
            this->listen(receivedBitBuffer);
            this->resetBitBuffer();
        }
    }

    void BENNetwork::listen(char receivedByte) {

        if ((STATES & RECEIVING_MESSAGE) != 0){

        } else if ((STATES & RECEIVING_MESSAGE_LENGTH) != 0){

        } else if ((STATES & LISTEN_TO_RECEIVER_ADDRESS) != 0){

        } else if ((STATES & LISTEN_TO_SENDER_ADDRESS) != 0){

        } else if ((STATES & RECEIVED_PREFIX) != 0){
            availableData = new BENDataPackage ();
        }
    }

    void BENNetwork::resetBitBuffer() {
        this->receivedBitBuffer         = 0;
        this->receivedBitBufferPosition = 0;
    }

    void BENNetwork::resetFlags() {
        this->STATES = DEFAULT_STATES;
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
