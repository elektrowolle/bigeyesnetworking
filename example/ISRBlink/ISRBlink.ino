/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#include <avr/wdt.h>        // Supplied Watch Dog Timer Macros 
#include <avr/sleep.h>      // Supplied AVR Sleep Macros



// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

boolean badDream = false;
boolean ISRToggled = false;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  
  cli();    // Disable the Interrupts
  
  if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
    MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
    WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
    WDTCSR = 0x00;                      // Disable the WDT
    badDream = true;
    
  }
  // Set up Watch Dog Timer for Inactivity
  WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
  WDTCSR =   _BV(WDIE) |              // Enable WDT Interrupt
             _BV(WDP3);// | _BV(WDP1);   // Set Timeout to ~1 seconds
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Set Sleep Mode: Power Down
  sleep_enable(); 
  sei();
}

// the loop routine runs over and over again forever:
void loop() {
  if(badDream){
    Serial.print(millis());
    Serial.println(" Bad Dream");
  }
  
  if(ISRToggled){
    Serial.print(millis());
    Serial.println(" ISR toggled");
    ISRToggled = false;
  }
   
   if (MCUCR & _BV(SE)){    // If Sleep is Enabled...
     cli();                 // Disable Interrupts
     sei();                 // Enable Interrupts
     sleep_cpu();           // Go to Sleep
     
     
 /****************************
  *   Sleep Until WDT Times Out  
  *   -> Go to WDT ISR   
  ****************************/

   }else {
     sleep_enable(); 
   }
}

ISR(WDT_vect)
{
  
  
  sleep_disable();          // Disable Sleep on Wakeup
  
  ISRToggled = true;
  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  
  sleep_enable();           // Enable Sleep Mode
}
