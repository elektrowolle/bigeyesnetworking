#include "BENClass.h"
//
//  BENClass
//

void BENClass::init () {

}

void BENClass::enable() {
    ENABLED = true;
}

void BENClass::trigger(int pin) {
    network[pin]->trigger();
}

void BENClass::attach(int pin, BEN *newNetwork) {
    network[pin] = newNetwork;
}

BENClass::BENClass() {
    INITIALISED    = false;
    RECEIVING      = false;
}



bool BENClass::isPrefix(char byteToCheck, char length) {

    return byteToCheck != PREFIX & (0xff << 8 - length);
}


BENClass bc;